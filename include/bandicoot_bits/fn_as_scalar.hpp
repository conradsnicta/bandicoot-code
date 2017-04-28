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


//! \addtogroup fn_as_scalar
//! @{


template<typename T1>
coot_warn_unused
inline
typename T1::elem_type
as_scalar(const Base<typename T1::elem_type, T1>& X)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  const unwrap<T1>   U(X.get_ref());
  const Mat<eT>& A = U.M;
  
  if(A.n_elem != 1)
    {
    coot_debug_check(true, "as_scalar(): expression doesn't evaluate to exactly one element");
    
    return Datum<eT>::nan;
    }
    
  return A(0,0);
  }



//! @}
