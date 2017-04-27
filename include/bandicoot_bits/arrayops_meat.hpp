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


//! \addtogroup arrayops
//! @{



template<typename eT>
inline
void
arrayops::copy(cl_mem dest, cl_mem src, const uword n_elem)
  {
  coot_extra_debug_sigprint();
  
  coot_runtime_t::queue_guard guard;
  
  coot_extra_debug_print("clEnqueueCopyBuffer()");
  
  cl_int status = clEnqueueCopyBuffer(coot_runtime.get_queue(), src, dest, size_t(0), size_t(0), sizeof(eT)*size_t(n_elem), cl_uint(0), NULL, NULL);
  
  coot_check_runtime_error( (status != 0), "arrayops::copy(): couldn't copy buffer" );
  }



template<typename eT>
inline
void
arrayops::inplace_op_scalar(cl_mem dest, const eT val, const uword n_elem, cl_kernel kernel)
  {
  coot_extra_debug_sigprint();
  
  coot_runtime_t::queue_guard guard;
  
  coot_runtime_t::adapt_val N(n_elem);
  
  cl_int status = 0;
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &dest );
  status |= clSetKernelArg(kernel, 1, sizeof(eT),     &val  );
  status |= clSetKernelArg(kernel, 2, N.size,         N.addr);
  
  const size_t global_work_size[1] = { size_t(n_elem) };
  
  coot_extra_debug_print("clEnqueueNDRangeKernel()");
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_queue(), kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
  
  coot_check_runtime_error( (status != 0), "arrayops::inplace_op_scalar(): couldn't execute kernel" );
  }



template<typename eT>
inline
void
arrayops::inplace_set_scalar(cl_mem dest, const eT val, const uword n_elem)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::inplace_set_scalar);
  
  arrayops::inplace_op_scalar(dest, val, n_elem, kernel);
  }



template<typename eT>
inline
void
arrayops::inplace_plus_scalar(cl_mem dest, const eT val, const uword n_elem)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::inplace_plus_scalar);
  
  arrayops::inplace_op_scalar(dest, val, n_elem, kernel);
  }



template<typename eT>

inline
void
arrayops::inplace_minus_scalar(cl_mem dest, const eT val, const uword n_elem)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::inplace_minus_scalar);
  
  arrayops::inplace_op_scalar(dest, val, n_elem, kernel);
  }



template<typename eT>

inline
void
arrayops::inplace_mul_scalar(cl_mem dest, const eT val, const uword n_elem)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::inplace_mul_scalar);
  
  arrayops::inplace_op_scalar(dest, val, n_elem, kernel);
  }



template<typename eT>

inline
void
arrayops::inplace_div_scalar(cl_mem dest, const eT val, const uword n_elem)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::inplace_div_scalar);
  
  arrayops::inplace_op_scalar(dest, val, n_elem, kernel);
  }



template<typename eT>
inline
void
arrayops::inplace_op_array(cl_mem dest, cl_mem src, const uword n_elem, cl_kernel kernel)
  {
  coot_extra_debug_sigprint();
  
  coot_runtime_t::queue_guard guard;
  
  coot_runtime_t::adapt_val N(n_elem);
  
  cl_int status = 0;
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &dest  );
  status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &src   );
  status |= clSetKernelArg(kernel, 2, N.size,          N.addr);
  
  const size_t global_work_size[1] = { size_t(n_elem) };
  
  coot_extra_debug_print("clEnqueueNDRangeKernel()");
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_queue(), kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
  
  coot_check_runtime_error( (status != 0), "arrayops::inplace_op_array(): couldn't execute kernel" );
  }



template<typename eT>
inline
void
arrayops::inplace_plus_array(cl_mem dest, cl_mem src, const uword n_elem)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::inplace_plus_array);
  
  arrayops::inplace_op_array<eT>(dest, src, n_elem, kernel);
  }



template<typename eT>
inline
void
arrayops::inplace_minus_array(cl_mem dest, cl_mem src, const uword n_elem)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::inplace_minus_array);
  
  arrayops::inplace_op_array<eT>(dest, src, n_elem, kernel);
  }



template<typename eT>
inline
void
arrayops::inplace_mul_array(cl_mem dest, cl_mem src, const uword n_elem)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::inplace_mul_array);
  
  arrayops::inplace_op_array<eT>(dest, src, n_elem, kernel);
  }



template<typename eT>
inline
void
arrayops::inplace_div_array(cl_mem dest, cl_mem src, const uword n_elem)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::inplace_div_array);
  
  arrayops::inplace_op_array<eT>(dest, src, n_elem, kernel);
  }



template<typename eT>
inline
eT
arrayops::accumulate(cl_mem src, const uword n_elem)
  {
  // TODO
  }



template<typename eT>
inline
eT
arrayops::product(cl_mem src, const uword n_elem)
  {
  // TODO
  }



//! @}
