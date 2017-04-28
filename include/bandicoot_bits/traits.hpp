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


//! \addtogroup traits
//! @{


template<typename T1>
struct get_pod_type
  { typedef T1 result; };

template<typename T2>
struct get_pod_type< std::complex<T2> >
  { typedef T2 result; };



template<typename T>
struct is_Mat_only
  { static const bool value = false; };

template<typename eT>
struct is_Mat_only< Mat<eT> >
  { static const bool value = true; };

template<typename eT>
struct is_Mat_only< const Mat<eT> >
  { static const bool value = true; };



template<typename T>
struct is_Mat
  { static const bool value = false; };

template<typename eT>
struct is_Mat< Mat<eT> >
  { static const bool value = true; };

template<typename eT>
struct is_Mat< const Mat<eT> >
  { static const bool value = true; };

template<typename eT>
struct is_Mat< Row<eT> >
  { static const bool value = true; };

template<typename eT>
struct is_Mat< const Row<eT> >
  { static const bool value = true; };

template<typename eT>
struct is_Mat< Col<eT> >
  { static const bool value = true; };

template<typename eT>
struct is_Mat< const Col<eT> >
  { static const bool value = true; };



template<typename T>
struct is_Row
  { static const bool value = false; };

template<typename eT>
struct is_Row< Row<eT> >
  { static const bool value = true; };

template<typename eT>
struct is_Row< const Row<eT> >
  { static const bool value = true; };



template<typename T>
struct is_Col
  { static const bool value = false; };

template<typename eT>
struct is_Col< Col<eT> >
  { static const bool value = true; };

template<typename eT>
struct is_Col< const Col<eT> >
  { static const bool value = true; };



//
//
//



template<typename T>
struct is_subview
  { static const bool value = false; };

template<typename eT>
struct is_subview< subview<eT> >
  { static const bool value = true; };

template<typename eT>
struct is_subview< const subview<eT> >
  { static const bool value = true; };


template<typename T>
struct is_subview_row
  { static const bool value = false; };

template<typename eT>
struct is_subview_row< subview_row<eT> >
  { static const bool value = true; };

template<typename eT>
struct is_subview_row< const subview_row<eT> >
  { static const bool value = true; };


template<typename T>
struct is_subview_col
  { static const bool value = false; };

template<typename eT>
struct is_subview_col< subview_col<eT> >
  { static const bool value = true; };

template<typename eT>
struct is_subview_col< const subview_col<eT> >
  { static const bool value = true; };



//
//
//


template<typename T>
struct is_Gen
  { static const bool value = false; };
 
template<typename T1, typename gen_type>
struct is_Gen< Gen<T1,gen_type> >
  { static const bool value = true; };
 
template<typename T1, typename gen_type>
struct is_Gen< const Gen<T1,gen_type> >
  { static const bool value = true; };


template<typename T>
struct is_Op
  { static const bool value = false; };
 
template<typename T1, typename op_type>
struct is_Op< Op<T1,op_type> >
  { static const bool value = true; };
 
template<typename T1, typename op_type>
struct is_Op< const Op<T1,op_type> >
  { static const bool value = true; };


template<typename T>
struct is_eOp
  { static const bool value = false; };
 
template<typename T1, typename eop_type>
struct is_eOp< eOp<T1,eop_type> >
  { static const bool value = true; };
 
template<typename T1, typename eop_type>
struct is_eOp< const eOp<T1,eop_type> >
  { static const bool value = true; };


template<typename T>
struct is_mtOp
  { static const bool value = false; };
 
template<typename eT, typename T1, typename op_type>
struct is_mtOp< mtOp<eT, T1, op_type> >
  { static const bool value = true; };
 
template<typename eT, typename T1, typename op_type>
struct is_mtOp< const mtOp<eT, T1, op_type> >
  { static const bool value = true; };


template<typename T>
struct is_Glue
  { static const bool value = false; };
 
template<typename T1, typename T2, typename glue_type>
struct is_Glue< Glue<T1,T2,glue_type> >
  { static const bool value = true; };

template<typename T1, typename T2, typename glue_type>
struct is_Glue< const Glue<T1,T2,glue_type> >
  { static const bool value = true; };


template<typename T>
struct is_eGlue
  { static const bool value = false; };
 
template<typename T1, typename T2, typename eglue_type>
struct is_eGlue< eGlue<T1,T2,eglue_type> >
  { static const bool value = true; };

template<typename T1, typename T2, typename eglue_type>
struct is_eGlue< const eGlue<T1,T2,eglue_type> >
  { static const bool value = true; };


template<typename T>
struct is_mtGlue
  { static const bool value = false; };
 
template<typename eT, typename T1, typename T2, typename glue_type>
struct is_mtGlue< mtGlue<eT, T1, T2, glue_type> >
  { static const bool value = true; };

template<typename eT, typename T1, typename T2, typename glue_type>
struct is_mtGlue< const mtGlue<eT, T1, T2, glue_type> >
  { static const bool value = true; };


//
//
//



template<typename T>
struct is_Cube
  { static const bool value = false; };

template<typename eT>
struct is_Cube< Cube<eT> >
  { static const bool value = true; };


//
//
//



template<typename T>
struct is_GenCube
  { static const bool value = false; };
 
template<typename eT, typename gen_type>
struct is_GenCube< GenCube<eT,gen_type> >
  { static const bool value = true; };
 

template<typename T>
struct is_OpCube
  { static const bool value = false; };
 
template<typename T1, typename op_type>
struct is_OpCube< OpCube<T1,op_type> >
  { static const bool value = true; };


template<typename T>
struct is_eOpCube
  { static const bool value = false; };
 
template<typename T1, typename eop_type>
struct is_eOpCube< eOpCube<T1,eop_type> >
  { static const bool value = true; };
 

template<typename T>
struct is_mtOpCube
  { static const bool value = false; };
 
template<typename eT, typename T1, typename op_type>
struct is_mtOpCube< mtOpCube<eT, T1, op_type> >
  { static const bool value = true; };
 

template<typename T>
struct is_GlueCube
  { static const bool value = false; };
 
template<typename T1, typename T2, typename glue_type>
struct is_GlueCube< GlueCube<T1,T2,glue_type> >
  { static const bool value = true; };


template<typename T>
struct is_eGlueCube
  { static const bool value = false; };
 
template<typename T1, typename T2, typename eglue_type>
struct is_eGlueCube< eGlueCube<T1,T2,eglue_type> >
  { static const bool value = true; };


template<typename T>
struct is_mtGlueCube
  { static const bool value = false; };
 
template<typename eT, typename T1, typename T2, typename glue_type>
struct is_mtGlueCube< mtGlueCube<eT, T1, T2, glue_type> >
  { static const bool value = true; };


//
//
//



template<typename T1>
struct is_coot_type
  {
  static const bool value
  =  is_Mat<T1>::value
  || is_Gen<T1>::value
  || is_Op<T1>::value
  || is_Glue<T1>::value
  || is_eOp<T1>::value
  || is_eGlue<T1>::value
  || is_mtOp<T1>::value
  || is_mtGlue<T1>::value
  || is_subview<T1>::value
  ;
  };



template<typename T1>
struct is_coot_cube_type
  {
  static const bool value
  =  is_Cube<T1>::value
  || is_GenCube<T1>::value
  || is_OpCube<T1>::value
  || is_eOpCube<T1>::value
  || is_mtOpCube<T1>::value
  || is_GlueCube<T1>::value
  || is_eGlueCube<T1>::value
  || is_mtGlueCube<T1>::value
  // TODO: subview_cube
  ;
  };



//
//
//


template<typename T1, typename T2>
struct is_same_type
  {
  static const bool value = false;
  static const bool yes   = false;
  static const bool no    = true;
  };


template<typename T1>
struct is_same_type<T1,T1>
  {
  static const bool value = true;
  static const bool yes   = true;
  static const bool no    = false;
  };



//
//
//


template<typename T1>
struct is_u8
  { static const bool value = false; };

template<>
struct is_u8<u8>
  { static const bool value = true; };



template<typename T1>
struct is_s8
  { static const bool value = false; };

template<>
struct is_s8<s8>
  { static const bool value = true; };



template<typename T1>
struct is_u16
  { static const bool value = false; };

template<>
struct is_u16<u16>
  { static const bool value = true; };



template<typename T1>
struct is_s16
  { static const bool value = false; };

template<>
struct is_s16<s16>
  { static const bool value = true; };



template<typename T1>
struct is_u32
  { static const bool value = false; };

template<>
struct is_u32<u32>
  { static const bool value = true; };



template<typename T1>
struct is_s32
  { static const bool value = false; };

template<>
struct is_s32<s32>
  { static const bool value = true; };



template<typename T1>
struct is_u64
  { static const bool value = false; };

template<>
struct is_u64<u64>
  { static const bool value = true; };


template<typename T1>
struct is_s64
  { static const bool value = false; };

template<>
struct is_s64<s64>
  { static const bool value = true; };



template<typename T1>
struct is_uword
  { static const bool value = false; };

template<>
struct is_uword<uword>
  { static const bool value = true; };



template<typename T1>
struct is_sword
  { static const bool value = false; };

template<>
struct is_sword<sword>
  { static const bool value = true; };



template<typename T1>
struct is_float
  { static const bool value = false; };

template<>
struct is_float<float>
  { static const bool value = true; };



template<typename T1>
struct is_double
  { static const bool value = false; };

template<>
struct is_double<double>
  { static const bool value = true; };



template<typename T1>
struct is_real
  { static const bool value = false; };

template<>
struct is_real<float>
  { static const bool value = true; };
  
template<>
struct is_real<double>
  { static const bool value = true; };




template<typename T1>
struct is_not_complex
  { static const bool value = true; };

template<typename eT>
struct is_not_complex< std::complex<eT> >
  { static const bool value = false; };



template<typename T1>
struct is_complex
  { static const bool value = false; };

// template<>
template<typename eT>
struct is_complex< std::complex<eT> >
  { static const bool value = true; };



template<typename T1>
struct is_complex_float
  { static const bool value = false; };

template<>
struct is_complex_float< std::complex<float> >
  { static const bool value = true; };



template<typename T1>
struct is_complex_double
  { static const bool value = false; };

template<>
struct is_complex_double< std::complex<double> >
  { static const bool value = true; };



template<typename T1>
struct is_complex_strict
  { static const bool value = false; };

template<>
struct is_complex_strict< std::complex<float> >
  { static const bool value = true; };

template<>
struct is_complex_strict< std::complex<double> >
  { static const bool value = true; };



template<typename T1>
struct is_cx
  {
  static const bool value = false;
  static const bool yes   = false;
  static const bool no    = true;
  };

// template<>
template<typename T>
struct is_cx< std::complex<T> >
  {
  static const bool value = true;
  static const bool yes   = true;
  static const bool no    = false;
  };



//! check for a weird implementation of the std::complex class
template<typename T1>
struct is_supported_complex
  { static const bool value = false; };

//template<>
template<typename eT>
struct is_supported_complex< std::complex<eT> >
  { static const bool value = ( sizeof(std::complex<eT>) == 2*sizeof(eT) ); };



template<typename T1>
struct is_supported_complex_float
  { static const bool value = false; };

template<>
struct is_supported_complex_float< std::complex<float> >
  { static const bool value = ( sizeof(std::complex<float>) == 2*sizeof(float) ); };



template<typename T1>
struct is_supported_complex_double
  { static const bool value = false; };

template<>
struct is_supported_complex_double< std::complex<double> >
  { static const bool value = ( sizeof(std::complex<double>) == 2*sizeof(double) ); };



template<typename T1>
struct is_supported_elem_type
  {
  static const bool value = \
    is_u8<T1>::value ||
    is_s8<T1>::value ||
    is_u16<T1>::value ||
    is_s16<T1>::value ||
    is_u32<T1>::value ||
    is_s32<T1>::value ||
    is_u64<T1>::value ||
    is_s64<T1>::value ||
    is_float<T1>::value ||
    is_double<T1>::value ||
    is_supported_complex_float<T1>::value ||
    is_supported_complex_double<T1>::value;
  };



template<typename T1>
struct is_supported_blas_type
  {
  static const bool value = \
    is_float<T1>::value ||
    is_double<T1>::value ||
    is_supported_complex_float<T1>::value ||
    is_supported_complex_double<T1>::value;
  };



template<typename T>
struct is_signed
  {
  static const bool value = true;
  };


template<> struct is_signed<u8>     { static const bool value = false; };
template<> struct is_signed<u16>    { static const bool value = false; };
template<> struct is_signed<u32>    { static const bool value = false; };
template<> struct is_signed<u64>    { static const bool value = false; };


template<typename T>
struct is_non_integral
  {
  static const bool value = false;
  };


template<> struct is_non_integral<              float   > { static const bool value = true; };
template<> struct is_non_integral<              double  > { static const bool value = true; };
template<> struct is_non_integral< std::complex<float>  > { static const bool value = true; };
template<> struct is_non_integral< std::complex<double> > { static const bool value = true; };




//

class coot_junk_class;

template<typename T1, typename T2>
struct force_different_type
  {
  typedef T1 T1_result;
  typedef T2 T2_result;
  };
  

template<typename T1>
struct force_different_type<T1,T1>
  {
  typedef T1              T1_result;
  typedef coot_junk_class T2_result;
  };
  
  

//


template<typename T1>
struct resolves_to_vector_default { static const bool value = false;                    };

template<typename T1>
struct resolves_to_vector_test    { static const bool value = T1::is_col || T1::is_row; };


template<typename T1, bool condition>
struct resolves_to_vector_redirect {};

template<typename T1>
struct resolves_to_vector_redirect<T1, false> { typedef resolves_to_vector_default<T1> result; };

template<typename T1>
struct resolves_to_vector_redirect<T1, true>  { typedef resolves_to_vector_test<T1>    result; };


template<typename T1>
struct resolves_to_vector : public resolves_to_vector_redirect<T1, is_coot_type<T1>::value>::result {};


//

template<typename T1>
struct resolves_to_rowvector_default { static const bool value = false;      };

template<typename T1>
struct resolves_to_rowvector_test    { static const bool value = T1::is_row; };


template<typename T1, bool condition>
struct resolves_to_rowvector_redirect {};

template<typename T1>
struct resolves_to_rowvector_redirect<T1, false> { typedef resolves_to_rowvector_default<T1> result; };

template<typename T1>
struct resolves_to_rowvector_redirect<T1, true>  { typedef resolves_to_rowvector_test<T1>    result; };


template<typename T1>
struct resolves_to_rowvector : public resolves_to_rowvector_redirect<T1, is_coot_type<T1>::value>::result {};

//

template<typename T1>
struct resolves_to_colvector_default { static const bool value = false;      };

template<typename T1>
struct resolves_to_colvector_test    { static const bool value = T1::is_col; };


template<typename T1, bool condition>
struct resolves_to_colvector_redirect {};

template<typename T1>
struct resolves_to_colvector_redirect<T1, false> { typedef resolves_to_colvector_default<T1> result; };

template<typename T1>
struct resolves_to_colvector_redirect<T1, true>  { typedef resolves_to_colvector_test<T1>    result; };


template<typename T1>
struct resolves_to_colvector : public resolves_to_colvector_redirect<T1, is_coot_type<T1>::value>::result {};



//! @}
