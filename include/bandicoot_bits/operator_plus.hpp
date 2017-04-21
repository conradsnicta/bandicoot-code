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


//! \addtogroup operator_plus
//! @{



//! unary plus operation (does nothing, but is required for completeness)
template<typename T1>
coot_inline
typename enable_if2< is_coot_type<T1>::value, const T1& >::result
operator+
(const T1& X)
  {
  coot_extra_debug_sigprint();
  
  return X;
  }



//! Base + scalar
template<typename T1>
coot_inline
typename enable_if2< is_coot_type<T1>::value, const eOp<T1, eop_scalar_plus> >::result
operator+
(const T1& X, const typename T1::elem_type k)
  {
  coot_extra_debug_sigprint();
  
  return eOp<T1, eop_scalar_plus>(X, k);
  }



//! scalar + Base
template<typename T1>
coot_inline
typename enable_if2< is_coot_type<T1>::value, const eOp<T1, eop_scalar_plus> >::result
operator+
(const typename T1::elem_type k, const T1& X)
  {
  coot_extra_debug_sigprint();
  
  return eOp<T1, eop_scalar_plus>(X, k);  // NOTE: order is swapped
  }



//! addition of Base objects with same element type
template<typename T1, typename T2>
coot_inline
typename
enable_if2
  <
  is_coot_type<T1>::value && is_coot_type<T2>::value && is_same_type<typename T1::elem_type, typename T2::elem_type>::value,
  const eGlue<T1, T2, eglue_plus>
  >::result
operator+
  (
  const T1& X,
  const T2& Y
  )
  {
  coot_extra_debug_sigprint();
  
  return eGlue<T1, T2, eglue_plus>(X, Y);
  }



//! addition of Base objects with different element types
template<typename T1, typename T2>
inline
typename
enable_if2
  <
  (is_coot_type<T1>::value && is_coot_type<T2>::value && (is_same_type<typename T1::elem_type, typename T2::elem_type>::no)),
  const mtGlue<typename promote_type<typename T1::elem_type, typename T2::elem_type>::result, T1, T2, glue_mixed_plus>
  >::result
operator+
  (
  const T1& X,
  const T2& Y
  )
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT1;
  typedef typename T2::elem_type eT2;
  
  typedef typename promote_type<eT1,eT2>::result out_eT;
  
  promote_type<eT1,eT2>::check();
  
  return mtGlue<out_eT, T1, T2, glue_mixed_plus>( X, Y );
  }



//! @}
