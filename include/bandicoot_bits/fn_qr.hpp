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


//! \addtogroup fn_qr
//! @{


// TODO: add restrictor to allow only BLAS types
template<typename T1>
inline
bool
qr
  (
         Mat<typename T1::elem_type>&    Q,
         Mat<typename T1::elem_type>&    R,
  const Base<typename T1::elem_type,T1>& X
  )
  {
  coot_extra_debug_sigprint();
  
  coot_debug_check( (&Q == &R), "qr(): Q and R are the same object");
  
  const bool status = false;
  
  // TODO
  // dgeqrf()
  // copy result matrix to new matrix
  // to get R: zero out elements below diagonal in new matrix (NOTE: need similar operation for chol(); use a dedicated kernel?)
  // to get Q: on result matrix apply dorgqr() for double, or zungqr() for cx_double
  
  if(status == false)
    {
    Q.reset();
    R.reset();
    coot_debug_warn("qr(): decomposition failed");
    }
  
  return status;
  }



//! @}
