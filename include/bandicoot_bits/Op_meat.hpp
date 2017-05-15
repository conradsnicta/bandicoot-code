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
inline
Op<T1, op_type>::Op(const T1& in_m)
  : m(in_m)
  {
  coot_extra_debug_sigprint();
  }
  


template<typename T1, typename op_type>
inline
Op<T1, op_type>::Op(const T1& in_m, const typename T1::elem_type in_aux)
  : m(in_m)
  , aux(in_aux)
  {
  coot_extra_debug_sigprint();
  }
  


template<typename T1, typename op_type>
inline
Op<T1, op_type>::Op(const T1& in_m, const typename T1::elem_type in_aux, const uword in_aux_uword_a, const uword in_aux_uword_b)
  : m(in_m)
  , aux(in_aux)
  , aux_uword_a(in_aux_uword_a)
  , aux_uword_b(in_aux_uword_b)
  {
  coot_extra_debug_sigprint();
  }
  


template<typename T1, typename op_type>
inline
Op<T1, op_type>::Op(const T1& in_m, const uword in_aux_uword_a, const uword in_aux_uword_b)
  : m(in_m)
  , aux_uword_a(in_aux_uword_a)
  , aux_uword_b(in_aux_uword_b)
  {
  coot_extra_debug_sigprint();
  }



template<typename T1, typename op_type>
inline
Op<T1, op_type>::Op(const T1& in_m, const uword in_aux_uword_a, const uword in_aux_uword_b, const uword in_aux_uword_c, const char)
  : m(in_m)
  , aux_uword_a(in_aux_uword_a)
  , aux_uword_b(in_aux_uword_b)
  , aux_uword_c(in_aux_uword_c)
  {
  coot_extra_debug_sigprint();
  }



template<typename T1, typename op_type>
inline
Op<T1, op_type>::~Op()
  {
  coot_extra_debug_sigprint();
  }



//! @}
