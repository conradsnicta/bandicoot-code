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


//! \addtogroup Op
//! @{



template<typename T1, typename op_type>
class Op : public Base< typename T1::elem_type, Op<T1, op_type> >
  {
  public:
  
  typedef typename T1::elem_type                   elem_type;
  typedef typename get_pod_type<elem_type>::result pod_type;
  
  inline explicit Op(const T1& in_m);
  inline          Op(const T1& in_m, const elem_type in_aux);
  inline          Op(const T1& in_m, const elem_type in_aux,         const uword in_aux_uword_a, const uword in_aux_uword_b);
  inline          Op(const T1& in_m, const uword     in_aux_uword_a, const uword in_aux_uword_b);
  inline          Op(const T1& in_m, const uword     in_aux_uword_a, const uword in_aux_uword_b, const uword in_aux_uword_c, const char junk);
  inline         ~Op();
  
  coot_aligned const T1&       m;
  coot_aligned       elem_type aux;
  coot_aligned       uword     aux_uword_a;
  coot_aligned       uword     aux_uword_b;
  coot_aligned       uword     aux_uword_c;
  
  static const bool is_row = false; // TODO: expand
  static const bool is_col = false; // TODO: expand
  static const bool is_vec = (is_row || is_col) || (is_same_type<op_type, op_sum>::yes);
  };



//! @}
