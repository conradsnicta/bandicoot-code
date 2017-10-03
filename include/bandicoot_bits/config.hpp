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



#if !defined(COOT_USE_LAPACK)
#define COOT_USE_LAPACK
//// Comment out the above line if you don't have LAPACK or a high-speed replacement for LAPACK,
//// such as Intel MKL, AMD ACML, or the Accelerate framework.
//// LAPACK is required for matrix decompositions (eg. SVD) and matrix inverse.
#endif

#if !defined(COOT_USE_BLAS)
#define COOT_USE_BLAS
//// Comment out the above line if you don't have BLAS or a high-speed replacement for BLAS,
//// such as OpenBLAS, GotoBLAS, Intel MKL, AMD ACML, or the Accelerate framework.
//// BLAS is used for matrix multiplication.
//// Without BLAS, matrix multiplication will still work, but might be slower.
#endif

// #define COOT_USE_WRAPPER
//// Comment out the above line if you're getting linking errors when compiling your programs,
//// or if you prefer to directly link with LAPACK, BLAS + etc instead of the Bandicoot runtime library.
//// You will then need to link your programs directly with -llapack -lblas instead of -lbandicoot

// #define COOT_BLAS_CAPITALS
//// Uncomment the above line if your BLAS and LAPACK libraries have capitalised function names

#define COOT_BLAS_UNDERSCORE
//// Uncomment the above line if your BLAS and LAPACK libraries have function names with a trailing underscore.
//// Conversely, comment it out if the function names don't have a trailing underscore.

// #define COOT_BLAS_LONG
//// Uncomment the above line if your BLAS and LAPACK libraries use "long" instead of "int"

// #define COOT_BLAS_LONG_LONG
//// Uncomment the above line if your BLAS and LAPACK libraries use "long long" instead of "int"

#if !defined(COOT_USE_CXX11)
// #define COOT_USE_CXX11
//// Uncomment the above line to forcefully enable use of C++11 features (eg. initialiser lists).
//// Note that COOT_USE_CXX11 is automatically enabled when a C++11 compiler is detected.
#endif

// #define COOT_NO_DEBUG
//// Uncomment the above line if you want to disable all run-time checks.
//// This will result in faster code, but you first need to make sure that your code runs correctly!
//// We strongly recommend to have the run-time checks enabled during development,
//// as this greatly aids in finding mistakes in your code, and hence speeds up development.
//// We recommend that run-time checks be disabled _only_ for the shipped version of your program.

// #define COOT_EXTRA_DEBUG
//// Uncomment the above line if you want to see the function traces of how Bandicoot evaluates expressions.
//// This is mainly useful for debugging of the library.


#if !defined(COOT_COUT_STREAM)
  #define COOT_COUT_STREAM std::cout
#endif

#if !defined(COOT_CERR_STREAM)
  #define COOT_CERR_STREAM std::cerr
#endif

#if !defined(COOT_PRINT_ERRORS)
#define COOT_PRINT_ERRORS
//// Comment out the above line if you don't want errors and warnings printed (eg. failed decompositions)
#endif

#if defined(COOT_DONT_USE_LAPACK)
  #undef COOT_USE_LAPACK
#endif

#if defined(COOT_DONT_USE_BLAS)
  #undef COOT_USE_BLAS
#endif

#if defined(COOT_DONT_USE_WRAPPER)
  #undef COOT_USE_WRAPPER
#endif

#if defined(COOT_DONT_USE_CXX11)
  #undef COOT_USE_CXX11
  #undef COOT_USE_EXTERN_CXX11_RNG
#endif

#if defined(COOT_USE_WRAPPER)
  #if defined(COOT_USE_CXX11)
    #if !defined(COOT_USE_EXTERN_CXX11_RNG)
      // #define COOT_USE_EXTERN_CXX11_RNG
    #endif
  #endif
#endif

#if defined(COOT_DONT_USE_EXTERN_CXX11_RNG)
  #undef COOT_USE_EXTERN_CXX11_RNG
#endif

#if defined(COOT_DONT_PRINT_ERRORS)
  #undef COOT_PRINT_ERRORS
#endif


// if Bandicoot was installed on this system via CMake and COOT_USE_WRAPPER is not defined,
// COOT_AUX_LIBS lists the libraries required by Bandicoot on this system, and
// COOT_AUX_INCDIRS lists the include directories required by Bandicoot on this system.
// Do not use these unless you know what you are doing.
#define COOT_AUX_LIBS
#define COOT_AUX_INCDIRS
