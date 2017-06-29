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



#undef coot_hot
#undef coot_cold
#undef coot_aligned
#undef coot_align_mem
#undef coot_warn_unused
#undef coot_deprecated
#undef coot_malloc
#undef coot_inline
#undef coot_noinline
#undef coot_ignore

#define coot_hot
#define coot_cold
#define coot_aligned
#define coot_align_mem
#define coot_warn_unused
#define coot_deprecated
#define coot_malloc
#define coot_inline            inline
#define coot_noinline
#define coot_ignore(variable)  ((void)(variable))

#undef coot_fortran_noprefix
#undef coot_fortran_prefix

#undef coot_fortran2_noprefix
#undef coot_fortran2_prefix
 
#if defined(COOT_BLAS_UNDERSCORE)
  #define coot_fortran2_noprefix(function) function##_
  #define coot_fortran2_prefix(function)   wrapper_##function##_
#else
  #define coot_fortran2_noprefix(function) function
  #define coot_fortran2_prefix(function)   wrapper_##function
#endif

// TODO: do we still want to go through a wrapper library?

#if defined(COOT_USE_WRAPPER)
  #define coot_fortran(function) coot_fortran2_prefix(function)
  #define coot_wrapper(function) wrapper_##function
#else
  #define coot_fortran(function) coot_fortran2_noprefix(function)
  #define coot_wrapper(function) function
#endif

#define coot_fortran_prefix(function)   coot_fortran2_prefix(function)
#define coot_fortran_noprefix(function) coot_fortran2_noprefix(function)

#undef  COOT_INCFILE_WRAP
#define COOT_INCFILE_WRAP(x) <x>


#undef COOT_GOOD_COMPILER

#undef COOT_HAVE_GETTIMEOFDAY
#undef COOT_HAVE_SNPRINTF
#undef COOT_HAVE_ISFINITE
#undef COOT_HAVE_LOG1P
#undef COOT_HAVE_ISINF
#undef COOT_HAVE_ISNAN


#if (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L))
  #define COOT_HAVE_GETTIMEOFDAY
#endif


#if ( defined(_POSIX_ADVISORY_INFO) && (_POSIX_ADVISORY_INFO >= 200112L) )
  #undef  COOT_HAVE_POSIX_MEMALIGN
  #define COOT_HAVE_POSIX_MEMALIGN
#endif


#if defined(__APPLE__) || defined(__apple_build_version__)
  #undef  COOT_BLAS_SDOT_BUG
  #define COOT_BLAS_SDOT_BUG
  
  #undef  COOT_HAVE_POSIX_MEMALIGN
  #undef  COOT_USE_EXTERN_CXX11_RNG
  // TODO: thread local storage (TLS) (eg. "extern thread_local") appears currently broken on Mac OS X
#endif


#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER)
  #undef COOT_HAVE_POSIX_MEMALIGN
#endif


#undef COOT_FNSIG

#if defined (__GNUG__)
  #define COOT_FNSIG  __PRETTY_FUNCTION__
#elif defined (_MSC_VER)
  #define COOT_FNSIG  __FUNCSIG__ 
#elif defined(__INTEL_COMPILER)
  #define COOT_FNSIG  __FUNCTION__
#elif defined(COOT_USE_CXX11)
  #define COOT_FNSIG  __func__
#else 
  #define COOT_FNSIG  "(unknown)"
#endif


#if (defined(__GNUG__) || defined(__GNUC__)) && (defined(__clang__) || defined(__INTEL_COMPILER) || defined(__NVCC__) || defined(__CUDACC__) || defined(__PGI) || defined(__PATHSCALE__) || defined(__ARMCC_VERSION) || defined(__IBMCPP__))
  #undef  COOT_FAKE_GCC
  #define COOT_FAKE_GCC
#endif


#if defined(__GNUG__) && !defined(COOT_FAKE_GCC)
  
  #undef  COOT_GCC_VERSION
  #define COOT_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
  
  #if (COOT_GCC_VERSION < 40400)
    #error "*** Need a newer compiler ***"
  #endif
  
  #if (COOT_GCC_VERSION < 40600)
    #undef  COOT_PRINT_CXX98_WARNING
    #define COOT_PRINT_CXX98_WARNING
  #endif
  
  #if ( (COOT_GCC_VERSION >= 40700) && (COOT_GCC_VERSION <= 40701) )
    #error "gcc versions 4.7.0 and 4.7.1 are unsupported; use 4.7.2 or later"
    // due to http://gcc.gnu.org/bugzilla/show_bug.cgi?id=53549
  #endif
  
  #define COOT_GOOD_COMPILER
  
  #undef  coot_hot
  #undef  coot_cold
  #undef  coot_aligned
  #undef  coot_align_mem
  #undef  coot_warn_unused
  #undef  coot_deprecated
  #undef  coot_malloc
  #undef  coot_inline
  #undef  coot_noinline
  
  #define coot_hot                __attribute__((__hot__))
  #define coot_cold               __attribute__((__cold__))
  #define coot_aligned            __attribute__((__aligned__))
  #define coot_align_mem          __attribute__((__aligned__(16)))
  #define coot_warn_unused        __attribute__((__warn_unused_result__))
  #define coot_deprecated         __attribute__((__deprecated__))
  #define coot_malloc             __attribute__((__malloc__))
  #define coot_inline      inline __attribute__((__always_inline__))
  #define coot_noinline           __attribute__((__noinline__))
  
  #undef  COOT_HAVE_ALIGNED_ATTRIBUTE
  #define COOT_HAVE_ALIGNED_ATTRIBUTE
  
  #if defined(COOT_USE_CXX11)
    #if (COOT_GCC_VERSION < 40800)
      #undef  COOT_PRINT_CXX11_WARNING
      #define COOT_PRINT_CXX11_WARNING
    #endif
  #endif
  
  #if !defined(COOT_USE_CXX11) && (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L))
    #define COOT_HAVE_SNPRINTF
    #define COOT_HAVE_ISFINITE
    #define COOT_HAVE_LOG1P
    #define COOT_HAVE_ISINF
    #define COOT_HAVE_ISNAN
  #endif
  
  #undef COOT_GCC_VERSION
  
#endif


#if defined(__clang__) && (defined(__INTEL_COMPILER) || defined(__NVCC__) || defined(__CUDACC__) || defined(__PGI) || defined(__PATHSCALE__) || defined(__ARMCC_VERSION) || defined(__IBMCPP__))
  #undef  COOT_FAKE_CLANG
  #define COOT_FAKE_CLANG
#endif


#if defined(__clang__) && !defined(COOT_FAKE_CLANG)
  
  #define COOT_GOOD_COMPILER
  
  #if !defined(__has_attribute)
    #define __has_attribute(x) 0
  #endif
  
  #if __has_attribute(__aligned__)
    #undef  coot_aligned
    #undef  coot_align_mem
    
    #define coot_aligned   __attribute__((__aligned__))
    #define coot_align_mem __attribute__((__aligned__(16)))
    
    #undef  COOT_HAVE_ALIGNED_ATTRIBUTE
    #define COOT_HAVE_ALIGNED_ATTRIBUTE
  #endif
  
  #if __has_attribute(__warn_unused_result__)
    #undef  coot_warn_unused
    #define coot_warn_unused __attribute__((__warn_unused_result__))
  #endif
  
  #if __has_attribute(__deprecated__)
    #undef  coot_deprecated
    #define coot_deprecated __attribute__((__deprecated__))
  #endif
  
  #if __has_attribute(__malloc__)
    #undef  coot_malloc
    #define coot_malloc __attribute__((__malloc__))
  #endif
  
  #if __has_attribute(__always_inline__)
    #undef  coot_inline
    #define coot_inline inline __attribute__((__always_inline__))
  #endif
  
  #if __has_attribute(__noinline__)
    #undef  coot_noinline
    #define coot_noinline __attribute__((__noinline__))
  #endif
  
  #if __has_attribute(__hot__)
    #undef  coot_hot
    #define coot_hot __attribute__((__hot__))
  #endif
  
  #if __has_attribute(__cold__)
    #undef  coot_cold
    #define coot_cold __attribute__((__cold__))
  #endif
  
  // #pragma clang diagnostic push
  // #pragma clang diagnostic ignored "-Wignored-attributes"
  
  #if !defined(COOT_USE_CXX11) && (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L))
    #define COOT_HAVE_SNPRINTF
    #define COOT_HAVE_ISFINITE
    #define COOT_HAVE_LOG1P
    #define COOT_HAVE_ISINF
    #define COOT_HAVE_ISNAN
  #endif
  
#endif


#if defined(__INTEL_COMPILER)
  
  #if (__INTEL_COMPILER == 9999)
    #error "*** Need a newer compiler ***"
  #endif
  
  #if (__INTEL_COMPILER < 1300)
    #error "*** Need a newer compiler ***"
  #endif
  
  #if defined(COOT_USE_CXX11)
    #if (__INTEL_COMPILER < 1500)
      #undef  COOT_PRINT_CXX11_WARNING
      #define COOT_PRINT_CXX11_WARNING
    #endif
  #endif
  
#endif


#if defined(_MSC_VER)
  
  #if (_MSC_VER < 1700)
    #error "*** Need a newer compiler ***"
  #endif
  
  #if (_MSC_VER < 1800)
    #undef  COOT_PRINT_CXX98_WARNING
    #define COOT_PRINT_CXX98_WARNING
  #endif
  
  #if defined(COOT_USE_CXX11)
    #if (_MSC_VER < 1900)
      #undef  COOT_PRINT_CXX11_WARNING
      #define COOT_PRINT_CXX11_WARNING
    #endif
  #endif
  
  #undef  coot_deprecated
  #define coot_deprecated __declspec(deprecated)
  // #undef  coot_inline
  // #define coot_inline inline __forceinline
  
  #pragma warning(push)
  
  #pragma warning(disable: 4127)  // conditional expression is constant
  #pragma warning(disable: 4180)  // qualifier has no meaning
  #pragma warning(disable: 4244)  // possible loss of data when converting types
  #pragma warning(disable: 4510)  // default constructor could not be generated
  #pragma warning(disable: 4511)  // copy constructor can't be generated
  #pragma warning(disable: 4512)  // assignment operator can't be generated
  #pragma warning(disable: 4513)  // destructor can't be generated
  #pragma warning(disable: 4514)  // unreferenced inline function has been removed
  #pragma warning(disable: 4522)  // multiple assignment operators specified
  #pragma warning(disable: 4623)  // default constructor can't be generated
  #pragma warning(disable: 4624)  // destructor can't be generated
  #pragma warning(disable: 4625)  // copy constructor can't be generated
  #pragma warning(disable: 4626)  // assignment operator can't be generated
  #pragma warning(disable: 4702)  // unreachable code
  #pragma warning(disable: 4710)  // function not inlined
  #pragma warning(disable: 4711)  // call was inlined
  #pragma warning(disable: 4714)  // __forceinline can't be inlined
  #pragma warning(disable: 4800)  // value forced to bool
  
  // #if (_MANAGED == 1) || (_M_CEE == 1)
  //   
  //   // don't do any alignment when compiling in "managed code" mode 
  //   
  //   #undef  coot_aligned
  //   #define coot_aligned
  //   
  //   #undef  coot_align_mem
  //   #define coot_align_mem
  //  
  // #elif (_MSC_VER >= 1700)
  //   
  //   #undef  coot_align_mem
  //   #define coot_align_mem __declspec(align(16))
  //   
  //   #define COOT_HAVE_ALIGNED_ATTRIBUTE
  //   
  //   // disable warnings: "structure was padded due to __declspec(align(16))"
  //   #pragma warning(disable: 4324)
  //   
  // #endif
  
#endif


#if defined(__SUNPRO_CC)
  
  // http://www.oracle.com/technetwork/server-storage/solarisstudio/training/index-jsp-141991.html
  // http://www.oracle.com/technetwork/server-storage/solarisstudio/documentation/cplusplus-faq-355066.html
  
  #if (__SUNPRO_CC < 0x5100)
    #error "*** Need a newer compiler ***"
  #endif
  
  #if defined(COOT_USE_CXX11)
    #if (__SUNPRO_CC < 0x5130)
      #undef  COOT_PRINT_CXX11_WARNING
      #define COOT_PRINT_CXX11_WARNING
    #endif
  #endif
  
#endif


#if defined(COOT_USE_CXX11) && defined(__CYGWIN__) && !defined(COOT_DONT_PRINT_CXX11_WARNING)
  #pragma message ("WARNING: Cygwin may have incomplete support for C++11 features.")
#endif


#if defined(COOT_USE_CXX11) && (__cplusplus < 201103L)
  #undef  COOT_PRINT_CXX11_WARNING
  #define COOT_PRINT_CXX11_WARNING
#endif


#if defined(COOT_PRINT_CXX98_WARNING) && !defined(COOT_DONT_PRINT_CXX98_WARNING)
  #pragma message ("WARNING: this compiler is OUTDATED and has INCOMPLETE support for the C++ standard;")
  #pragma message ("WARNING: if something breaks, you get to keep all the pieces.")
#endif


#if defined(COOT_PRINT_CXX11_WARNING) && !defined(COOT_DONT_PRINT_CXX11_WARNING)
  #pragma message ("WARNING: use of C++11 features has been enabled,")
  #pragma message ("WARNING: but this compiler has INCOMPLETE support for C++11;")
  #pragma message ("WARNING: if something breaks, you get to keep all the pieces.")
  #pragma message ("WARNING: to forcefully prevent Bandicoot from using C++11 features,")
  #pragma message ("WARNING: #define COOT_DONT_USE_CXX11 before #include <bandicoot>")
#endif


#undef COOT_PRINT_CXX98_WARNING
#undef COOT_PRINT_CXX11_WARNING


#if defined(log2)
  #undef log2
  #pragma message ("WARNING: detected 'log2' macro and undefined it")
#endif



// 
// whoever defined macros with the names "min" and "max" should be permanently removed from the gene pool

#if defined(min) || defined(max)
  #undef min
  #undef max
  #pragma message ("WARNING: detected 'min' and/or 'max' macros and undefined them;")
  #pragma message ("WARNING: you may wish to define NOMINMAX before including any windows header")
#endif
