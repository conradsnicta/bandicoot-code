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


//! \addtogroup restrictors
//! @{



template<typename T> struct coot_scalar_only { };

template<> struct coot_scalar_only<u8>     { typedef u8     result; };
template<> struct coot_scalar_only<s8>     { typedef s8     result; };
template<> struct coot_scalar_only<u16>    { typedef u16    result; };
template<> struct coot_scalar_only<s16>    { typedef s16    result; };
template<> struct coot_scalar_only<u32>    { typedef u32    result; };
template<> struct coot_scalar_only<s32>    { typedef s32    result; };
#if defined(COOT_USE_U64S64)
template<> struct coot_scalar_only<u64>    { typedef u64    result; };
template<> struct coot_scalar_only<s64>    { typedef s64    result; };
#endif
template<> struct coot_scalar_only<float>  { typedef float  result; };
template<> struct coot_scalar_only<double> { typedef double result; };
#if defined(COOT_ALLOW_LONG)
template<> struct coot_scalar_only<ulng_t> { typedef ulng_t result; };
template<> struct coot_scalar_only<slng_t> { typedef slng_t result; };
#endif

template<typename T>
struct coot_scalar_only< std::complex<T> > { typedef std::complex<T> result; };



template<typename T> struct coot_integral_only { };

template<> struct coot_integral_only<u8>  { typedef u8  result; };
template<> struct coot_integral_only<s8>  { typedef s8  result; };
template<> struct coot_integral_only<u16> { typedef u16 result; };
template<> struct coot_integral_only<s16> { typedef s16 result; };
template<> struct coot_integral_only<u32> { typedef u32 result; };
template<> struct coot_integral_only<s32> { typedef s32 result; };
#if defined(COOT_USE_U64S64)
template<> struct coot_integral_only<u64> { typedef u64 result; };
template<> struct coot_integral_only<s64> { typedef s64 result; };
#endif
#if defined(COOT_ALLOW_LONG)
template<> struct coot_integral_only<ulng_t> { typedef ulng_t result; };
template<> struct coot_integral_only<slng_t> { typedef slng_t result; };
#endif



template<typename T> struct coot_unsigned_integral_only { };

template<> struct coot_unsigned_integral_only<u8>     { typedef u8     result; };
template<> struct coot_unsigned_integral_only<u16>    { typedef u16    result; };
template<> struct coot_unsigned_integral_only<u32>    { typedef u32    result; };
#if defined(COOT_USE_U64S64)
template<> struct coot_unsigned_integral_only<u64>    { typedef u64    result; };
#endif
#if defined(COOT_ALLOW_LONG)
template<> struct coot_unsigned_integral_only<ulng_t> { typedef ulng_t result; };
#endif



template<typename T> struct coot_signed_integral_only { };

template<> struct coot_signed_integral_only<s8>     { typedef s8     result; };
template<> struct coot_signed_integral_only<s16>    { typedef s16    result; };
template<> struct coot_signed_integral_only<s32>    { typedef s32    result; };
#if defined(COOT_USE_U64S64)
template<> struct coot_signed_integral_only<s64>    { typedef s64    result; };
#endif
#if defined(COOT_ALLOW_LONG)
template<> struct coot_signed_integral_only<slng_t> { typedef slng_t result; };
#endif



template<typename T> struct coot_signed_only { };

template<> struct coot_signed_only<s8>     { typedef s8     result; };
template<> struct coot_signed_only<s16>    { typedef s16    result; };
template<> struct coot_signed_only<s32>    { typedef s32    result; };
#if defined(COOT_USE_U64S64)
template<> struct coot_signed_only<s64>    { typedef s64    result; };
#endif
template<> struct coot_signed_only<float>  { typedef float  result; };
template<> struct coot_signed_only<double> { typedef double result; };
#if defined(COOT_ALLOW_LONG)
template<> struct coot_signed_only<slng_t> { typedef slng_t result; };
#endif

template<typename T> struct coot_signed_only< std::complex<T> > { typedef std::complex<T> result; };



template<typename T> struct coot_real_only { };

template<> struct coot_real_only<float>  { typedef float  result; };
template<> struct coot_real_only<double> { typedef double result; };


template<typename T> struct coot_real_or_cx_only { };

template<> struct coot_real_or_cx_only< float >                { typedef float                result; };
template<> struct coot_real_or_cx_only< double >               { typedef double               result; };
template<> struct coot_real_or_cx_only< std::complex<float>  > { typedef std::complex<float>  result; };
template<> struct coot_real_or_cx_only< std::complex<double> > { typedef std::complex<double> result; };



template<typename T> struct coot_cx_only { };

template<> struct coot_cx_only< std::complex<float>  > { typedef std::complex<float>  result; };
template<> struct coot_cx_only< std::complex<double> > { typedef std::complex<double> result; };



template<typename T> struct coot_not_cx                    { typedef T result; };
template<typename T> struct coot_not_cx< std::complex<T> > { };



template<typename T> struct coot_blas_type_only { };

template<> struct coot_blas_type_only< float                > { typedef float                result; };
template<> struct coot_blas_type_only< double               > { typedef double               result; };
template<> struct coot_blas_type_only< std::complex<float>  > { typedef std::complex<float>  result; };
template<> struct coot_blas_type_only< std::complex<double> > { typedef std::complex<double> result; };



template<typename T> struct coot_not_blas_type { typedef T result; };

template<> struct coot_not_blas_type< float                > {  };
template<> struct coot_not_blas_type< double               > {  };
template<> struct coot_not_blas_type< std::complex<float>  > {  };
template<> struct coot_not_blas_type< std::complex<double> > {  };



template<typename T> struct coot_Mat_Col_Row_only { };

template<typename eT> struct coot_Mat_Col_Row_only< Mat<eT> > { typedef Mat<eT> result; };
template<typename eT> struct coot_Mat_Col_Row_only< Col<eT> > { typedef Col<eT> result; };
template<typename eT> struct coot_Mat_Col_Row_only< Row<eT> > { typedef Row<eT> result; };



template<typename  T> struct coot_Cube_only             { };
template<typename eT> struct coot_Cube_only< Cube<eT> > { typedef Cube<eT> result; };


template<bool> struct enable_if       {                     };
template<>     struct enable_if<true> { typedef int result; };


template<bool, typename result_type > struct enable_if2                    {                             };
template<      typename result_type > struct enable_if2<true, result_type> { typedef result_type result; };



//! @}
