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


//! \addtogroup debug
//! @{



template<typename T>
inline
std::ostream&
coot_stream_err1(std::ostream* user_stream)
  {
  static std::ostream* stream_err1 = &(COOT_DEFAULT_OSTREAM);
  
  if(user_stream != NULL)  { stream_err1 = user_stream; }
  
  return *stream_err1;
  }



template<typename T>
inline
std::ostream&
coot_stream_err2(std::ostream* user_stream)
  {
  static std::ostream* stream_err2 = &(COOT_DEFAULT_OSTREAM);
  
  if(user_stream != NULL)  { stream_err2 = user_stream; }
  
  return *stream_err2;
  }



inline
void
set_stream_err1(std::ostream& user_stream)
  {
  coot_stream_err1<char>(&user_stream);
  }



inline
void
set_stream_err2(std::ostream& user_stream)
  {
  coot_stream_err2<char>(&user_stream);
  }



inline
std::ostream&
get_stream_err1()
  {
  return coot_stream_err1<char>(NULL);
  }



inline
std::ostream&
get_stream_err2()
  {
  return coot_stream_err2<char>(NULL);
  }



//! print a message to get_stream_err1() and throw logic_error exception
template<typename T1>
coot_cold
coot_noinline
static
void
coot_stop_logic_error(const T1& x)
  {
  #if defined(COOT_PRINT_ERRORS)
    {
    get_stream_err1() << "\nerror: " << x << std::endl;
    }
  #endif
  
  throw std::logic_error( std::string(x) );
  }



//! print a message to get_stream_err2() and throw bad_alloc exception
template<typename T1>
coot_cold
coot_noinline
static
void
coot_stop_bad_alloc(const T1& x)
  {
  #if defined(COOT_PRINT_ERRORS)
    {
    get_stream_err2() << "\nerror: " << x << std::endl;
    }
  #else
    {
    coot_ignore(x);
    }
  #endif
  
  throw std::bad_alloc();
  }



//! print a message to get_stream_err2() and throw runtime_error exception
template<typename T1>
coot_cold
coot_noinline
static
void
coot_stop_runtime_error(const T1& x)
  {
  #if defined(COOT_PRINT_ERRORS)
    {
    get_stream_err2() << "\nerror: " << x << std::endl;
    }
  #endif
  
  throw std::runtime_error( std::string(x) );
  }



//! print a message to get_stream_err2() and throw runtime_error exception
template<typename T1, typename T2>
coot_cold
coot_noinline
static
void
coot_stop_runtime_error(const T1& x, const T2& y)
  {
  #if defined(COOT_PRINT_ERRORS)
    {
    get_stream_err2() << "\nerror: " << x << ": " << y << std::endl;
    }
  #endif
  
  throw std::runtime_error( std::string(x) + std::string(": ") + std::string(y) );
  }



//
// coot_print


coot_cold
inline
void
coot_print()
  {
  get_stream_err1() << std::endl;
  }


template<typename T1>
coot_cold
coot_noinline
static
void
coot_print(const T1& x)
  {
  get_stream_err1() << x << std::endl;
  }



template<typename T1, typename T2>
coot_cold
coot_noinline
static
void
coot_print(const T1& x, const T2& y)
  {
  get_stream_err1() << x << y << std::endl;
  }



template<typename T1, typename T2, typename T3>
coot_cold
coot_noinline
static
void
coot_print(const T1& x, const T2& y, const T3& z)
  {
  get_stream_err1() << x << y << z << std::endl;
  }






//
// coot_sigprint

//! print a message the the log stream with a preceding @ character.
//! by default the log stream is cout.
//! used for printing the signature of a function
//! (see the coot_extra_debug_sigprint macro) 
inline
void
coot_sigprint(const char* x)
  {
  get_stream_err1() << "@ " << x;
  }



//
// coot_bktprint


inline
void
coot_bktprint()
  {
  get_stream_err1() << std::endl;
  }


template<typename T1>
inline
void
coot_bktprint(const T1& x)
  {
  get_stream_err1() << " [" << x << ']' << std::endl;
  }



template<typename T1, typename T2>
inline
void
coot_bktprint(const T1& x, const T2& y)
  {
  get_stream_err1() << " [" << x << y << ']' << std::endl;
  }






//
// coot_thisprint

inline
void
coot_thisprint(const void* this_ptr)
  {
  get_stream_err1() << " [this = " << this_ptr << ']' << std::endl;
  }



//
// coot_warn


//! print a message to the warn stream
template<typename T1>
coot_cold
coot_noinline
static
void
coot_warn(const T1& x)
  {
  #if defined(COOT_PRINT_ERRORS)
    {
    get_stream_err2() << "\nwarning: " << x << '\n';
    }
  #else
    {
    coot_ignore(x);
    }
  #endif
  }


template<typename T1, typename T2>
coot_cold
coot_noinline
static
void
coot_warn(const T1& x, const T2& y)
  {
  #if defined(COOT_PRINT_ERRORS)
    {
    get_stream_err2() << "\nwarning: " << x << y << '\n';
    }
  #else
    {
    coot_ignore(x);
    coot_ignore(y);
    }
  #endif
  }


template<typename T1, typename T2, typename T3>
coot_cold
coot_noinline
static
void
coot_warn(const T1& x, const T2& y, const T3& z)
  {
  #if defined(COOT_PRINT_ERRORS)
    {
    get_stream_err2() << "\nwarning: " << x << y << z << '\n';
    }
  #else
    {
    coot_ignore(x);
    coot_ignore(y);
    coot_ignore(z);
    }
  #endif
  }



//
// coot_check

//! if state is true, abort program
template<typename T1>
coot_hot
inline
void
coot_check(const bool state, const T1& x)
  {
  if(state)  { coot_stop_logic_error(coot_str::str_wrapper(x)); }
  }


template<typename T1, typename T2>
coot_hot
inline
void
coot_check(const bool state, const T1& x, const T2& y)
  {
  if(state)  { coot_stop_logic_error( std::string(x) + std::string(y) ); }
  }


template<typename T1>
coot_hot
inline
void
coot_check_bad_alloc(const bool state, const T1& x)
  {
  if(state)  { coot_stop_bad_alloc(x); }
  }


template<typename T1>
coot_hot
inline
void
coot_check_runtime_error(const bool state, const T1& x)
  {
  if(state)  { coot_stop_runtime_error(x); }
  }



template<typename T1>
coot_hot
inline
void
coot_check_cl_error(const cl_int error_code, const T1& x)
  {
  if(error_code != CL_SUCCESS)
    {
    coot_stop_runtime_error( x, coot_clerror::as_string(error_code) );
    }
  }



//
// functions for generating strings indicating size errors

coot_cold
coot_noinline
static
std::string
coot_incompat_size_string(const uword A_n_rows, const uword A_n_cols, const uword B_n_rows, const uword B_n_cols, const char* x)
  {
  std::stringstream tmp;
  
  tmp << x << ": incompatible matrix dimensions: " << A_n_rows << 'x' << A_n_cols << " and " << B_n_rows << 'x' << B_n_cols;
  
  return tmp.str();
  }



//
// functions for checking whether two dense matrices have the same dimensions



coot_inline
coot_hot
void
coot_assert_same_size(const uword A_n_rows, const uword A_n_cols, const uword B_n_rows, const uword B_n_cols, const char* x)
  {
  if( (A_n_rows != B_n_rows) || (A_n_cols != B_n_cols) )
    {
    coot_stop_logic_error( coot_incompat_size_string(A_n_rows, A_n_cols, B_n_rows, B_n_cols, x) );
    }
  }



//! stop if given matrices have different sizes
template<typename eT1, typename eT2>
coot_hot
inline
void
coot_assert_same_size(const Mat<eT1>& A, const Mat<eT2>& B, const char* x)
  {
  const uword A_n_rows = A.n_rows;
  const uword A_n_cols = A.n_cols;
  
  const uword B_n_rows = B.n_rows;
  const uword B_n_cols = B.n_cols;
  
  if( (A_n_rows != B_n_rows) || (A_n_cols != B_n_cols) )
    {
    coot_stop_logic_error( coot_incompat_size_string(A_n_rows, A_n_cols, B_n_rows, B_n_cols, x) );
    }
  }



//
// functions for checking whether two matrices have dimensions that are compatible with the matrix multiply operation



coot_hot
inline
void
coot_assert_mul_size(const uword A_n_rows, const uword A_n_cols, const uword B_n_rows, const uword B_n_cols, const char* x)
  {
  if(A_n_cols != B_n_rows)
    {
    coot_stop_logic_error( coot_incompat_size_string(A_n_rows, A_n_cols, B_n_rows, B_n_cols, x) );
    }
  }



//! stop if given matrices are incompatible for multiplication
template<typename eT1, typename eT2>
coot_hot
inline
void
coot_assert_mul_size(const Mat<eT1>& A, const Mat<eT2>& B, const char* x)
  {
  const uword A_n_cols = A.n_cols;
  const uword B_n_rows = B.n_rows;
  
  if(A_n_cols != B_n_rows)
    {
    coot_stop_logic_error( coot_incompat_size_string(A.n_rows, A_n_cols, B_n_rows, B.n_cols, x) );
    }
  }



//! stop if given matrices are incompatible for multiplication
template<typename eT1, typename eT2>
coot_hot
inline
void
coot_assert_mul_size(const Mat<eT1>& A, const Mat<eT2>& B, const bool do_trans_A, const bool do_trans_B, const char* x)
  {
  const uword final_A_n_cols = (do_trans_A == false) ? A.n_cols : A.n_rows;
  const uword final_B_n_rows = (do_trans_B == false) ? B.n_rows : B.n_cols;
    
  if(final_A_n_cols != final_B_n_rows)
    {
    const uword final_A_n_rows = (do_trans_A == false) ? A.n_rows : A.n_cols;
    const uword final_B_n_cols = (do_trans_B == false) ? B.n_cols : B.n_rows;
    
    coot_stop_logic_error( coot_incompat_size_string(final_A_n_rows, final_A_n_cols, final_B_n_rows, final_B_n_cols, x) );
    }
  }



template<const bool do_trans_A, const bool do_trans_B>
coot_hot
inline
void
coot_assert_trans_mul_size(const uword A_n_rows, const uword A_n_cols, const uword B_n_rows, const uword B_n_cols, const char* x)
  {
  const uword final_A_n_cols = (do_trans_A == false) ? A_n_cols : A_n_rows;
  const uword final_B_n_rows = (do_trans_B == false) ? B_n_rows : B_n_cols;
    
  if(final_A_n_cols != final_B_n_rows)
    {
    const uword final_A_n_rows = (do_trans_A == false) ? A_n_rows : A_n_cols;
    const uword final_B_n_cols = (do_trans_B == false) ? B_n_cols : B_n_rows;
    
    coot_stop_logic_error( coot_incompat_size_string(final_A_n_rows, final_A_n_cols, final_B_n_rows, final_B_n_cols, x) );
    }
  }



template<typename T1>
coot_hot
inline
void
coot_assert_blas_size(const T1& A)
  {
  if(sizeof(uword) >= sizeof(blas_int))
    {
    bool overflow;
    
    overflow = (A.n_rows > COOT_MAX_BLAS_INT);
    overflow = (A.n_cols > COOT_MAX_BLAS_INT) || overflow;
    
    if(overflow)
      {
      coot_stop_runtime_error("integer overflow: matrix dimensions are too large for integer type used by BLAS and LAPACK");
      }
    }
  }



template<typename T1, typename T2>
coot_hot
inline
void
coot_assert_blas_size(const T1& A, const T2& B)
  {
  if(sizeof(uword) >= sizeof(blas_int))
    {
    bool overflow;
    
    overflow = (A.n_rows > COOT_MAX_BLAS_INT);
    overflow = (A.n_cols > COOT_MAX_BLAS_INT) || overflow;
    overflow = (B.n_rows > COOT_MAX_BLAS_INT) || overflow;
    overflow = (B.n_cols > COOT_MAX_BLAS_INT) || overflow;
    
    if(overflow)
      {
      coot_stop_runtime_error("integer overflow: matrix dimensions are too large for integer type used by BLAS and LAPACK");
      }
    }
  }



//
// macros


// #define COOT_STRING1(x) #x
// #define COOT_STRING2(x) COOT_STRING1(x)
// #define COOT_FILELINE  __FILE__ ": " COOT_STRING2(__LINE__)


#if defined(COOT_NO_DEBUG)
  
  #undef COOT_EXTRA_DEBUG
  
  #define coot_debug_print                   true ? (void)0 : coot_print
  #define coot_debug_warn                    true ? (void)0 : coot_warn
  #define coot_debug_check                   true ? (void)0 : coot_check
  #define coot_debug_assert_same_size        true ? (void)0 : coot_assert_same_size
  #define coot_debug_assert_mul_size         true ? (void)0 : coot_assert_mul_size
  #define coot_debug_assert_trans_mul_size   true ? (void)0 : coot_assert_trans_mul_size
  #define coot_debug_assert_blas_size        true ? (void)0 : coot_assert_blas_size
  
#else
  
  #define coot_debug_print                 coot_print
  #define coot_debug_warn                  coot_warn
  #define coot_debug_check                 coot_check
  #define coot_debug_assert_same_size      coot_assert_same_size
  #define coot_debug_assert_mul_size       coot_assert_mul_size
  #define coot_debug_assert_trans_mul_size coot_assert_trans_mul_size
  #define coot_debug_assert_blas_size      coot_assert_blas_size
  
#endif



#if defined(COOT_EXTRA_DEBUG)
  
  #define coot_extra_debug_sigprint       coot_sigprint(COOT_FNSIG); coot_bktprint
  #define coot_extra_debug_sigprint_this  coot_sigprint(COOT_FNSIG); coot_thisprint
  #define coot_extra_debug_print          coot_print
  #define coot_extra_debug_warn           coot_warn
  #define coot_extra_debug_check          coot_check

#else
  
  #define coot_extra_debug_sigprint        true ? (void)0 : coot_bktprint
  #define coot_extra_debug_sigprint_this   true ? (void)0 : coot_thisprint
  #define coot_extra_debug_print           true ? (void)0 : coot_print
  #define coot_extra_debug_warn            true ? (void)0 : coot_warn
  #define coot_extra_debug_check           true ? (void)0 : coot_check
 
#endif




#if defined(COOT_EXTRA_DEBUG)

  namespace junk
    {
    class coot_first_extra_debug_message
      {
      public:
      
      inline
      coot_first_extra_debug_message()
        {
        union
          {
          unsigned short a;
          unsigned char  b[sizeof(unsigned short)];
          } endian_test;
        
        endian_test.a = 1;
        
        const bool        little_endian = (endian_test.b[0] == 1);
        const std::string note          = COOT_VERSION_NOTE;
        
        std::ostream& out = get_stream_err1();
        
        out << "@ ---" << '\n';
        out << "@ Bandicoot " << coot_version::major << '.' << coot_version::minor << '.' << coot_version::patch;
        if(note.length() > 0)  { out << " (" << note << ')'; }
        out << '\n';
        out << "@ coot_config::use_wrapper  = " << coot_config::use_wrapper  << '\n';
        out << "@ coot_config::use_cxx11    = " << coot_config::use_cxx11    << '\n';
        out << "@ coot_config::lapack       = " << coot_config::lapack       << '\n';
        out << "@ coot_config::blas         = " << coot_config::blas         << '\n';
        out << "@ coot_config::extra_code   = " << coot_config::extra_code   << '\n';
        out << "@ sizeof(void*)    = " << sizeof(void*)    << '\n';
        out << "@ sizeof(int)      = " << sizeof(int)      << '\n';
        out << "@ sizeof(long)     = " << sizeof(long)     << '\n';
        out << "@ sizeof(uword)    = " << sizeof(uword)    << '\n';
        out << "@ sizeof(blas_int) = " << sizeof(blas_int) << '\n';
        out << "@ little_endian    = " << little_endian    << '\n';
        out << "@ ---" << std::endl;
        }
      
      };
    
    static coot_first_extra_debug_message coot_first_extra_debug_message_run;
    }

#endif



//! @}
