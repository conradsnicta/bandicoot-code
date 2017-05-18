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


//! \addtogroup fn_strans
//! @{



template<typename T1>
coot_warn_unused
coot_inline
const Op<T1, op_strans>
strans
  (
  const Base<typename T1::elem_type,T1>& X,
  const typename coot_cx_only<typename T1::elem_type>::result* junk = 0
  )
  {
  coot_extra_debug_sigprint();
  coot_ignore(junk);
  
  return Op<T1, op_strans>(X.get_ref());
  }



// NOTE: for non-complex objects, using op_htrans instead of op_strans to simplify implementation in glue_times
template<typename T1>
coot_warn_unused
coot_inline
const Op<T1, op_htrans>
strans
  (
  const Base<typename T1::elem_type,T1>& X,
  const typename coot_not_cx<typename T1::elem_type>::result* junk1 = 0
  )
  {
  coot_extra_debug_sigprint();
  coot_ignore(junk);
  
  return Op<T1, op_htrans>(X.get_ref());
  }



//! @}
