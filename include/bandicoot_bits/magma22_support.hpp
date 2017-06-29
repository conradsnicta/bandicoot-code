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
void
check_error(const cl_int error_code)
  {
  coot_check_cl_error(error_code, "magma_function");
  }



// -----------------------------------------------------------------------------
// Return codes
// LAPACK argument errors are < 0 but > MAGMA_ERR.
// MAGMA errors are < MAGMA_ERR.

#define MAGMA_SUCCESS               0       ///< operation was successful
#define MAGMA_ERR                  -100     ///< unspecified error
#define MAGMA_ERR_NOT_INITIALIZED  -101     ///< magma_init() was not called
#define MAGMA_ERR_REINITIALIZED    -102     // unused
#define MAGMA_ERR_NOT_SUPPORTED    -103     ///< not supported on this GPU
#define MAGMA_ERR_ILLEGAL_VALUE    -104     // unused
#define MAGMA_ERR_NOT_FOUND        -105     ///< file not found
#define MAGMA_ERR_ALLOCATION       -106     // unused
#define MAGMA_ERR_INTERNAL_LIMIT   -107     // unused
#define MAGMA_ERR_UNALLOCATED      -108     // unused
#define MAGMA_ERR_FILESYSTEM       -109     // unused
#define MAGMA_ERR_UNEXPECTED       -110     // unused
#define MAGMA_ERR_SEQUENCE_FLUSHED -111     // unused
#define MAGMA_ERR_HOST_ALLOC       -112     ///< could not malloc CPU host memory
#define MAGMA_ERR_DEVICE_ALLOC     -113     ///< could not malloc GPU device memory
#define MAGMA_ERR_CUDASTREAM       -114     // unused
#define MAGMA_ERR_INVALID_PTR      -115     ///< can't free invalid pointer
#define MAGMA_ERR_UNKNOWN          -116     ///< unspecified error
#define MAGMA_ERR_NOT_IMPLEMENTED  -117     ///< not implemented yet
#define MAGMA_ERR_NAN              -118     ///< NaN (not-a-number) detected


#define MagmaUpperStr         "Upper"
#define MagmaLowerStr         "Lower"
#define MagmaFullStr          "Full"


typedef enum {
    MagmaUpper         = 121,
    MagmaLower         = 122,
    MagmaFull          = 123,  /* lascl, laset */
    MagmaHessenberg    = 124   /* lascl */
} magma_uplo_t;


typedef enum {
    MagmaLeft          = 141,
    MagmaRight         = 142,
    MagmaBothSides     = 143   /* trevc */
} magma_side_t;


typedef enum {
    MagmaNoTrans       = 111,
    MagmaTrans         = 112,
    MagmaConjTrans     = 113,
    Magma_ConjTrans    = MagmaConjTrans
} magma_trans_t;

typedef enum {
    MagmaNonUnit       = 131,
    MagmaUnit          = 132
} magma_diag_t;

typedef int magma_int_t;

typedef cl_mem magma_ptr;
typedef cl_mem magmaInt_ptr;
typedef cl_mem magmaIndex_ptr;
typedef cl_mem magmaFloat_ptr;
typedef cl_mem magmaDouble_ptr;
typedef cl_mem magmaFloatComplex_ptr;
typedef cl_mem magmaDoubleComplex_ptr;

typedef cl_mem magma_const_ptr;
typedef cl_mem magmaInt_const_ptr;
typedef cl_mem magmaIndex_const_ptr;
typedef cl_mem magmaFloat_const_ptr;
typedef cl_mem magmaDouble_const_ptr;
typedef cl_mem magmaFloatComplex_const_ptr;
typedef cl_mem magmaDoubleComplex_const_ptr;

typedef cl_command_queue  magma_queue_t;
typedef cl_event          magma_event_t;
typedef cl_device_id      magma_device_t;

// TODO: refactor code to avoid using the following global variable
// TODO: the code below seems to only write to g_event; it's never read, meaning it's not used for waiting
magma_event_t  *g_event;


/////////////////////
// CS: adaptions for OpenCL

inline
magma_int_t
magma_malloc_cpu( void** ptrPtr, size_t size )
{
    if ( size == 0 )  { size = 16; }
    
    *ptrPtr = malloc( size );
    if ( *ptrPtr == NULL ) {
        return MAGMA_ERR_HOST_ALLOC;
    }
    return MAGMA_SUCCESS;
}


//inline magma_int_t magma_dmalloc_pinned( double **ptr_ptr, size_t n ) { return magma_malloc_pinned( (void**) ptr_ptr, n*sizeof(double)             ); }
inline magma_int_t magma_dmalloc_pinned( double **ptr_ptr, size_t n ) { return magma_malloc_cpu( (void**) ptr_ptr, n*sizeof(double) ); }

inline magma_int_t magma_free_pinned( void* ptr )  { free( ptr ); return MAGMA_SUCCESS; }


inline
void
magma_getdevice(magma_device_t* dev)
  {
  (*dev) = coot_rt.get_device();
  }


// inline
// magma_int_t
// magma_getdevices(
//     magma_device_t* devices,
//     magma_int_t     size,
//     magma_int_t*    numPtr )
// {
//     // TODO just copy from g_runtime.get_devices()
//     cl_int err;
//     //err = clGetDeviceIDs( gPlatform, CL_DEVICE_TYPE_GPU, 1, size, devices, num );
//     size_t n;
//     err = clGetContextInfo(
//         g_runtime.get_context(), CL_CONTEXT_DEVICES,
//         size*sizeof(magma_device_t), devices, &n );
//     *numPtr = n / sizeof(magma_device_t);
//     check_error( err );
//     return err;
// }



inline
void
magma_queue_create(magma_device_t dev, magma_queue_t* queue)
  {
  (void)dev;  // ignore the variable
  
  coot_rt.create_extra_cq(*queue);
  }



inline
void
magma_queue_destroy(magma_queue_t queue)
  {
  coot_rt.delete_extra_cq(queue);
  }



inline
void
magma_dgetmatrix(magma_int_t m, magma_int_t n, magmaDouble_const_ptr dA_src, size_t dA_offset, magma_int_t ldda, double* hB_dst, magma_int_t ldhb, magma_queue_t queue)
  {
  if (m <= 0 || n <= 0)  { return; }

  size_t buffer_origin[3] = { dA_offset*sizeof(double), 0, 0 };
  size_t host_orig[3]     = { 0, 0, 0 };
  size_t region[3]        = { m*sizeof(double), n, 1 };
  
  cl_int err = clEnqueueReadBufferRect(
      queue, dA_src, CL_TRUE,  // blocking
      buffer_origin, host_orig, region,
      ldda*sizeof(double), 0,
      ldhb*sizeof(double), 0,
      hB_dst, 0, NULL, g_event );
  
  // TODO: g_event can probably be replaced with NULL; check if any other function required by magma_dpotrf_gpu accesses g_event
  // TODO: as the call is blocking, why use g_event in the first place?
  
  // OpenCL 1.2 docs for clEnqueueReadBufferRect()
  // event
  // Returns an event object that identifies this particular read command and can be used to query or queue a wait for this particular command to complete.
  // event can be NULL in which case it will not be possible for the application to query the status of this command or queue a wait for this command to complete.
  // If the event_wait_list and the event arguments are not NULL, the event argument should not refer to an element of the event_wait_list array.   
  
  check_error( err );  // TODO: replace check_error() with corresponding bandicoot function
  }



inline
void
magma_dsetmatrix(magma_int_t m, magma_int_t n, double const* hA_src, magma_int_t ldha, magmaDouble_ptr dB_dst, size_t dB_offset, magma_int_t lddb, magma_queue_t queue)
  {
  if (m <= 0 || n <= 0)  { return; }

  size_t buffer_origin[3] = { dB_offset*sizeof(double), 0, 0 };
  size_t host_orig[3]     = { 0, 0, 0 };
  size_t region[3]        = { m*sizeof(double), n, 1 };
  
  cl_int err = clEnqueueWriteBufferRect(
      queue, dB_dst, CL_TRUE,  // blocking
      buffer_origin, host_orig, region,
      lddb*sizeof(double), 0,
      ldha*sizeof(double), 0,
      hA_src, 0, NULL, g_event );
  
  // TODO: g_event can probably be replaced with NULL; see note above
  // TODO: as the call is blocking, why use g_event in the first place?
  
  // OpenCL 1.2 docs for clEnqueueWriteBufferRect()
  // event
  // 
  // Returns an event object that identifies this particular write command and can be used to query or queue a wait for this particular command to complete.
  // event can be NULL in which case it will not be possible for the application to query the status of this command or queue a wait for this command to complete.
  
  check_error( err );
  }


inline
void
magma_dgetmatrix_async(magma_int_t m, magma_int_t n, magmaDouble_const_ptr dA_src, size_t dA_offset, magma_int_t ldda, double* hB_dst, magma_int_t ldhb, magma_queue_t queue, magma_event_t *event)
  {
  if (m <= 0 || n <= 0)  { return; }

  size_t buffer_origin[3] = { dA_offset*sizeof(double), 0, 0 };
  size_t host_orig[3]     = { 0, 0, 0 };
  size_t region[3]        = { m*sizeof(double), n, 1 };
  
  cl_int err = clEnqueueReadBufferRect(
      queue, dA_src, CL_FALSE,  // non-blocking
      buffer_origin, host_orig, region,
      ldda*sizeof(double), 0,
      ldhb*sizeof(double), 0,
      hB_dst, 0, NULL, event );
  
  clFlush(queue);
  check_error( err );
  }


inline
void
magma_dgetmatrix_async(magma_int_t m, magma_int_t n, magmaDouble_const_ptr dA_src, size_t dA_offset, magma_int_t ldda, double* hB_dst, magma_int_t ldhb, magma_queue_t queue)
  {
  if (m <= 0 || n <= 0)  { return; }

  size_t buffer_origin[3] = { dA_offset*sizeof(double), 0, 0 };
  size_t host_orig[3]     = { 0, 0, 0 };
  size_t region[3]        = { m*sizeof(double), n, 1 };
  
  cl_int err = clEnqueueReadBufferRect(
      queue, dA_src, CL_FALSE,  // non-blocking
      buffer_origin, host_orig, region,
      ldda*sizeof(double), 0,
      ldhb*sizeof(double), 0,
      hB_dst, 0, NULL, NULL );
  
  clFlush(queue);
  check_error( err );
  }


inline
void
magma_dsetmatrix_async(magma_int_t m, magma_int_t n, double const* hA_src, magma_int_t ldha, magmaDouble_ptr dB_dst, size_t dB_offset, magma_int_t lddb, magma_queue_t queue, magma_event_t *event)
  {
  if (m <= 0 || n <= 0)  { return; }

  size_t buffer_origin[3] = { dB_offset*sizeof(double), 0, 0 };
  size_t host_orig[3]     = { 0, 0, 0 };
  size_t region[3]        = { m*sizeof(double), n, 1 };
  
  cl_int err = clEnqueueWriteBufferRect(
      queue, dB_dst, CL_FALSE,  // non-blocking
      buffer_origin, host_orig, region,
      lddb*sizeof(double), 0,
      ldha*sizeof(double), 0,
      hA_src, 0, NULL, event );
  
  clFlush(queue);
  check_error( err );
  }


inline
void
magma_dsetmatrix_async(magma_int_t m, magma_int_t n, double const* hA_src, magma_int_t ldha, magmaDouble_ptr dB_dst, size_t dB_offset, magma_int_t lddb, magma_queue_t queue)
  {
  if (m <= 0 || n <= 0)  { return; }

  size_t buffer_origin[3] = { dB_offset*sizeof(double), 0, 0 };
  size_t host_orig[3]     = { 0, 0, 0 };
  size_t region[3]        = { m*sizeof(double), n, 1 };
  
  cl_int err = clEnqueueWriteBufferRect(
      queue, dB_dst, CL_FALSE,  // non-blocking
      buffer_origin, host_orig, region,
      lddb*sizeof(double), 0,
      ldha*sizeof(double), 0,
      hA_src, 0, NULL, NULL );
  
  clFlush(queue);
  check_error( err );
  }


inline
magma_int_t
magma_queue_sync( magma_queue_t queue )
  {
  cl_int err = clFinish( queue );
  clFlush( queue );
  check_error( err );
  return err;
  }


///////////////////////////
// LAPACK interface related

// TODO: what a horror
const char *magma2lapack_constants[] =
{
    "No",                                    //  0: MagmaFalse
    "Yes",                                   //  1: MagmaTrue (zlatrs)
    "", "", "", "", "", "", "", "", "",      //  2-10
    "", "", "", "", "", "", "", "", "", "",  // 11-20
    "", "", "", "", "", "", "", "", "", "",  // 21-30
    "", "", "", "", "", "", "", "", "", "",  // 31-40
    "", "", "", "", "", "", "", "", "", "",  // 41-50
    "", "", "", "", "", "", "", "", "", "",  // 51-60
    "", "", "", "", "", "", "", "", "", "",  // 61-70
    "", "", "", "", "", "", "", "", "", "",  // 71-80
    "", "", "", "", "", "", "", "", "", "",  // 81-90
    "", "", "", "", "", "", "", "", "", "",  // 91-100
    "Row",                                   // 101: MagmaRowMajor
    "Column",                                // 102: MagmaColMajor
    "", "", "", "", "", "", "", "",          // 103-110
    "No transpose",                          // 111: MagmaNoTrans
    "Transpose",                             // 112: MagmaTrans
    "Conjugate transpose",                   // 113: MagmaConjTrans
    "", "", "", "", "", "", "",              // 114-120
    "Upper",                                 // 121: MagmaUpper
    "Lower",                                 // 122: MagmaLower
    "General",                               // 123: MagmaFull; see lascl for "G"
    "", "", "", "", "", "", "",              // 124-130
    "Non-unit",                              // 131: MagmaNonUnit
    "Unit",                                  // 132: MagmaUnit
    "", "", "", "", "", "", "", "",          // 133-140
    "Left",                                  // 141: MagmaLeft
    "Right",                                 // 142: MagmaRight
    "Both",                                  // 143: MagmaBothSides (dtrevc)
    "", "", "", "", "", "", "",              // 144-150
    "", "", "", "", "", "", "", "", "", "",  // 151-160
    "", "", "", "", "", "", "", "", "", "",  // 161-170
    "1 norm",                                // 171: MagmaOneNorm
    "",                                      // 172: MagmaRealOneNorm
    "2 norm",                                // 173: MagmaTwoNorm
    "Frobenius norm",                        // 174: MagmaFrobeniusNorm
    "Infinity norm",                         // 175: MagmaInfNorm
    "",                                      // 176: MagmaRealInfNorm
    "Maximum norm",                          // 177: MagmaMaxNorm
    "",                                      // 178: MagmaRealMaxNorm
    "", "",                                  // 179-180
    "", "", "", "", "", "", "", "", "", "",  // 181-190
    "", "", "", "", "", "", "", "", "", "",  // 191-200
    "Uniform",                               // 201: MagmaDistUniform
    "Symmetric",                             // 202: MagmaDistSymmetric
    "Normal",                                // 203: MagmaDistNormal
    "", "", "", "", "", "", "",              // 204-210
    "", "", "", "", "", "", "", "", "", "",  // 211-220
    "", "", "", "", "", "", "", "", "", "",  // 221-230
    "", "", "", "", "", "", "", "", "", "",  // 231-240
    "Hermitian",                             // 241 MagmaHermGeev
    "Positive ev Hermitian",                 // 242 MagmaHermPoev
    "NonSymmetric pos sv",                   // 243 MagmaNonsymPosv
    "Symmetric pos sv",                      // 244 MagmaSymPosv
    "", "", "", "", "", "",                  // 245-250
    "", "", "", "", "", "", "", "", "", "",  // 251-260
    "", "", "", "", "", "", "", "", "", "",  // 261-270
    "", "", "", "", "", "", "", "", "", "",  // 271-280
    "", "", "", "", "", "", "", "", "", "",  // 281-290
    "No Packing",                            // 291 MagmaNoPacking
    "U zero out subdiag",                    // 292 MagmaPackSubdiag
    "L zero out superdiag",                  // 293 MagmaPackSupdiag
    "C",                                     // 294 MagmaPackColumn
    "R",                                     // 295 MagmaPackRow
    "B",                                     // 296 MagmaPackLowerBand
    "Q",                                     // 297 MagmaPackUpeprBand
    "Z",                                     // 298 MagmaPackAll
    "", "",                                  // 299-300
    "No vectors",                            // 301 MagmaNoVec
    "Vectors needed",                        // 302 MagmaVec
    "I",                                     // 303 MagmaIVec
    "All",                                   // 304 MagmaAllVec
    "Some",                                  // 305 MagmaSomeVec
    "Overwrite",                             // 306 MagmaOverwriteVec
    "", "", "", "",                          // 307-310
    "All",                                   // 311 MagmaRangeAll
    "V",                                     // 312 MagmaRangeV
    "I",                                     // 313 MagmaRangeI
    "", "", "", "", "", "", "",              // 314-320
    "",                                      // 321
    "Q",                                     // 322
    "P",                                     // 323
    "", "", "", "", "", "", "",              // 324-330
    "", "", "", "", "", "", "", "", "", "",  // 331-340
    "", "", "", "", "", "", "", "", "", "",  // 341-350
    "", "", "", "", "", "", "", "", "", "",  // 351-360
    "", "", "", "", "", "", "", "", "", "",  // 361-370
    "", "", "", "", "", "", "", "", "", "",  // 371-380
    "", "", "", "", "", "", "", "", "", "",  // 381-390
    "Forward",                               // 391: MagmaForward
    "Backward",                              // 392: MagmaBackward
    "", "", "", "", "", "", "", "",          // 393-400
    "Columnwise",                            // 401: MagmaColumnwise
    "Rowwise",                               // 402: MagmaRowwise
    "", "", "", "", "", "", "", ""           // 403-410
    // Remember to add a comma!
};


inline
const char* lapack_trans_const( magma_trans_t magma_const )
{
    assert( magma_const >= MagmaNoTrans   );
    assert( magma_const <= MagmaConjTrans );
    return magma2lapack_constants[ magma_const ];
}

inline
const char* lapack_uplo_const ( magma_uplo_t magma_const )
{
    assert( magma_const >= MagmaUpper );
    assert( magma_const <= MagmaFull  );
    return magma2lapack_constants[ magma_const ];
}

inline
const char* lapack_diag_const ( magma_diag_t magma_const )
{
    assert( magma_const >= MagmaNonUnit );
    assert( magma_const <= MagmaUnit    );
    return magma2lapack_constants[ magma_const ];
}


inline
const char* lapack_side_const ( magma_side_t magma_const )
{
    assert( magma_const >= MagmaLeft  );
    assert( magma_const <= MagmaBothSides );
    return magma2lapack_constants[ magma_const ];
}


/////////////////////
// clBLAS wrappers


// TODO: what a horror
const int magma2amdblas_constants[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,                      // 100
    clblasRowMajor,         // 101: MagmaRowMajor
    clblasColumnMajor,      // 102: MagmaColMajor
    0, 0, 0, 0, 0, 0, 0, 0,
    clblasNoTrans,          // 111: MagmaNoTrans
    clblasTrans,            // 112: MagmaTrans
    clblasConjTrans,        // 113: MagmaConjTrans
    0, 0, 0, 0, 0, 0, 0,
    clblasUpper,            // 121: MagmaUpper
    clblasLower,            // 122: MagmaLower
    0, 0, 0, 0, 0, 0, 0, 0,
    clblasNonUnit,          // 131: MagmaNonUnit
    clblasUnit,             // 132: MagmaUnit
    0, 0, 0, 0, 0, 0, 0, 0,
    clblasLeft,             // 141: MagmaLeft
    clblasRight,            // 142: MagmaRight
    0, 0, 0, 0, 0, 0, 0, 0
};



inline
clblasUplo
clblas_uplo_const ( magma_uplo_t magma_const )
  {
  assert( magma_const >= MagmaUpper );
  assert( magma_const <= MagmaLower );
  
  return (clblasUplo)      magma2amdblas_constants[ magma_const ];
  }


inline
clblasTranspose
clblas_trans_const( magma_trans_t magma_const )
  {
  assert( magma_const >= MagmaNoTrans   );
  assert( magma_const <= MagmaConjTrans );
  
  return (clblasTranspose) magma2amdblas_constants[ magma_const ];
  }


inline
clblasSide
clblas_side_const ( magma_side_t magma_const )
  {
  assert( magma_const >= MagmaLeft  );
  assert( magma_const <= MagmaRight );
  
  return (clblasSide)      magma2amdblas_constants[ magma_const ];
  }

inline
clblasDiag
clblas_diag_const ( magma_diag_t magma_const )
  {
  assert( magma_const >= MagmaNonUnit );
  assert( magma_const <= MagmaUnit    );
  return (clblasDiag)      magma2amdblas_constants[ magma_const ];
  }


inline
void
magma_dgemm
  (
  magma_trans_t transA, magma_trans_t transB,
  magma_int_t m, magma_int_t n, magma_int_t k,
  double alpha,
  magmaDouble_const_ptr dA, size_t dA_offset, magma_int_t ldda,
  magmaDouble_const_ptr dB, size_t dB_offset, magma_int_t lddb,
  double beta,
  magmaDouble_ptr       dC, size_t dC_offset, magma_int_t lddc,
  magma_queue_t queue
  )
  {
  if ( m <= 0 || n <= 0 || k <= 0 )  { return; }

  cl_int err = clblasDgemm(
      clblasColumnMajor,
      clblas_trans_const( transA ),
      clblas_trans_const( transB ),
      m, n, k,
      alpha, dA, dA_offset, ldda,
             dB, dB_offset, lddb,
      beta,  dC, dC_offset, lddc,
      1, &queue, 0, NULL, g_event );
  
  clFlush(queue);
  
  check_error( err );
  }


inline
void
magma_dsyrk
  (
  magma_uplo_t uplo, magma_trans_t trans,
  magma_int_t n, magma_int_t k,
  double alpha,
  magmaDouble_const_ptr dA, size_t dA_offset, magma_int_t ldda,
  double beta,
  magmaDouble_ptr       dC, size_t dC_offset, magma_int_t lddc,
  magma_queue_t queue
  )
  {
  cl_int err = clblasDsyrk(
    clblasColumnMajor,
    clblas_uplo_const( uplo ),
    clblas_trans_const( trans ),
    n, k,
    alpha,
    dA, dA_offset, ldda,
    beta,
    dC, dC_offset, lddc,
    1, &queue, 0, NULL, g_event );
  
  coot_check_clblas_error(err, "magma_dsyrk()");
  }


inline
void
magma_dtrsm
  (
  magma_side_t side, magma_uplo_t uplo, magma_trans_t trans, magma_diag_t diag,
  magma_int_t m, magma_int_t n,
  double alpha,
  magmaDouble_const_ptr dA, size_t dA_offset, magma_int_t ldda,
  magmaDouble_ptr       dB, size_t dB_offset, magma_int_t lddb,
  magma_queue_t queue
  )
  {
  if (m <= 0 || n <= 0)  { return; }

  cl_int err = clblasDtrsm(
      clblasColumnMajor,
      clblas_side_const( side ),
      clblas_uplo_const( uplo ),
      clblas_trans_const( trans ),
      clblas_diag_const( diag ),
      m, n,
      alpha, dA, dA_offset, ldda,
             dB, dB_offset, lddb,
      1, &queue, 0, NULL, g_event );
  
  clFlush(queue);
  
  check_error( err );
  }



////////////////////////////////////////


#define MAGMA_D_ZERO              ( 0.0)
#define MAGMA_D_ONE               ( 1.0)
#define MAGMA_D_HALF              ( 0.5)
#define MAGMA_D_NEG_ONE           (-1.0)
#define MAGMA_D_NEG_HALF          (-0.5)

