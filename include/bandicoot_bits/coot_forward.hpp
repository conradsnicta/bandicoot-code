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


using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::size_t;

template<typename elem_type, typename derived> struct Base;
template<typename elem_type, typename derived> struct BaseCube;

template<typename eT> class MatValProxy;

template<typename eT> class Mat;
template<typename eT> class Col;
template<typename eT> class Row;
template<typename eT> class Cube;

template<typename eT> class subview;
template<typename eT> class subview_col;
template<typename eT> class subview_row;
template<typename eT> class subview_cube;


class SizeMat;
class SizeCube;

class coot_empty_class {};

template<const bool, const bool, const bool, const bool> class gemm;
template<const bool, const bool, const bool>             class gemv;


template<                 typename eT, typename gen_type> class  Gen; 

template<                 typename T1, typename  op_type> class   Op; 
template<                 typename T1, typename eop_type> class  eOp;
template<typename out_eT, typename T1, typename  op_type> class mtOp;

template<                 typename T1, typename T2, typename  glue_type> class   Glue;
template<                 typename T1, typename T2, typename eglue_type> class  eGlue;
template<typename out_eT, typename T1, typename T2, typename  glue_type> class mtGlue;



template<                 typename eT, typename gen_type> class  GenCube; 

template<                 typename T1, typename  op_type> class   OpCube; 
template<                 typename T1, typename eop_type> class  eOpCube; 
template<typename out_eT, typename T1, typename  op_type> class mtOpCube;

template<                 typename T1, typename T2, typename  glue_type> class   GlueCube;
template<                 typename T1, typename T2, typename eglue_type> class  eGlueCube;
template<typename out_eT, typename T1, typename T2, typename  glue_type> class mtGlueCube;


struct coot_vec_indicator {};


class op_sum;
class op_inv;
class op_strans;
class op_htrans;
class op_htrans2;

class eop_scalar_plus;
class eop_neg;
class eop_scalar_minus_pre;
class eop_scalar_minus_post;
class eop_scalar_div_pre;
class eop_scalar_div_post;

class eglue_plus;
class eglue_minus;
class eglue_schur;
class eglue_div;

class glue_times;

class glue_mixed_plus;
class glue_mixed_minus;
class glue_mixed_schur;
class glue_mixed_div;
class glue_mixed_times;


//! \addtogroup fill
//! @{

namespace fill
  {
  struct fill_none  {};
  struct fill_zeros {};
  struct fill_ones  {};
  struct fill_eye   {};
  struct fill_randu {};
  struct fill_randn {};
  
  template<typename fill_type> 
  struct fill_class { inline fill_class() {} };
  
  static const fill_class<fill_none > none;
  static const fill_class<fill_zeros> zeros;
  static const fill_class<fill_ones > ones;
  static const fill_class<fill_eye  > eye;
  static const fill_class<fill_randu> randu;
  static const fill_class<fill_randn> randn;
  }

//! @}
