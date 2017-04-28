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



struct kernel_id
  {
  enum enum_id
    {
    inplace_set_scalar = 0,
    inplace_plus_scalar,
    inplace_minus_scalar,
    inplace_mul_scalar,
    inplace_div_scalar,
    //
    submat_inplace_set_scalar,
    submat_inplace_plus_scalar,
    submat_inplace_minus_scalar,
    submat_inplace_mul_scalar,
    submat_inplace_div_scalar,
    //
    inplace_plus_array,
    inplace_minus_array,
    inplace_mul_array,
    inplace_div_array,
    //
    submat_inplace_set_mat,
    submat_inplace_plus_mat,
    submat_inplace_minus_mat,
    submat_inplace_schur_mat,
    submat_inplace_div_mat,
    //
    equ_array_plus_scalar,
    equ_array_neg,
    equ_array_minus_scalar_pre,
    equ_array_minus_scalar_post,
    equ_array_mul_scalar,
    equ_array_div_scalar_pre,
    equ_array_div_scalar_post,
    equ_array_square,
    equ_array_sqrt,
    //
    equ_array_plus_array,
    equ_array_minus_array,
    equ_array_mul_array,
    equ_array_div_array,
    //
    inplace_set_eye,
    //
    get_diag,
    trace,
    //
    sum_all,
    sum_colwise,
    sum_rowwise,
    submat_sum_colwise,
    submat_sum_rowwise,
    invalid_kernel
    };
  
  
  static
  inline
  std::vector<std::string>
  init_names()
    {
    // NOTE: the order and names of kernels in "names" must match the order and names in the kernel_id enum
    
    std::vector<std::string> names;
    
    names.push_back("inplace_set_scalar");
    names.push_back("inplace_plus_scalar");
    names.push_back("inplace_minus_scalar");
    names.push_back("inplace_mul_scalar");
    names.push_back("inplace_div_scalar");
    
    names.push_back("submat_inplace_set_scalar");
    names.push_back("submat_inplace_plus_scalar");
    names.push_back("submat_inplace_minus_scalar");
    names.push_back("submat_inplace_mul_scalar");
    names.push_back("submat_inplace_div_scalar");
    
    names.push_back("inplace_plus_array");
    names.push_back("inplace_minus_array");
    names.push_back("inplace_mul_array");
    names.push_back("inplace_div_array");
    
    names.push_back("submat_inplace_set_mat");
    names.push_back("submat_inplace_plus_mat");
    names.push_back("submat_inplace_minus_mat");
    names.push_back("submat_inplace_schur_mat");
    names.push_back("submat_inplace_div_mat");
    
    names.push_back("equ_array_plus_scalar");
    names.push_back("equ_array_neg");
    names.push_back("equ_array_minus_scalar_pre");
    names.push_back("equ_array_minus_scalar_post");
    names.push_back("equ_array_mul_scalar");
    names.push_back("equ_array_div_scalar_pre");
    names.push_back("equ_array_div_scalar_post");
    names.push_back("equ_array_square");
    names.push_back("equ_array_sqrt");
    
    names.push_back("equ_array_plus_array");
    names.push_back("equ_array_minus_array");
    names.push_back("equ_array_mul_array");
    names.push_back("equ_array_div_array");
    
    names.push_back("inplace_set_eye");
    
    names.push_back("get_diag");
    names.push_back("trace");
    
    names.push_back("sum_all");
    names.push_back("sum_colwise");
    names.push_back("sum_rowwise");
    names.push_back("submat_sum_colwise");
    names.push_back("submat_sum_rowwise");
    
    return names;
    }
  
  
  static
  inline
  const std::vector<std::string>&
  get_names()
    {
    static const std::vector<std::string> names = init_names();
    
    return names;
    }
  };
