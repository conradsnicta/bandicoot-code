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


//! \addtogroup fn_trace
//! @{


template<typename T1>
coot_warn_unused
inline
typename T1::elem_type
trace(const Base<typename T1::elem_type, T1>& X)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  const unwrap<T1>   U(X.get_ref());
  const Mat<eT>& A = U.M;
  
  if(A.n_elem == 0)  { return eT(0); }
  
  const uword diag_len = (std::min)(A.n_rows, A.n_cols);
  
  Mat<eT> tmp(1,1);
  
  coot_runtime_t::cq_guard guard;
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::trace);
  
  cl_mem tmp_mem = tmp.get_device_mem(false);
  cl_mem   A_mem =   A.get_device_mem(false);
  
  coot_runtime_t::adapt_val n_rows(A.n_rows);
  coot_runtime_t::adapt_val      N(diag_len);
  
  cl_int status = 0;
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem),  &tmp_mem   );
  status |= clSetKernelArg(kernel, 1, sizeof(cl_mem),  &A_mem     );
  status |= clSetKernelArg(kernel, 2, n_rows.size,     n_rows.addr);
  status |= clSetKernelArg(kernel, 3, N.size,          N.addr     );
  
  const size_t global_work_size[1] = { size_t(1) };
  
  coot_extra_debug_print("clEnqueueNDRangeKernel()");
  status |= clEnqueueNDRangeKernel(coot_runtime.get_cq(), kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
  
  coot_check_runtime_error( (status != 0), "trace(): couldn't execute kernel" );
  
  const Mat<eT>& tmp2 = tmp;
  const eT val = tmp2(0);
  
  return val;
  }



//! @}
