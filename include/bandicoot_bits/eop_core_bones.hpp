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


//! \addtogroup eop_core
//! @{



template<typename eop_type>
class eop_core
  {
  public:
  
  // matrices
  
  template<typename T1> inline static void apply              (Mat<typename T1::elem_type>& out, const eOp<T1, eop_type>& x);
  template<typename T1> inline static void apply_inplace_plus (Mat<typename T1::elem_type>& out, const eOp<T1, eop_type>& x);
  template<typename T1> inline static void apply_inplace_minus(Mat<typename T1::elem_type>& out, const eOp<T1, eop_type>& x);
  template<typename T1> inline static void apply_inplace_schur(Mat<typename T1::elem_type>& out, const eOp<T1, eop_type>& x);
  template<typename T1> inline static void apply_inplace_div  (Mat<typename T1::elem_type>& out, const eOp<T1, eop_type>& x);
  
  
  // cubes
  
  // TODO
  };



class eop_scalar_plus       : public eop_core<eop_scalar_plus>       {};
class eop_neg               : public eop_core<eop_neg>               {};
class eop_scalar_minus_pre  : public eop_core<eop_scalar_minus_pre>  {};
class eop_scalar_minus_post : public eop_core<eop_scalar_minus_post> {};
class eop_scalar_times      : public eop_core<eop_scalar_times>      {};
class eop_scalar_div_pre    : public eop_core<eop_scalar_div_pre>    {};
class eop_scalar_div_post   : public eop_core<eop_scalar_div_post>   {};
class eop_square            : public eop_core<eop_square>            {};
class eop_sqrt              : public eop_core<eop_sqrt>              {};

// // TODO
class eop_log               : public eop_core<eop_log>               {};
// class eop_log2              : public eop_core<eop_log2>              {};
// class eop_log10             : public eop_core<eop_log10>             {};
// class eop_trunc_log         : public eop_core<eop_trunc_log>         {};
class eop_exp               : public eop_core<eop_exp>               {};
// class eop_exp2              : public eop_core<eop_exp2>              {};
// class eop_exp10             : public eop_core<eop_exp10>             {};
// class eop_trunc_exp         : public eop_core<eop_trunc_exp>         {};
// class eop_cos               : public eop_core<eop_cos>               {};
// class eop_sin               : public eop_core<eop_sin>               {};
// class eop_tan               : public eop_core<eop_tan>               {};
// class eop_acos              : public eop_core<eop_acos>              {};
// class eop_asin              : public eop_core<eop_asin>              {};
// class eop_atan              : public eop_core<eop_atan>              {};
// class eop_cosh              : public eop_core<eop_cosh>              {};
// class eop_sinh              : public eop_core<eop_sinh>              {};
// class eop_tanh              : public eop_core<eop_tanh>              {};
// class eop_acosh             : public eop_core<eop_acosh>             {};
// class eop_asinh             : public eop_core<eop_asinh>             {};
// class eop_atanh             : public eop_core<eop_atanh>             {};
// class eop_abs               : public eop_core<eop_abs>               {};
// class eop_arg               : public eop_core<eop_arg>               {};
// class eop_conj              : public eop_core<eop_conj>              {};
// class eop_pow               : public eop_core<eop_pow>               {};
// class eop_floor             : public eop_core<eop_floor>             {};
// class eop_ceil              : public eop_core<eop_ceil>              {};
// class eop_round             : public eop_core<eop_round>             {};
// class eop_trunc             : public eop_core<eop_trunc>             {};
// class eop_sign              : public eop_core<eop_sign>              {};
// class eop_erf               : public eop_core<eop_erf>               {};
// class eop_erfc              : public eop_core<eop_erfc>              {};
// class eop_lgamma            : public eop_core<eop_lgamma>            {};

template<typename T> inline std::string eop_name() { return "unknown"; }

template<> inline std::string eop_name<eop_scalar_plus>() { return "plus"; }
template<> inline std::string eop_name<eop_neg>() { return "neg"; }
template<> inline std::string eop_name<eop_scalar_minus_pre>() { return "minus_pre"; }
template<> inline std::string eop_name<eop_scalar_minus_post>() { return "minus_post"; }
template<> inline std::string eop_name<eop_scalar_times>() { return "times"; }
template<> inline std::string eop_name<eop_scalar_div_pre>() { return "div_pre"; }
template<> inline std::string eop_name<eop_scalar_div_post>() { return "div_post"; }
template<> inline std::string eop_name<eop_square>() { return "square"; }
template<> inline std::string eop_name<eop_sqrt>() { return "sqrt"; }

template<typename T> inline std::string eop_op() { return "unknown"; }

template<> inline std::string eop_op<eop_sqrt>() { return "sqrt"; }
template<> inline std::string eop_op<eop_neg>() { return "-"; }


//! @}
