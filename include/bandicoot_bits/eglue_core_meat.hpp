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


//! \addtogroup eglue_core
//! @{



//
// matrices



template<typename eglue_type>
template<typename T1, typename T2>
inline
void
eglue_core<eglue_type>::apply(Mat<typename T1::elem_type>& out, const eGlue<T1, T2, eglue_type>& x)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  const unwrap<T1> UA(x.A);
  const unwrap<T2> UB(x.B);
  
  const Mat<eT>& A = UA.M;
  const Mat<eT>& B = UB.M;
  
  coot_rt_t::cq_guard guard;
  
  cl_kernel kernel;
  
       if(is_same_type<eglue_type, eglue_plus >::yes)  { kernel = coot_rt.get_kernel<eT>(kernel_id::equ_array_plus_array ); }
  else if(is_same_type<eglue_type, eglue_minus>::yes)  { kernel = coot_rt.get_kernel<eT>(kernel_id::equ_array_minus_array); }
  else if(is_same_type<eglue_type, eglue_div  >::yes)  { kernel = coot_rt.get_kernel<eT>(kernel_id::equ_array_div_array  ); }
  else if(is_same_type<eglue_type, eglue_schur>::yes)  { kernel = coot_rt.get_kernel<eT>(kernel_id::equ_array_mul_array  ); }
  
  cl_mem out_dev_mem = out.get_dev_mem(false);
  cl_mem   A_dev_mem =   A.get_dev_mem(false);
  cl_mem   B_dev_mem =   B.get_dev_mem(false);
  
  uword n_elem = out.get_n_elem();
  
  coot_rt_t::adapt_uword N(n_elem);
  
  cl_int status = 0;
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &out_dev_mem);
  status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &  A_dev_mem);
  status |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &  B_dev_mem);
  status |= clSetKernelArg(kernel, 3, N.size,         N.addr      );
  
  size_t global_work_size = size_t(n_elem);
  
  status |= clEnqueueNDRangeKernel(coot_rt.get_cq(), kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
  
  coot_check_runtime_error( (status != CL_SUCCESS), "eglue_core: couldn't execute kernel" );
  }



template<typename eglue_type>
template<typename T1, typename T2>
inline
void
eglue_core<eglue_type>::apply_inplace_plus(Mat<typename T1::elem_type>& out, const eGlue<T1, T2, eglue_type>& x)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  // TODO: this is currently a "better-than-nothing" solution
  // TODO: replace with code that uses dedicated kernels
  
  const Mat<eT> tmp(x);
  
  out += tmp;
  }



template<typename eglue_type>
template<typename T1, typename T2>
inline
void
eglue_core<eglue_type>::apply_inplace_minus(Mat<typename T1::elem_type>& out, const eGlue<T1, T2, eglue_type>& x)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  // TODO: this is currently a "better-than-nothing" solution
  // TODO: replace with code that uses dedicated kernels
  
  const Mat<eT> tmp(x);
  
  out -= tmp;
  }



template<typename eglue_type>
template<typename T1, typename T2>
inline
void
eglue_core<eglue_type>::apply_inplace_schur(Mat<typename T1::elem_type>& out, const eGlue<T1, T2, eglue_type>& x)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  // TODO: this is currently a "better-than-nothing" solution
  // TODO: replace with code that uses dedicated kernels
  
  const Mat<eT> tmp(x);
  
  out %= tmp;
  }



template<typename eglue_type>
template<typename T1, typename T2>
inline
void
eglue_core<eglue_type>::apply_inplace_div(Mat<typename T1::elem_type>& out, const eGlue<T1, T2, eglue_type>& x)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  // TODO: this is currently a "better-than-nothing" solution
  // TODO: replace with code that uses dedicated kernels
  
  const Mat<eT> tmp(x);
  
  out /= tmp;
  }



//! @}
