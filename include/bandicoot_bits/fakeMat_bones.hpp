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


//! \addtogroup fakeMat
//! @{



template<typename eT>
class fakeMat
  {
  public:
  
  typedef eT                                elem_type;  //!< the type of elements stored in the matrix
  typedef typename get_pod_type<eT>::result  pod_type;  //!< if eT is std::complex<T>, pod_type is T; otherwise pod_type is eT
  
  coot_aligned const uword n_rows;    //!< number of rows     (read-only)
  coot_aligned const uword n_cols;    //!< number of columns  (read-only)
  coot_aligned const uword n_elem;    //!< number of elements (read-only)
  
  private:
  
  coot_aligned  eT* mem;
  
  
  public:
  
  inline ~fakeMat();
  inline  fakeMat();
  
  inline fakeMat(const Mat<eT>& X);
  inline fakeMat(const uword new_n_rows, const uword new_n_cols);
  
  inline void set_size(const uword new_n_rows, const uword new_n_cols);
  
  coot_inline bool is_empty() const;
  
  coot_inline       eT* memptr();
  coot_inline const eT* memptr() const;
  
  coot_inline coot_warn_unused       eT& operator[] (const uword ii);
  coot_inline coot_warn_unused const eT& operator[] (const uword ii) const;
  coot_inline coot_warn_unused       eT& at         (const uword ii);
  coot_inline coot_warn_unused const eT& at         (const uword ii) const;
  coot_inline coot_warn_unused       eT& operator() (const uword ii);
  coot_inline coot_warn_unused const eT& operator() (const uword ii) const;
  
  coot_inline coot_warn_unused       eT& at         (const uword in_row, const uword in_col);
  coot_inline coot_warn_unused const eT& at         (const uword in_row, const uword in_col) const;
  coot_inline coot_warn_unused       eT& operator() (const uword in_row, const uword in_col);
  coot_inline coot_warn_unused const eT& operator() (const uword in_row, const uword in_col) const;
  
  
  private:
  
  inline void init(const uword new_n_rows, const uword new_n_cols);
  };



//! @}
