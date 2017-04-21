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
coot_inline
eGlue<T1,T2,eglue_type>::~eGlue()
  {
  coot_extra_debug_sigprint();
  }



template<typename T1, typename T2, typename eglue_type>
coot_inline
eGlue<T1,T2,eglue_type>::eGlue(const T1& in_A, const T2& in_B)
  : A(in_A)
  , B(in_B)
  {
  coot_extra_debug_sigprint();
  
  coot_debug_assert_same_size
    (
    A.get_n_rows(), A.get_n_cols(),
    B.get_n_rows(), B.get_n_cols(),
    eglue_type::text()
    );
  }



template<typename T1, typename T2, typename eglue_type>
coot_inline
uword
eGlue<T1,T2,eglue_type>::get_n_rows() const
  {
  return is_row ? 1 : A.get_n_rows();
  }



template<typename T1, typename T2, typename eglue_type>
coot_inline
uword
eGlue<T1,T2,eglue_type>::get_n_cols() const
  {
  return is_col ? 1 : A.get_n_cols();
  }



template<typename T1, typename T2, typename eglue_type>
coot_inline
uword
eGlue<T1,T2,eglue_type>::get_n_elem() const
  {
  return A.get_n_elem();
  }



//! @}
