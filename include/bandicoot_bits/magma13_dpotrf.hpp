// Copyright 2017 Conrad Sanderson (http://conradsanderson.id.au)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------


// This file contains source code adapted from
// clMAGMA 1.3 (2014-11-14) and/or MAGMA 2.2 (2016-11-20).
// clMAGMA 1.3 and MAGMA 2.2 are distributed under a
// 3-clause BSD license as follows:
// 
//  -- Innovative Computing Laboratory
//  -- Electrical Engineering and Computer Science Department
//  -- University of Tennessee
//  -- (C) Copyright 2009-2015
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the University of Tennessee, Knoxville nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
//  This software is provided by the copyright holders and contributors
//  ``as is'' and any express or implied warranties, including, but not
//  limited to, the implied warranties of merchantability and fitness for
//  a particular purpose are disclaimed. In no event shall the copyright
//  holders or contributors be liable for any direct, indirect, incidental,
//  special, exemplary, or consequential damages (including, but not
//  limited to, procurement of substitute goods or services; loss of use,
//  data, or profits; or business interruption) however caused and on any
//  theory of liability, whether in contract, strict liability, or tort
//  (including negligence or otherwise) arising in any way out of the use
//  of this software, even if advised of the possibility of such damage.


inline
magma_int_t
magma_get_dpotrf_nb( magma_int_t m )
  {
  if      (m <= 4256) return 128;
  else                return 256;
  }


// TODO: fix this shit to something resembling sanity
// original note: produces pointer and offset as two args to magmaBLAS routines
#undef  dA
#define dA(i,j)  dA, ( (dA_offset) + (i) + (j)*ldda )

// produces pointer as single arg to BLAS routines
#undef  A
#define A(i,j)  &A[ (i) + (j)*lda ]


inline
magma_int_t
magma_dpotrf_gpu(magma_uplo_t uplo, magma_int_t n, magmaDouble_ptr dA, size_t dA_offset, magma_int_t ldda, magma_queue_t queue, magma_int_t* info)
  {
  magma_int_t j, jb, nb;
  double  z_one = MAGMA_D_MAKE(  1.0, 0.0 );
  double mz_one = MAGMA_D_MAKE( -1.0, 0.0 );
  double  one =  1.0;
  double  m_one = -1.0;
  double* work;
  magma_int_t err;
  
  *info = 0;
  if ( uplo != MagmaUpper && uplo != MagmaLower ) {
    *info = -1;
  } else if ( n < 0 ) {
    *info = -2;
  } else if ( ldda < std::max(1,n) ) {
    *info = -4;
  }
  
  if ( *info != 0 )
    {
    // magma_xerbla( __func__, -(*info) );
    return *info;
    }
  
  nb = magma_get_dpotrf_nb( n );  // TODO: test on various GPUs to get a good range of values 
  
  err = magma_dmalloc_cpu(  &work, nb*nb );
  if ( err != MAGMA_SUCCESS )
    {
    *info = MAGMA_ERR_HOST_ALLOC;
    return *info;
    }
  
  magma_event_t event = NULL;
  
  if ((nb <= 1) || (nb >= n))
    {
    //cout << "using CPU version" << endl;
    // TODO: this code is probably unnecessary when using Armadillo to call coot::chol()
    // TODO: otherwise it only makes sense if using bandicoot as a stand-alone library
    
    // use unblocked code
    magma_dgetmatrix( n, n, dA, dA_offset, ldda, work, n, queue );  // internally writes to g_event
    
    // TODO: replace with corresponding wrapper function from Armadillo
    // TODO: need to copy LAPACK wrappers from Armadillo to allow bandicoot work in stand-alone mode
    
    //lapackf77_dpotrf( lapack_uplo_const(uplo), &n, work, &n, info );
    arma::lapack::potrf<double>( (char*)lapack_uplo_const(uplo), &n, work, &n, info );  // TODO: when adapting LAPACK wrappers, change the proto to use const char* instead of char*
    
    magma_dsetmatrix( n, n, work, n, dA, dA_offset, ldda, queue );  // internally uses g_event
    }
  else
    {
    //cout << "using GPU version" << endl;
    
    if ( uplo == MagmaUpper )
      {
      // --------------------
      // compute Cholesky factorization A = U'*U using the left looking algorithm
      
      for( j = 0; j < n; j += nb )
        {
        // apply all previous updates to diagonal block
        jb = std::min( nb, n-j );
        if ( j > 0 )
          {
          magma_dsyrk( MagmaUpper, MagmaConjTrans, jb, j, m_one, dA(0,j), ldda, one, dA(j,j), ldda, queue ); // internally uses g_event
          }
        
        // start asynchronous data transfer
        magma_dgetmatrix_async( jb, jb, dA(j,j), ldda, work, jb, queue, &event );
        
        // apply all previous updates to block row right of diagonal block
        if ( j+jb < n )
          {
          magma_dgemm( MagmaConjTrans, MagmaNoTrans, jb, n-j-jb, j, mz_one, dA(0, j   ), ldda, dA(0, j+jb), ldda, z_one,  dA(j, j+jb), ldda, queue );   // internally uses g_event
          }
        
        // simultaneous with above dgemm, transfer data, factor diagonal block on CPU, and test for positive definiteness
        
        magma_event_sync( event );
        
        //lapackf77_dpotrf( MagmaUpperStr, &jb, work, &jb, info );
        arma::lapack::potrf<double>( (char*)MagmaUpperStr, &jb, work, &jb, info );
        
        if ( *info != 0 )
          {
          assert( *info > 0 );
          *info += j;
          break;
          }
        
        magma_dsetmatrix_async( jb, jb, work, jb, dA(j,j), ldda, queue, &event );
        
        // apply diagonal block to block row right of diagonal block
        if ( j+jb < n )
          {
          magma_event_sync( event );
          magma_dtrsm( MagmaLeft, MagmaUpper, MagmaConjTrans, MagmaNonUnit, jb, n-j-jb, z_one, dA(j, j),  ldda, dA(j, j+jb), ldda, queue );    // internally uses g_event
          }
        }
      }
    else
      {
      // --------------------
      // compute Cholesky factorization A = L*L'
      // using the left looking algorithm
      for( j = 0; j < n; j += nb )
        {
        // apply all previous updates to diagonal block
        jb = std::min( nb, n-j );
        if ( j>0 )
          {
          magma_dsyrk( MagmaLower, MagmaNoTrans, jb, j, m_one, dA(j, 0), ldda, one, dA(j, j), ldda, queue );
          }

        // start asynchronous data transfer
        magma_dgetmatrix_async( jb, jb, dA(j,j), ldda, work, jb, queue, &event );

        // apply all previous updates to block column below diagonal block
        if ( j+jb < n )
          {
          magma_dgemm( MagmaNoTrans, MagmaConjTrans, n-j-jb, jb, j, mz_one, dA(j+jb, 0), ldda, dA(j,  0), ldda, z_one,  dA(j+jb, j), ldda, queue );  // internally uses g_event
          }
        
        // simultaneous with above dgemm, transfer data, factor diagonal block on CPU, and test for positive definiteness
        magma_event_sync( event );
        
        //lapackf77_dpotrf( MagmaLowerStr, &jb, work, &jb, info );
        arma::lapack::potrf<double>( (char*)MagmaLowerStr, &jb, work, &jb, info );
        
        if ( *info != 0 )
          {
          assert( *info > 0 );
          *info += j;
          break;
          }
        
        magma_dsetmatrix_async( jb, jb, work, jb, dA(j,j), ldda, queue, &event );
        
        // apply diagonal block to block column below diagonal
        if ( j+jb < n )
          {
          magma_event_sync( event );
          magma_dtrsm( MagmaRight, MagmaLower, MagmaConjTrans, MagmaNonUnit, n-j-jb, jb, z_one, dA(j, j), ldda, dA(j+jb, j), ldda, queue );  // internally uses g_event
          }
        }
      }
    }
  
  magma_queue_sync( queue );
  magma_free_cpu( work );
  
  return *info;
  }


#undef  dA
#undef  A



