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
inline
fakeMat<eT>::~fakeMat()
  {
  coot_extra_debug_sigprint_this(this);
  
  coot_type_check(( is_supported_elem_type<eT>::value == false ));
  }



template<typename eT>
inline
fakeMat<eT>::fakeMat()
  : n_rows(0)
  , n_cols(0)
  , n_elem(0)
  , mem   (NULL)
  {
  coot_extra_debug_sigprint_this(this);
  }



template<typename eT>
inline
fakeMat<eT>::fakeMat(const Mat<eT>& X)
  : n_rows(0)
  , n_cols(0)
  , n_elem(0)
  , mem   (NULL)
  {
  coot_extra_debug_sigprint_this(this);
  
  init(X.n_rows, X.n_cols);
  
  X.read_device_mem(memptr(), n_elem);
  }



template<typename eT>
inline
fakeMat<eT>::fakeMat(const uword in_n_rows, const uword in_n_cols)
  : n_rows(0)
  , n_cols(0)
  , n_elem(0)
  , mem   (NULL)
  {
  coot_extra_debug_sigprint_this(this);
  
  init(in_n_rows, in_n_cols);
  }



template<typename eT>
inline
void
fakeMat<eT>::init(const uword new_n_rows, const uword new_n_cols)
  {
  coot_extra_debug_sigprint( coot_str::format("new_n_rows = %d, new_n_cols = %d") % new_n_rows % new_n_cols );
  
  if( (n_rows == new_n_rows) && (n_cols == new_n_cols) )  { return; }
  
  // ensure that n_elem can hold the result of (n_rows * n_cols)
  coot_debug_check( ((double(new_n_rows)*double(new_n_cols)) > double(std::numeric_limits<uword>::max())), "fakeMat::init(): requested size is too large" );
  
  const uword old_n_elem = n_elem;
  const uword new_n_elem = new_n_rows*new_n_cols;
  
  if(old_n_elem == new_n_elem)
    {
    coot_extra_debug_print("fakeMat::init(): reusing memory");
    access::rw(n_rows) = new_n_rows;
    access::rw(n_cols) = new_n_cols;
    }
  else  // condition: old_n_elem != new_n_elem
    {
    if(new_n_elem == 0)
      {
      if(old_n_elem > 0)
        {
        coot_extra_debug_print("fakeMat::init(): releasing memory");
        cpu_memory::release(mem);
        }
      
      mem = NULL;
      }
    else
    if(new_n_elem < old_n_elem)
      {
      coot_extra_debug_print("fakeMat::init(): reusing memory");
      }
    else  // condition: new_n_elem > old_n_elem
      {
      if(old_n_elem > 0)
        {
        coot_extra_debug_print("fakeMat::init(): releasing memory");
        cpu_memory::release(mem);
        }
      
      coot_extra_debug_print("fakeMat::init(): acquiring memory");
      mem = cpu_memory::acquire<eT>(new_n_elem);
      }
    
    access::rw(n_rows) = new_n_rows;
    access::rw(n_cols) = new_n_cols;
    access::rw(n_elem) = new_n_elem;
    }
  }



template<typename eT>
inline
void
fakeMat<eT>::set_size(const uword in_n_rows, const uword in_n_cols)
  {
  init(in_n_rows, in_n_cols);
  }



template<typename eT>
coot_inline
bool
fakeMat<eT>::is_empty() const
  {
  return (n_elem == 0);
  }



template<typename eT>
coot_inline
eT*
fakeMat<eT>::memptr()
  {
  return mem;
  }



template<typename eT>
coot_inline
const eT*
fakeMat<eT>::memptr() const
  {
  return mem;
  }



template<typename eT>
coot_inline
coot_warn_unused
eT&
fakeMat<eT>::operator() (const uword ii)
  {
  coot_debug_check( (ii >= n_elem), "fakeMat::operator(): index out of bounds");
  
  return mem[ii];
  }



template<typename eT>
coot_inline
coot_warn_unused
const eT&
fakeMat<eT>::operator() (const uword ii) const
  {
  coot_debug_check( (ii >= n_elem), "fakeMat::operator(): index out of bounds");
  
  return mem[ii];
  }


template<typename eT>
coot_inline
coot_warn_unused
eT&
fakeMat<eT>::operator[] (const uword ii)
  {
  return mem[ii];
  }



template<typename eT>
coot_inline
coot_warn_unused
const eT&
fakeMat<eT>::operator[] (const uword ii) const
  {
  return mem[ii];
  }



template<typename eT>
coot_inline
coot_warn_unused
eT&
fakeMat<eT>::at(const uword ii)
  {
  return mem[ii];
  }



template<typename eT>
coot_inline
coot_warn_unused
const eT&
fakeMat<eT>::at(const uword ii) const
  {
  return mem[ii];
  }



template<typename eT>
coot_inline
coot_warn_unused
eT&
fakeMat<eT>::operator() (const uword in_row, const uword in_col)
  {
  coot_debug_check( ((in_row >= n_rows) || (in_col >= n_cols)), "fakeMat::operator(): index out of bounds");
  
  return mem[in_row + in_col*n_rows];
  }



template<typename eT>
coot_inline
coot_warn_unused
const eT&
fakeMat<eT>::operator() (const uword in_row, const uword in_col) const
  {
  coot_debug_check( ((in_row >= n_rows) || (in_col >= n_cols)), "fakeMat::operator(): index out of bounds");
  
  return mem[in_row + in_col*n_rows];
  }



template<typename eT>
coot_inline
coot_warn_unused
eT&
fakeMat<eT>::at(const uword in_row, const uword in_col)
  {
  return mem[in_row + in_col*n_rows];
  }



template<typename eT>
coot_inline
coot_warn_unused
const eT&
fakeMat<eT>::at(const uword in_row, const uword in_col) const
  {
  return mem[in_row + in_col*n_rows];
  }



//! @}
