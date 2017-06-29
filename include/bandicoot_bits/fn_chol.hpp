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


//! \addtogroup fn_chol
//! @{


// TODO: add optional 'layout' argument
template<typename T1>
inline
bool
chol(Mat<typename T1::elem_type>& out, const Base<typename T1::elem_type, T1>& X)
  {
  coot_extra_debug_sigprint();
  
    // typedef typename T1::elem_type eT;
  
  coot_debug_check( (coot_rt.is_valid() == false), "coot_rt not valid" );
  
  out = X.get_ref();
  
  // TODO: check whether given matrix is square
  
  magma_int_t info   = 0;
  magma_int_t status = 0;
  
  //// using MAGMA 1.3
  //status = magma_dpotrf_gpu(MagmaUpper, out.n_rows, out.get_dev_mem(), 0, out.n_rows, coot_rt.get_cq(), &info);
  
  // using MAGMA 2.2
  status = magma_dpotrf_gpu(MagmaUpper, out.n_rows, out.get_dev_mem(), out.n_rows, &info);

  // TODO: check status
  
  // TODO: need to set the lower/upper triangular part (excluding the diagonal) to zero
  
  return true;
  }



//! @}
