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


//! \addtogroup eGlue
//! @{


template<typename T1, typename T2, typename eglue_type>
class eGlue : public Base<typename T1::elem_type, eGlue<T1, T2, eglue_type> >
  {
  public:
  
  typedef typename T1::elem_type                   elem_type;
  typedef typename get_pod_type<elem_type>::result pod_type;
  
  static const bool is_col = (T1::is_col || T2::is_col);
  static const bool is_row = (T1::is_row || T2::is_row);
  
  const T1& A;
  const T2& B;
  
  coot_inline ~eGlue();
  coot_inline  eGlue(const T1& in_A, const T2& in_B);
  
  coot_inline uword get_n_rows() const;
  coot_inline uword get_n_cols() const;
  coot_inline uword get_n_elem() const;
  };



//! @}
