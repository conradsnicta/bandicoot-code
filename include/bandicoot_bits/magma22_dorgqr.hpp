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


/***************************************************************************//**
    Purpose
    -------
    DORGQR generates an M-by-N DOUBLE PRECISION matrix Q with orthonormal columns,
    which is defined as the first N columns of a product of K elementary
    reflectors of order M

          Q  =  H(1) H(2) . . . H(k)

    as returned by DGEQRF.

    Arguments
    ---------
    @param[in]
    m       INTEGER
            The number of rows of the matrix Q. M >= 0.

    @param[in]
    n       INTEGER
            The number of columns of the matrix Q. M >= N >= 0.

    @param[in]
    k       INTEGER
            The number of elementary reflectors whose product defines the
            matrix Q. N >= K >= 0.

    @param[in,out]
    A       DOUBLE PRECISION array A, dimension (LDDA,N).
            On entry, the i-th column must contain the vector
            which defines the elementary reflector H(i), for
            i = 1,2,...,k, as returned by DGEQRF_GPU in the
            first k columns of its array argument A.
            On exit, the M-by-N matrix Q.

    @param[in]
    lda     INTEGER
            The first dimension of the array A. LDA >= max(1,M).

    @param[in]
    tau     DOUBLE PRECISION array, dimension (K)
            TAU(i) must contain the scalar factor of the elementary
            reflector H(i), as returned by DGEQRF_GPU.

    @param[in]
    dT      DOUBLE PRECISION array on the GPU device.
            DT contains the T matrices used in blocking the elementary
            reflectors H(i), e.g., this can be the 6th argument of
            magma_dgeqrf_gpu.

    @param[in]
    nb      INTEGER
            This is the block size used in DGEQRF_GPU, and correspondingly
            the size of the T matrices, used in the factorization, and
            stored in DT.

    @param[out]
    info    INTEGER
      -     = 0:  successful exit
      -     < 0:  if INFO = -i, the i-th argument had an illegal value

    @ingroup magma_ungqr
*******************************************************************************/
inline
magma_int_t
magma_dorgqr(
    magma_int_t m, magma_int_t n, magma_int_t k,
    double *A, magma_int_t lda,
    double *tau,
    magmaDouble_ptr dT, magma_int_t nb,
    magma_int_t *info)
{
    // #define  A(i,j) ( A + (i) + (j)*lda )
    // #define dA(i,j) (dA + (i) + (j)*ldda)
    // #define dT(j)   (dT + (j)*nb)
    
    // CS: adapt for clBLAS
    #define  A(i_,j_)  A, (i_) + (j_)*lda 
    #define dA(i_,j_) dA, (i_) + (j_)*ldda
    #define dT(j_)    dT, (j_)*nb
    
    double c_zero = MAGMA_D_ZERO;
    double c_one  = MAGMA_D_ONE;

    magma_int_t  m_kk, n_kk, k_kk, mi;
    magma_int_t lwork, ldda;
    magma_int_t i, ib, ki, kk;
    magma_int_t lddwork;
    double *dA=NULL, *dV=NULL, *dW=NULL;
    double *work=NULL;
    magma_queue_t queue=NULL;

    *info = 0;
    if (m < 0) {
        *info = -1;
    } else if ((n < 0) || (n > m)) {
        *info = -2;
    } else if ((k < 0) || (k > n)) {
        *info = -3;
    } else if (lda < std::max(1,m)) {
        *info = -5;
    }
    if (*info != 0) {
        // magma_xerbla( __func__, -(*info) );
        return *info;
    }

    if (n <= 0) {
        return *info;
    }

    // first kk columns are handled by blocked method.
    // ki is start of 2nd-to-last block
    if ((nb > 1) && (nb < k)) {
        ki = (k - nb - 1) / nb * nb;
        kk = std::min(k, ki + nb);
    } else {
        ki = 0;
        kk = 0;
    }

    // Allocate GPU work space
    // ldda*n     for matrix dA
    // ldda*nb    for dV
    // lddwork*nb for dW larfb workspace
    ldda    = magma_roundup( m, 32 );
    lddwork = magma_roundup( n, 32 );
    if (MAGMA_SUCCESS != magma_dmalloc( &dA, ldda*n + ldda*nb + lddwork*nb )) {
        *info = MAGMA_ERR_DEVICE_ALLOC;
        goto cleanup;
    }
    dV = dA + ldda*n;
    dW = dA + ldda*n + ldda*nb;

    // Allocate CPU work space
    // n*nb  for larfb work
    // m*nb  for V
    // nb*nb for T
    lwork = (n + m + nb) * nb;
    magma_dmalloc_cpu( &work, lwork );
    if (work == NULL) {
        *info = MAGMA_ERR_HOST_ALLOC;
        goto cleanup;
    }
    double *work_T, *work_V;
    work_T = work + n*nb;
    work_V = work + n*nb + nb*nb;

    magma_device_t cdev;
    magma_getdevice( &cdev );
    magma_queue_create( cdev, &queue );

    // Use unblocked code for the last or only block.
    if (kk < n) {
        m_kk = m - kk;
        n_kk = n - kk;
        k_kk = k - kk;
        
        // dorgqr requires less workspace (n*nb), but is slow if k < dorgqr's block size.
        // replacing it with the 4 routines below is much faster (e.g., 60x).
        //magma_int_t iinfo;
        //lapackf77_dorgqr( &m_kk, &n_kk, &k_kk,
        //                  A(kk, kk), &lda,
        //                  &tau[kk], work, &lwork, &iinfo );
        
        lapackf77_dlacpy( MagmaFullStr, &m_kk, &k_kk, A(kk,kk), &lda, work_V, &m_kk);
        lapackf77_dlaset( MagmaFullStr, &m_kk, &n_kk, &c_zero, &c_one, A(kk, kk), &lda );
        
        lapackf77_dlarft( MagmaForwardStr, MagmaColumnwiseStr,
                          &m_kk, &k_kk,
                          work_V, &m_kk, &tau[kk], work_T, &k_kk);
        lapackf77_dlarfb( MagmaLeftStr, MagmaNoTransStr, MagmaForwardStr, MagmaColumnwiseStr,
                          &m_kk, &n_kk, &k_kk,
                          work_V, &m_kk, work_T, &k_kk, A(kk, kk), &lda, work, &n_kk );
        
        if (kk > 0) {
            magma_dsetmatrix( m_kk, n_kk,
                              A(kk, kk),  lda,
                              dA(kk, kk), ldda, queue );
        
            // Set A(1:kk,kk+1:n) to zero.
            magmablas_dlaset( MagmaFull, kk, n - kk, c_zero, c_zero, dA(0, kk), ldda, queue );
        }
    }

    if (kk > 0) {
        // Use blocked code
        // queue: set Aii (V) --> laset --> laset --> larfb --> [next]
        // CPU has no computation
        
        for (i = ki; i >= 0; i -= nb) {
            ib = std::min(nb, k - i);

            // Send current panel to dV on the GPU
            mi = m - i;
            lapackf77_dlaset( "Upper", &ib, &ib, &c_zero, &c_one, A(i, i), &lda );
            magma_dsetmatrix_async( mi, ib,
                                    A(i, i), lda,
                                    dV,      ldda, queue );

            // set panel to identity
            magmablas_dlaset( MagmaFull, i,  ib, c_zero, c_zero, dA(0, i), ldda, queue );
            magmablas_dlaset( MagmaFull, mi, ib, c_zero, c_one,  dA(i, i), ldda, queue );
            
            if (i < n) {
                // Apply H to A(i:m,i:n) from the left
                magma_dlarfb_gpu( MagmaLeft, MagmaNoTrans, MagmaForward, MagmaColumnwise,
                                  mi, n-i, ib,
                                  dV,       ldda, dT(i), nb,
                                  dA(i, i), ldda, dW, lddwork, queue );
            }
        }
    
        // copy result back to CPU
        magma_dgetmatrix( m, n,
                          dA(0, 0), ldda, A(0, 0), lda, queue );
    }

cleanup:
    magma_queue_destroy( queue );
    magma_free( dA );
    magma_free_cpu( work );

    return *info;
} /* magma_dorgqr */
