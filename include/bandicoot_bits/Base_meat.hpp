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


//! \addtogroup Base
//! @{



template<typename elem_type, typename derived>
coot_inline
const derived&
Base<elem_type,derived>::get_ref() const
  {
  return static_cast<const derived&>(*this);
  }



template<typename elem_type, typename derived>
inline
void
Base<elem_type,derived>::print(const std::string extra_text) const
  {
  const unwrap<derived> tmp( (*this).get_ref() );
  
  tmp.M.impl_print(extra_text);
  }



template<typename elem_type, typename derived>
inline
void
Base<elem_type,derived>::print(std::ostream& user_stream, const std::string extra_text) const
  {
  const unwrap<derived> tmp( (*this).get_ref() );
  
  tmp.M.impl_print(user_stream, extra_text);
  }
  


template<typename elem_type, typename derived>
inline
void
Base<elem_type,derived>::raw_print(const std::string extra_text) const
  {
  const unwrap<derived> tmp( (*this).get_ref() );
  
  tmp.M.impl_raw_print(extra_text);
  }



template<typename elem_type, typename derived>
inline
void
Base<elem_type,derived>::raw_print(std::ostream& user_stream, const std::string extra_text) const
  {
  const unwrap<derived> tmp( (*this).get_ref() );
  
  tmp.M.impl_raw_print(user_stream, extra_text);
  }



// template<typename elem_type, typename derived>
// inline
// coot_warn_unused
// elem_type
// Base<elem_type,derived>::min() const
//   {
//   return op_min::min( (*this).get_ref() );
//   }
// 
// 
// 
// template<typename elem_type, typename derived>
// inline
// coot_warn_unused
// elem_type
// Base<elem_type,derived>::max() const
//   {
//   return op_max::max( (*this).get_ref() );
//   }
// 
// 
// 
// template<typename elem_type, typename derived>
// inline
// coot_warn_unused
// uword
// Base<elem_type,derived>::index_min() const
//   {
//   const Proxy<derived> P( (*this).get_ref() );
//   
//   uword index = 0;
//   
//   if(P.get_n_elem() == 0)
//     {
//     coot_debug_check(true, "index_min(): object has no elements");
//     }
//   else
//     {
//     op_min::min_with_index(P, index);
//     }
//   
//   return index;
//   }



// template<typename elem_type, typename derived>
// inline
// coot_warn_unused
// uword
// Base<elem_type,derived>::index_max() const
//   {
//   const Proxy<derived> P( (*this).get_ref() );
//   
//   uword index = 0;
//   
//   if(P.get_n_elem() == 0)
//     {
//     coot_debug_check(true, "index_max(): object has no elements");
//     }
//   else
//     {
//     op_max::max_with_index(P, index);
//     }
//   
//   return index;
//   }



//
// extra functions defined in Base_inv_yes

template<typename derived>
coot_inline
const Op<derived,op_inv>
Base_inv_yes<derived>::i() const
  {
  return Op<derived,op_inv>(static_cast<const derived&>(*this));
  }



//
// extra functions defined in Base_eval_Mat

template<typename elem_type, typename derived>
coot_inline
const derived&
Base_eval_Mat<elem_type, derived>::eval() const
  {
  coot_extra_debug_sigprint();
  
  return static_cast<const derived&>(*this);
  }



//
// extra functions defined in Base_eval_expr

template<typename elem_type, typename derived>
coot_inline
Mat<elem_type>
Base_eval_expr<elem_type, derived>::eval() const
  {
  coot_extra_debug_sigprint();
  
  return Mat<elem_type>( static_cast<const derived&>(*this) );
  }



//
// extra functions defined in Base_trans_cx

template<typename derived>
coot_inline
const Op<derived,op_htrans>
Base_trans_cx<derived>::t() const
  {
  return Op<derived,op_htrans>( static_cast<const derived&>(*this) );
  }



template<typename derived>
coot_inline
const Op<derived,op_htrans>
Base_trans_cx<derived>::ht() const
  {
  return Op<derived,op_htrans>( static_cast<const derived&>(*this) );
  }



template<typename derived>
coot_inline
const Op<derived,op_strans>
Base_trans_cx<derived>::st() const
  {
  return Op<derived,op_strans>( static_cast<const derived&>(*this) );
  }



//
// extra functions defined in Base_trans_default

template<typename derived>
coot_inline
const Op<derived,op_htrans>
Base_trans_default<derived>::t() const
  {
  return Op<derived,op_htrans>( static_cast<const derived&>(*this) );
  }



template<typename derived>
coot_inline
const Op<derived,op_htrans>
Base_trans_default<derived>::ht() const
  {
  return Op<derived,op_htrans>( static_cast<const derived&>(*this) );
  }



template<typename derived>
coot_inline
const Op<derived,op_htrans>
Base_trans_default<derived>::st() const
  {
  return Op<derived,op_htrans>( static_cast<const derived&>(*this) );
  }



//! @}
