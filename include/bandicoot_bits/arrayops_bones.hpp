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


class arrayops
  {
  public:
  
  template<typename eT>
  inline static void
  copy(cl_mem dest, cl_mem src, const uword n_elem);
  
  
  // 
  // array op= scalar
  
  template<typename eT>
  inline static void
  inplace_op_scalar(cl_mem dest, const eT val, const uword n_elem, cl_kernel kernel);
  
  template<typename eT>
  inline static void
  inplace_set_scalar(cl_mem dest, const eT val, const uword n_elem);
  
  template<typename eT>
  inline static void
  inplace_plus_scalar(cl_mem dest, const eT val, const uword n_elem);
  
  template<typename eT>
  inline static void
  inplace_minus_scalar(cl_mem dest, const eT val, const uword n_elem);
  
  template<typename eT>
  inline static void
  inplace_mul_scalar(cl_mem dest, const eT val, const uword n_elem);
  
  template<typename eT>
  inline static void
  inplace_div_scalar(cl_mem dest, const eT val, const uword n_elem);
  
  
  // 
  // array op= array
  
  template<typename eT>
  inline static void
  inplace_op_array(cl_mem dest, cl_mem src, const uword n_elem, cl_kernel kernel);
  
  template<typename eT>
  inline static void
  inplace_plus_array(cl_mem dest, cl_mem src, const uword n_elem);
  
  template<typename eT>
  inline static void
  inplace_minus_array(cl_mem dest, cl_mem src, const uword n_elem);
  
  template<typename eT>
  inline static void
  inplace_mul_array(cl_mem dest, cl_mem src, const uword n_elem);
   
  template<typename eT>
  inline static void
  inplace_div_array(cl_mem dest, cl_mem src, const uword n_elem);
  
  
  // 
  // scalar = op(array)
  
  template<typename eT>
  inline static
  eT
  accumulate(cl_mem src, const uword n_elem);
  
  template<typename eT>
  inline static
  eT
  product(cl_mem src, const uword n_elem);
  };



//! @}
