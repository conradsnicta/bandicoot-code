// work in progress - adapting code

inline
void
check_error(const cl_int error_code)
  {
  coot_check_cl_error(error_code, "magma_function");
  }



// ----------------------------------------
// Return codes
// LAPACK argument errors are < 0 but > MAGMA_ERR.
// MAGMA errors are < MAGMA_ERR.
#define MAGMA_SUCCESS               0
#define MAGMA_ERR                  -100
#define MAGMA_ERR_NOT_INITIALIZED  -101
#define MAGMA_ERR_REINITIALIZED    -102
#define MAGMA_ERR_NOT_SUPPORTED    -103
#define MAGMA_ERR_ILLEGAL_VALUE    -104
#define MAGMA_ERR_NOT_FOUND        -105
#define MAGMA_ERR_ALLOCATION       -106
#define MAGMA_ERR_INTERNAL_LIMIT   -107
#define MAGMA_ERR_UNALLOCATED      -108
#define MAGMA_ERR_FILESYSTEM       -109
#define MAGMA_ERR_UNEXPECTED       -110
#define MAGMA_ERR_SEQUENCE_FLUSHED -111
#define MAGMA_ERR_HOST_ALLOC       -112
#define MAGMA_ERR_DEVICE_ALLOC     -113
#define MAGMA_ERR_CUDASTREAM       -114
#define MAGMA_ERR_INVALID_PTR      -115
#define MAGMA_ERR_UNKNOWN          -116
#define MAGMA_ERR_NOT_IMPLEMENTED  -117

#undef  MagmaUpperStr
#undef  MagmaLowerStr
#define MagmaUpperStr         "Upper"
#define MagmaLowerStr         "Lower"

typedef enum {
    MagmaUpper         = 121,
    MagmaLower         = 122,
    MagmaUpperLower    = 123,
    MagmaFull          = 123   /* lascl, laset */
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


typedef int               magma_int_t;
typedef cl_mem            magmaDouble_ptr;
typedef cl_mem            magmaDouble_const_ptr;
typedef cl_command_queue  magma_queue_t;
typedef cl_event          magma_event_t;


// TODO: refactor code to avoid using the following global variable
// TODO: the code below seems to only write to g_event; it's never read, meaning it's not used for waiting
magma_event_t  *g_event;


inline
magma_int_t
magma_get_dpotrf_nb( magma_int_t m )
  {
  if      (m <= 4256) return 128;
  else                return 256;
  }


inline
magma_int_t
magma_malloc_cpu( void** ptrPtr, size_t size )
  {
  // malloc and free sometimes don't work for size=0, so allocate some minimal size
  if ( size == 0 )  { size = 16; }
    
  *ptrPtr = malloc( size );
  
  if(*ptrPtr == NULL) { return MAGMA_ERR_HOST_ALLOC; }
  
  return MAGMA_SUCCESS;
  }



inline
magma_int_t
magma_dmalloc_cpu(double **ptrPtr, size_t n )
  {
  return magma_malloc_cpu( (void**) ptrPtr, n*sizeof(double) );
  }


inline
magma_int_t
magma_free_cpu( void* ptr )
  {
  free( ptr );
  return MAGMA_SUCCESS;
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
magma_int_t
magma_event_sync( magma_event_t event )
  {
  // blocks CPU until event occurs
  cl_int err = clWaitForEvents(1, &event);
  check_error( err );
  return err;
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
    "GFull",                                 // 123: MagmaFull; see lascl for "G"
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
const char*
lapack_uplo_const ( magma_uplo_t magma_const )
  {
  assert( magma_const >= MagmaUpper );
  assert( magma_const <= MagmaFull  );
  
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
  
  check_error( err );
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


/////////////////////




#define MAGMA_D_MAKE(r,i) (r)

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



//! \addtogroup fn_chol
//! @{


// user facing function

// TODO: add optional 'layout' argument
template<typename T1>
coot_warn_unused
inline
bool
chol(Mat<typename T1::elem_type>& out, const Base<typename T1::elem_type, T1>& X)
  {
  coot_extra_debug_sigprint();
  
    // typedef typename T1::elem_type eT;
  
  coot_debug_check( (coot_rt.is_valid() == false), "coot_rt not valid" );
  
  out = X.get_ref();
  
  // TODO: check whether given matrix is square
  
  magma_int_t info   = 0;
  magma_int_t status = 0;
  
  status = magma_dpotrf_gpu(MagmaUpper, out.n_rows, out.get_dev_mem(), 0, out.n_rows, coot_rt.get_cq(), &info);
  
  // TODO: check status
  
  // TODO: need to set the lower/upper triangular part (excluding the diagonal) to zero
  
  return true;
  }



//! @}
