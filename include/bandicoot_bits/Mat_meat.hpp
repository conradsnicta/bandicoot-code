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


//! \addtogroup Mat
//! @{


template<typename eT>
inline
Mat<eT>::~Mat()
  {
  coot_extra_debug_sigprint_this(this);
  
  cleanup();
  
  coot_type_check(( is_supported_elem_type<eT>::value == false ));
  }



template<typename eT>
inline
Mat<eT>::Mat()
  : n_rows    (0)
  , n_cols    (0)
  , n_elem    (0)
  , vec_state (0)
  , mem_state (0)
  , device_mem(NULL)
  {
  coot_extra_debug_sigprint_this(this);
  }



//! construct the matrix to have user specified dimensions
template<typename eT>
inline
Mat<eT>::Mat(const uword in_n_rows, const uword in_n_cols)
  : n_rows    (0)
  , n_cols    (0)
  , n_elem    (0)
  , vec_state (0)
  , mem_state (0)
  , device_mem(NULL)
  {
  coot_extra_debug_sigprint_this(this);
  
  init(in_n_rows, in_n_cols);
  }



template<typename eT>
inline
Mat<eT>::Mat(cl_mem aux_device_mem, const uword in_n_rows, const uword in_n_cols)
  : n_rows    (in_n_rows)
  , n_cols    (in_n_cols)
  , n_elem    (in_n_rows*in_n_cols)  // TODO: need to check whether the result fits
  , vec_state (0)
  , mem_state (1)
  , device_mem(aux_device_mem)
  {
  coot_extra_debug_sigprint_this(this);
  }



template<typename eT>
inline
cl_mem
Mat<eT>::get_device_mem() const
  {
  coot_extra_debug_sigprint();
  
  clFinish(coot_runtime.get_queue());  // force synchronisation
  
  return device_mem;
  }



template<typename eT>
inline
void
Mat<eT>::read_device_mem(eT* dest_memptr, const uword N) const
  {
  coot_extra_debug_sigprint();
  
  // TODO: move this function to a separate class, so it can be shared with the Cube class
  
  if( (n_elem == 0) || (N == 0) )  { return; }
  
  coot_runtime_t::queue_guard guard;
  
  const uword n_elem_mod = (std::min)(n_elem, N);
  
  // use a blocking call
  cl_int status = clEnqueueReadBuffer(coot_runtime.get_queue(), device_mem, CL_TRUE, 0, sizeof(eT)*n_elem_mod, dest_memptr, 0, NULL, NULL);
  
  coot_check_runtime_error( (status != CL_SUCCESS), "Mat::read_device_mem(): couldn't read from device memory" );
  }



template<typename eT>
inline
void
Mat<eT>::write_device_mem(const eT* src_memptr, const uword N)
  {
  coot_extra_debug_sigprint();
  
  // TODO: move this function to a separate class, so it can be shared with the Cube class
  
  if( (n_elem == 0) || (N == 0) )  { return; }
  
  coot_runtime_t::queue_guard guard;
  
  const uword n_elem_mod = (std::min)(n_elem, N);
  
  // use a blocking call
  cl_int status = clEnqueueWriteBuffer(coot_runtime.get_queue(), device_mem, CL_TRUE, 0, sizeof(eT)*n_elem_mod, src_memptr, 0, NULL, NULL);
  
  coot_check_runtime_error( (status != CL_SUCCESS), "Mat::read_device_mem(): couldn't write to device memory" );
  }



template<typename eT>
inline
void
Mat<eT>::cleanup()
  {
  coot_extra_debug_sigprint();
  
  if((mem_state == 0) && (n_elem > 0) && (device_mem != NULL))
    {
    coot_runtime.release_memory(device_mem);
    }
  
  device_mem = NULL;  // for paranoia
  }



template<typename eT>
inline
void
Mat<eT>::init(const uword new_n_rows, const uword new_n_cols)
  {
  coot_extra_debug_sigprint( coot_str::format("new_n_rows = %d, new_n_cols = %d") % new_n_rows % new_n_cols );
  
  if( (n_rows == new_n_rows) && (n_cols == new_n_cols) )  { return; }
  
  // TODO: add handling of mem_state == 1  (ie. if memory is external...)
  
  // ensure that n_elem can hold the result of (n_rows * n_cols)
  coot_debug_check( ((double(new_n_rows)*double(new_n_cols)) > double(std::numeric_limits<uword>::max())), "Mat::init(): requested size is too large" );
  
  const uword old_n_elem = n_elem;
  const uword new_n_elem = new_n_rows*new_n_cols;
  
  if(old_n_elem == new_n_elem)
    {
    coot_extra_debug_print("Mat::init(): reusing memory");
    access::rw(n_rows) = new_n_rows;
    access::rw(n_cols) = new_n_cols;
    }
  else  // condition: old_n_elem != new_n_elem
    {
    if(new_n_elem == 0)
      {
      coot_extra_debug_print("Mat::init(): releasing memory");
      cleanup();
      }
    else
    if(new_n_elem < old_n_elem)
      {
      coot_extra_debug_print("Mat::init(): reusing memory");
      }
    else  // condition: new_n_elem > old_n_elem
      {
      if(old_n_elem > 0)
        {
        coot_extra_debug_print("Mat::init(): releasing memory");
        cleanup();
        }
      
      coot_extra_debug_print("Mat::init(): acquiring memory");
      device_mem = coot_runtime.acquire_memory<eT>(new_n_elem);
      }
    
    access::rw(n_rows) = new_n_rows;
    access::rw(n_cols) = new_n_cols;
    access::rw(n_elem) = new_n_elem;
    }
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator=(const eT val)
  {
  coot_extra_debug_sigprint();
  
  set_size(1,1);
  
  arrayops::inplace_set_scalar(device_mem, val, n_elem);
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator+=(const eT val)
  {
  coot_extra_debug_sigprint();
  
  arrayops::inplace_plus_scalar(device_mem, val, n_elem);
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator-=(const eT val)
  {
  coot_extra_debug_sigprint();
  
  arrayops::inplace_minus_scalar(device_mem, val, n_elem);
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator*=(const eT val)
  {
  coot_extra_debug_sigprint();
  
  arrayops::inplace_mul_scalar(device_mem, val, n_elem);
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator/=(const eT val)
  {
  coot_extra_debug_sigprint();
  
  arrayops::inplace_div_scalar(device_mem, val, n_elem);
  
  return *this;
  }



template<typename eT>
inline
Mat<eT>::Mat(const Mat<eT>& X)
  : n_rows   (0)
  , n_cols   (0)
  , n_elem   (0)
  , vec_state(0)
  , mem_state(0)
  {
  coot_extra_debug_sigprint_this(this);
  
  (*this).operator=(X);
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator=(const Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  if(this != &X)
    {
    (*this).set_size(X.n_rows, X.n_cols);
    
    arrayops::copy<eT>(device_mem, X.device_mem, n_elem);
    }
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator+=(const Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_assert_same_size((*this), X, "Mat::operator+=" );
  
  arrayops::inplace_plus_array<eT>(device_mem, X.device_mem, n_elem);
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator-=(const Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_assert_same_size((*this), X, "Mat::operator-=" );
  
  arrayops::inplace_minus_array<eT>(device_mem, X.device_mem, n_elem);
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator*=(const Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  // TODO: matrix multiplication
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator%=(const Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_assert_same_size((*this), X, "Mat::operator%=" );
  
  arrayops::inplace_mul_array<eT>(device_mem, X.device_mem, n_elem);
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator/=(const Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_assert_same_size((*this), X, "Mat::operator/=" );
  
  arrayops::inplace_div_array<eT>(device_mem, X.device_mem, n_elem);
  
  return *this;
  }



#if defined(COOT_USE_CXX11)

template<typename eT>
inline
Mat<eT>::Mat(Mat&& X)
  : n_rows   (0)
  , n_cols   (0)
  , n_elem   (0)
  , vec_state(0)
  , mem_state(0)
  {
  coot_extra_debug_sigprint_this(this);
  
  (*this).steal_mem(X);
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::operator=(Mat<eT>&& X)
  {
  coot_extra_debug_sigprint();
  
  (*this).steal_mem(X);
  
  return *this;
  }

#endif



template<typename eT>
inline
void
Mat<eT>::steal_mem(Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  if(this != &X)
    {
    access::rw(n_rows)     = X.n_rows;
    access::rw(n_cols)     = X.n_cols;
    access::rw(n_elem)     = X.n_elem;
    access::rw(vec_state)  = X.vec_state;
    access::rw(mem_state)  = X.mem_state;
    access::rw(device_mem) = X.device_mem;
    
    access::rw(X.n_rows)     = 0;
    access::rw(X.n_cols)     = 0;
    access::rw(X.n_elem)     = 0;
    access::rw(X.vec_state)  = 0;
    access::rw(X.mem_state)  = 0;
    access::rw(X.device_mem) = NULL;
    }
  }



template<typename eT>
template<typename T1, typename eop_type>
inline
Mat<eT>::Mat(const eOp<T1, eop_type>& X)
  : n_rows   (0)
  , n_cols   (0)
  , n_elem   (0)
  , vec_state(0)
  , mem_state(0)
  {
  coot_extra_debug_sigprint_this(this);
  
  (*this).operator=(X);
  }



template<typename eT>
template<typename T1, typename eop_type>
inline
const Mat<eT>&
Mat<eT>::operator=(const eOp<T1, eop_type>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_type_check(( is_same_type< eT, typename T1::elem_type >::no ));
  
  // eop_core currently uses unwrap to convert submatrices to separate standard matrices,
  // so currently there can't be dangerous aliasing with the out matrix
  
  set_size(X.get_n_rows(), X.get_n_cols());
  
  eop_type::apply(*this, X);
  
  return *this;
  }



template<typename eT>
template<typename T1, typename eop_type>
inline
const Mat<eT>&
Mat<eT>::operator+=(const eOp<T1, eop_type>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_type_check(( is_same_type< eT, typename T1::elem_type >::no ));
  
  coot_assert_same_size(n_rows, n_cols, X.get_n_rows(), X.get_n_cols(), "Mat::operator+=");
  
  eop_type::apply_inplace_plus(*this, X);
  
  return *this;
  }



template<typename eT>
template<typename T1, typename eop_type>
inline
const Mat<eT>&
Mat<eT>::operator-=(const eOp<T1, eop_type>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_type_check(( is_same_type< eT, typename T1::elem_type >::no ));
  
  coot_assert_same_size(n_rows, n_cols, X.get_n_rows(), X.get_n_cols(), "Mat::operator-=");
  
  eop_type::apply_inplace_minus(*this, X);
  
  return *this;
  }



template<typename eT>
template<typename T1, typename eop_type>
inline
const Mat<eT>&
Mat<eT>::operator*=(const eOp<T1, eop_type>& X)
  {
  coot_extra_debug_sigprint();
  
  // TODO: matrix multiplication
  
  return *this;
  }



template<typename eT>
template<typename T1, typename eop_type>
inline
const Mat<eT>&
Mat<eT>::operator%=(const eOp<T1, eop_type>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_type_check(( is_same_type< eT, typename T1::elem_type >::no ));
  
  coot_assert_same_size(n_rows, n_cols, X.get_n_rows(), X.get_n_cols(), "Mat::operator%=");
  
  eop_type::apply_inplace_schur(*this, X);
  
  return *this;
  }



template<typename eT>
template<typename T1, typename eop_type>
inline
const Mat<eT>&
Mat<eT>::operator/=(const eOp<T1, eop_type>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_type_check(( is_same_type< eT, typename T1::elem_type >::no ));
  
  coot_assert_same_size(n_rows, n_cols, X.get_n_rows(), X.get_n_cols(), "Mat::operator/=");
  
  eop_type::apply_inplace_div(*this, X);
  
  return *this;
  }



template<typename eT>
template<typename T1, typename T2, typename eglue_type>
inline
Mat<eT>::Mat(const eGlue<T1, T2, eglue_type>& X)
  : n_rows   (0)
  , n_cols   (0)
  , n_elem   (0)
  , vec_state(0)
  , mem_state(0)
  {
  coot_extra_debug_sigprint_this(this);
  
  (*this).operator=(X);
  }



template<typename eT>
template<typename T1, typename T2, typename eglue_type>
inline
const Mat<eT>&
Mat<eT>::operator=(const eGlue<T1, T2, eglue_type>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_type_check(( is_same_type< eT, typename T1::elem_type >::no ));
  coot_type_check(( is_same_type< eT, typename T2::elem_type >::no ));
  
  // eglue_core currently uses unwrap to convert submatrices to separate standard matrices,
  // so currently there can't be dangerous aliasing with the out matrix
  
  set_size(X.get_n_rows(), X.get_n_cols());
  
  eglue_type::apply(*this, X);
  
  return *this;
  }



template<typename eT>
template<typename T1, typename T2, typename eglue_type>
inline
const Mat<eT>&
Mat<eT>::operator+=(const eGlue<T1, T2, eglue_type>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_type_check(( is_same_type< eT, typename T1::elem_type >::no ));
  coot_type_check(( is_same_type< eT, typename T2::elem_type >::no ));
  
  coot_assert_same_size(n_rows, n_cols, X.get_n_rows(), X.get_n_cols(), "Mat::operator+=");
  
  eglue_type::apply_inplace_plus(*this, X);
  
  return *this;
  }



template<typename eT>
template<typename T1, typename T2, typename eglue_type>
inline
const Mat<eT>&
Mat<eT>::operator-=(const eGlue<T1, T2, eglue_type>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_type_check(( is_same_type< eT, typename T1::elem_type >::no ));
  coot_type_check(( is_same_type< eT, typename T2::elem_type >::no ));
  
  coot_assert_same_size(n_rows, n_cols, X.get_n_rows(), X.get_n_cols(), "Mat::operator-=");
  
  eglue_type::apply_inplace_minus(*this, X);
  
  return *this;
  }



template<typename eT>
template<typename T1, typename T2, typename eglue_type>
inline
const Mat<eT>&
Mat<eT>::operator*=(const eGlue<T1, T2, eglue_type>& X)
  {
  coot_extra_debug_sigprint();
  
  // TODO: matrix multiplication
  
  return *this;
  }



template<typename eT>
template<typename T1, typename T2, typename eglue_type>
inline
const Mat<eT>&
Mat<eT>::operator%=(const eGlue<T1, T2, eglue_type>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_type_check(( is_same_type< eT, typename T1::elem_type >::no ));
  coot_type_check(( is_same_type< eT, typename T2::elem_type >::no ));
  
  coot_assert_same_size(n_rows, n_cols, X.get_n_rows(), X.get_n_cols(), "Mat::operator%=");
  
  eglue_type::apply_inplace_schur(*this, X);
  
  return *this;
  }



template<typename eT>
template<typename T1, typename T2, typename eglue_type>
inline
const Mat<eT>&
Mat<eT>::operator/=(const eGlue<T1, T2, eglue_type>& X)
  {
  coot_extra_debug_sigprint();
  
  coot_type_check(( is_same_type< eT, typename T1::elem_type >::no ));
  coot_type_check(( is_same_type< eT, typename T2::elem_type >::no ));
  
  coot_assert_same_size(n_rows, n_cols, X.get_n_rows(), X.get_n_cols(), "Mat::operator/=");
  
  eglue_type::apply_inplace_div(*this, X);
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::fill(const eT val)
  {
  coot_extra_debug_sigprint();
  
  arrayops::inplace_set_scalar(device_mem, val, n_elem);
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::zeros()
  {
  coot_extra_debug_sigprint();
  
  (*this).fill(eT(0));
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::zeros(const uword new_n_elem)
  {
  coot_extra_debug_sigprint();
  
  (*this).set_size(new_n_elem);
  (*this).fill(eT(0));
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::zeros(const uword new_n_rows, const uword new_n_cols)
  {
  coot_extra_debug_sigprint();
  
  (*this).set_size(new_n_rows, new_n_cols);
  (*this).fill(eT(0));
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::ones()
  {
  coot_extra_debug_sigprint();
  
  (*this).fill(eT(1));
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::ones(const uword new_n_elem)
  {
  coot_extra_debug_sigprint();
  
  (*this).set_size(new_n_elem);
  (*this).fill(eT(1));
  
  return *this;
  }



template<typename eT>
inline
const Mat<eT>&
Mat<eT>::ones(const uword new_n_rows, const uword new_n_cols)
  {
  coot_extra_debug_sigprint();
  
  (*this).set_size(new_n_rows, new_n_cols);
  (*this).fill(eT(1));
  
  return *this;
  }



template<typename eT>
inline
void
Mat<eT>::reset()
  {
  coot_extra_debug_sigprint();
  
  uword new_n_rows = 0;
  uword new_n_cols = 0;
  
  switch(vec_state)
    {
    case  0:                 break;
    case  1: new_n_cols = 1; break;
    case  2: new_n_rows = 1; break;
    default: ;
    }
    
  init(new_n_rows, new_n_cols);
  }



template<typename eT>
inline
void
Mat<eT>::set_size(const uword new_n_elem)
  {
  coot_extra_debug_sigprint();
  
  uword new_n_rows = 0;
  uword new_n_cols = 0;
  
  switch(vec_state)
    {
    case  0: new_n_rows = new_n_elem; new_n_cols = 1;          break;
    case  1: new_n_rows = new_n_elem; new_n_cols = 1;          break;
    case  2: new_n_rows =          1; new_n_cols = new_n_elem; break;
    default: ;
    }
    
  init(new_n_rows, new_n_cols);
  }



template<typename eT>
inline
void
Mat<eT>::set_size(const uword new_n_rows, const uword new_n_cols)
  {
  coot_extra_debug_sigprint();
  
  init(new_n_rows, new_n_cols);
  }



template<typename eT>
inline
void
Mat<eT>::impl_print(const std::string extra_text) const
  {
  coot_extra_debug_sigprint();
  
  if(extra_text.length() != 0)
    {
    const std::streamsize orig_width = COOT_DEFAULT_OSTREAM.width();
    
    COOT_DEFAULT_OSTREAM << extra_text << '\n';
  
    COOT_DEFAULT_OSTREAM.width(orig_width);
    }
  
  fakeMat<eT> tmp(*this);
  
  coot_ostream::print(COOT_DEFAULT_OSTREAM, tmp, true);
  }



template<typename eT>
coot_inline
bool
Mat<eT>::is_vec() const
  {
  return ((n_rows == 1) || (n_cols == 1));
  }



template<typename eT>
coot_inline
bool
Mat<eT>::is_colvec() const
  {
  return (n_cols == 1);
  }



template<typename eT>
coot_inline
bool
Mat<eT>::is_rowvec() const
  {
  return (n_rows == 1);
  }



template<typename eT>
coot_inline
bool
Mat<eT>::is_square() const
  {
  return (n_rows == n_cols);
  }



template<typename eT>
coot_inline
bool
Mat<eT>::is_empty() const
  {
  return (n_elem == uword(0));
  }



template<typename eT>
coot_inline
uword
Mat<eT>::get_n_rows() const
  {
  return n_rows;
  }



template<typename eT>
coot_inline
uword
Mat<eT>::get_n_cols() const
  {
  return n_cols;
  }



template<typename eT>
coot_inline
uword
Mat<eT>::get_n_elem() const
  {
  return n_elem;
  }



//! linear element accessor; this is very slow - do not use it unless absolutely necessary
template<typename eT>
coot_inline
coot_warn_unused
MatValProxy<eT>
Mat<eT>::operator[] (const uword ii)
  {
  return MatValProxy<eT>(*this, ii);
  }



//! linear element accessor without bounds check; this is very slow - do not use it unless absolutely necessary
template<typename eT>
inline
coot_warn_unused
eT
Mat<eT>::operator[] (const uword ii) const
  {
  return MatValProxy<eT>::get_val(*this, ii);
  }



//! linear element accessor without bounds check; this is very slow - do not use it unless absolutely necessary
template<typename eT>
coot_inline
coot_warn_unused
MatValProxy<eT>
Mat<eT>::at(const uword ii)
  {
  return MatValProxy<eT>(*this, ii);
  }



//! linear element accessor without bounds check; this is very slow - do not use it unless absolutely necessary
template<typename eT>
coot_inline
coot_warn_unused
eT
Mat<eT>::at(const uword ii) const
  {
  coot_debug_check( (ii >= n_elem), "Mat::operator(): index out of bounds");
  
  return MatValProxy<eT>::get_val(*this, ii);
  }



//! linear element accessor without bounds check; this is very slow - do not use it unless absolutely necessary
template<typename eT>
coot_inline
coot_warn_unused
MatValProxy<eT>
Mat<eT>::operator() (const uword ii)
  {
  coot_debug_check( (ii >= n_elem), "Mat::operator(): index out of bounds");
 
  return MatValProxy<eT>(*this, ii);
  }



//! linear element accessor; this is very slow - do not use it unless absolutely necessary
template<typename eT>
coot_inline
coot_warn_unused
eT
Mat<eT>::operator() (const uword ii) const
  {
  coot_debug_check( (ii >= n_elem), "Mat::operator(): index out of bounds");
  
  return MatValProxy<eT>::get_val(*this, ii);
  }



template<typename eT>
coot_inline
coot_warn_unused
MatValProxy<eT>
Mat<eT>::at(const uword in_row, const uword in_col)
  {
  coot_debug_check( ((in_row >= n_rows) || (in_col >= n_cols)), "Mat::operator(): index out of bounds");
  
  return MatValProxy<eT>(*this, (in_row + in_col*n_rows));
  }



template<typename eT>
coot_inline
coot_warn_unused
eT
Mat<eT>::at(const uword in_row, const uword in_col) const
  {
  coot_debug_check( ((in_row >= n_rows) || (in_col >= n_cols)), "Mat::operator(): index out of bounds");

  return MatValProxy<eT>::get_val(*this, (in_row + in_col*n_rows));
  }



template<typename eT>
coot_inline
coot_warn_unused
MatValProxy<eT>
Mat<eT>::operator() (const uword in_row, const uword in_col)
  {
  coot_debug_check( ((in_row >= n_rows) || (in_col >= n_cols)), "Mat::operator(): index out of bounds");
  
  return MatValProxy<eT>(*this, (in_row + in_col*n_rows));
  }



template<typename eT>
coot_inline
coot_warn_unused
eT
Mat<eT>::operator() (const uword in_row, const uword in_col) const
  {
  coot_debug_check( ((in_row >= n_rows) || (in_col >= n_cols)), "Mat::operator(): index out of bounds");
  
  return MatValProxy<eT>::get_val(*this, (in_row + in_col*n_rows));
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_submatrix(Mat<eT>& X, const uword start_row, const uword start_col, const uword end_row, const uword end_col)
  {
  coot_extra_debug_sigprint();
  
  // NOTE: need to detect if the entire range is selected;
  // if entire range is selected, clEnqueueCopyBufferRect() crashes when using beignet 1.3.0
  
  const uword sub_n_rows = end_row - start_row + 1;
  const uword sub_n_cols = end_col - start_col + 1;
  
  X.set_size(sub_n_rows, sub_n_cols);
  
  // treat the matrix as an image rotated 90 degrees
  // width  of img = number of rows
  // height of img = number of cols
  
  // whoever designed the API for clEnqueueCopyBufferRect() should be permanently removed from the gene pool
  
  // the starting row needs to be multiplied by the size of each element,
  // because khronos folks were too lazy to add a separate "size of element" argument
  
  size_t src_origin[3] = { start_row*sizeof(eT), start_col, 0 };
  size_t dst_origin[3] = { 0, 0, 0 };
  
  // region = 
  // The (width in bytes, height in rows, depth in slices) of the 2D or 3D rectangle being read or written.
  // For a 2D rectangle copy, the depth value given by region[2] should be 1. The values in region cannot be 0. 
  
  size_t region[3] = { X.n_rows*sizeof(eT), X.n_cols, 1 };
  
  size_t src_row_pitch   = sizeof(eT) * n_rows;          // length of each row in bytes to be used for the memory region associated with src_buffer. If src_row_pitch is 0, src_row_pitch is computed as region[0]. 
  size_t src_slice_pitch = sizeof(eT) * n_cols * n_rows; // length of each 2D slice in bytes to be used for the memory region associated with src_buffer. If src_slice_pitch is 0, src_slice_pitch is computed as region[1] * src_row_pitch. 
  
  //size_t dst_row_pitch   = sizeof(eT) * out.n_cols; // length of each row in bytes to be used for the memory region associated with dst_buffer. If dst_row_pitch is 0, dst_row_pitch is computed as region[0].
  //size_t dst_slice_pitch = sizeof(eT) * out.n_cols * out.n_rows; // length of each 2D slice in bytes to be used for the memory region associated with dst_buffer. If dst_slice_pitch is 0, dst_slice_pitch is computed as region[1] * dst_row_pitch.
  
  size_t dst_row_pitch   = 0;
  size_t dst_slice_pitch = 0;
  
  cl_int status = clEnqueueCopyBufferRect(coot_runtime.get_queue(), (*this).get_device_mem(), X.get_device_mem(), src_origin, dst_origin, region, src_row_pitch, src_slice_pitch, dst_row_pitch, dst_slice_pitch, 0, NULL, NULL);
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  clFinish(coot_runtime.get_queue());
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::set_submatrix(Mat<eT>& X, const uword start_row, const uword start_col)
  {
  coot_extra_debug_sigprint();
  
  // NOTE: need to detect if the entire range is selected;
  // if entire range is selected, clEnqueueCopyBufferRect() crashes when using beignet 1.3.0
  
  // treat the matrix as an image rotated 90 degrees
  // width  of img = number of rows
  // height of img = number of cols
  
  // whoever designed the API for clEnqueueCopyBufferRect() should be permanently removed from the gene pool
  
  // the starting row needs to be multiplied by the size of each element,
  // because khronos folks were too lazy to add a separate "size of element" argument
  
  size_t src_origin[3] = { 0, 0, 0 };
  size_t dst_origin[3] = { start_row*sizeof(eT), start_col, 0 };
  
  // region = 
  // The (width in bytes, height in rows, depth in slices) of the 2D or 3D rectangle being read or written.
  // For a 2D rectangle copy, the depth value given by region[2] should be 1. The values in region cannot be 0. 
  
  size_t region[3] = { X.n_rows*sizeof(eT), X.n_cols, 1 };
  
  size_t src_row_pitch   = 0; // length of each row in bytes to be used for the memory region associated with src_buffer. If src_row_pitch is 0, src_row_pitch is computed as region[0]. 
  size_t src_slice_pitch = 0; // length of each 2D slice in bytes to be used for the memory region associated with src_buffer. If src_slice_pitch is 0, src_slice_pitch is computed as region[1] * src_row_pitch. 
  
  size_t dst_row_pitch   = sizeof(eT) * n_rows;          // length of each row in bytes to be used for the memory region associated with dst_buffer. If dst_row_pitch is 0, dst_row_pitch is computed as region[0].
  size_t dst_slice_pitch = sizeof(eT) * n_cols * n_rows; // length of each 2D slice in bytes to be used for the memory region associated with dst_buffer. If dst_slice_pitch is 0, dst_slice_pitch is computed as region[1] * dst_row_pitch.
  
  cl_int status = clEnqueueCopyBufferRect(coot_runtime.get_queue(), X.get_device_mem(), (*this).get_device_mem(), src_origin, dst_origin, region, src_row_pitch, src_slice_pitch, dst_row_pitch, dst_slice_pitch, 0, NULL, NULL);
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  clFinish(coot_runtime.get_queue());
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_all_1(Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  X.set_size(1, 1);
  
  cl_kernel kernel = coot_runtime.get_kernel<float>(kernel_id::sum_all);
  
  cl_int status = 0;
  
  cl_mem X_mem =       X.get_device_mem();
  cl_mem A_mem = (*this).get_device_mem();
  
  coot_runtime_t::adapt_val start(0);
  coot_runtime_t::adapt_val end  (n_elem-1);
  
  status = clSetKernelArg(kernel, 0, sizeof(cl_mem),   &X_mem);
  status = clSetKernelArg(kernel, 1, sizeof(cl_mem),   &A_mem);
  status = clSetKernelArg(kernel, 2, start.size,       start.addr);
  status = clSetKernelArg(kernel, 3, start.size,         end.addr);
  
  coot_check_cl_error(status, "get_sum_all_1(): clSetKernelArg()");
  
  const size_t  work_dim = 1;
  const size_t  global_work_offset[1] = { 0 };
  const size_t  global_work_size[1]   = { 1 };
  const size_t* local_work_size = NULL;
  
  status = clEnqueueNDRangeKernel(coot_runtime.get_queue(), kernel, work_dim, global_work_offset, global_work_size, local_work_size, 0, NULL, NULL);
  
  coot_check_cl_error(status, "get_sum_all_1(): clEnqueueNDRangeKernel()");
  
  clFinish(coot_runtime.get_queue());
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_all_2(Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  // (*this).print("blah:");
  
  Mat<eT> tmp;
  (*this).get_sum_colwise(tmp);
  
  // tmp.print("tmp:");
  
  tmp.get_sum_all_1(X);
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_all_3(Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  const cl_uint n_units = coot_runtime.get_n_compunits();
  
  // create a fake matrix which is not so thin or thick
  
  // examine integers between 2 and sqrt(N);
  // smallest factor must lie between those two;
  // largest factor is simply N / the smallest factor.
  
  // First thought you can find the smallest divisor d (not equal to 1 of course),
  // then N/d will be the largest divisor you're looking for.
  // For example if N is divisible by 3 then you'll need 2 iterations to find the answer
  //
  // To further improve your algorithm you can iterate through odd numbers only
  // (after checking if you number is even) or, even better,
  // if you have the list of primes pre-calculated then you can iterate
  // through them only because smallest divisor is obviously is a prime number.

  uword fake_n_rows = n_rows;
  uword fake_n_cols = n_cols;
  
  if( (n_rows > 64) && (n_cols < n_units) )
    {
    if((n_rows % 2) == 0)
      {
      fake_n_rows = n_rows / 2;
      fake_n_cols = n_cols * 2;
      }
    else
      {
      // can use precalculated primes instead of going through every odd value
      const uword sqrt_n_rows = uword(std::floor(std::sqrt(double(n_rows))));
      
      uword divisor = 0;
      
      for(uword i=3; i <= sqrt_n_rows; i+=2)
        {
        if((n_rows % i) == 0)  { divisor = i; break; }
        }
      
      if(divisor != 0)
        {
        fake_n_rows = n_rows / divisor;
        fake_n_cols = n_cols * divisor;
        }
      }
    }
  else
  if( (n_rows <= 64) && (n_cols >= n_units) )
    {
    if((n_cols % 2) == 0)
      {
      fake_n_rows = n_rows * 2;
      fake_n_cols = n_cols / 2;
      }
    else
      {
      // can use precalculated primes instead of going through every odd value
      const uword sqrt_n_cols = uword(std::floor(std::sqrt(double(n_cols))));
      
      uword divisor = 0;
      
      for(uword i=3; i <= sqrt_n_cols; i+=2)
        {
        if((n_cols % i) == 0)  { divisor = i; break; }
        }
      
      if(divisor != 0)
        {
        fake_n_rows = n_rows * divisor;
        fake_n_cols = n_cols / divisor;
        }
      }
    }
  
  
  // make sure there is enough to process in each column
  if(fake_n_rows <= 64)
    {
    fake_n_rows = n_rows;
    fake_n_cols = n_cols;
    }
  
  
  // sanity check
  
  if( (fake_n_rows * fake_n_cols) != (n_rows * n_cols) )
    {
    cout << "sanity check failed: orig: " << n_rows << "x" << n_cols << "  proposed: " << fake_n_rows << "x" << fake_n_cols << endl;
    fake_n_rows = n_rows;
    fake_n_cols = n_cols;
    }
  
  Mat<eT> fake(get_device_mem(), fake_n_rows, fake_n_cols);
  
  Mat<eT> tmp;
  fake.get_sum_colwise(tmp);
  
  tmp.get_sum_all_1(X);
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_colwise(Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  X.set_size(1, (*this).n_cols);
  
  cl_kernel kernel = coot_runtime.get_kernel<float>(kernel_id::sum_colwise);
  
  cl_int status = 0;
  
  cl_mem X_mem =       X.get_device_mem();
  cl_mem A_mem = (*this).get_device_mem();
  
  coot_runtime_t::adapt_val local_n_rows(n_rows);
  coot_runtime_t::adapt_val local_n_cols(n_cols);
  
  status = clSetKernelArg(kernel, 0, sizeof(cl_mem),    &X_mem       );
  status = clSetKernelArg(kernel, 1, sizeof(cl_mem),    &A_mem       );
  status = clSetKernelArg(kernel, 2, local_n_rows.size, local_n_rows.addr);
  status = clSetKernelArg(kernel, 3, local_n_cols.size, local_n_cols.addr);

  coot_check_cl_error(status, "get_sum_colwise(): clSetKernelArg()");
  
  const size_t  work_dim = 1;
  const size_t  global_work_offset[1] = { 0      };
  const size_t  global_work_size[1]   = { n_cols };
  const size_t* local_work_size = NULL;
  //const size_t local_work_size[1] = { 512 };
  // local_work_size = number of work-items in a work-group, in each dimension
  // a work-group is a combination of work-items;
  // each work-group executes on a single compute-unit;
  // each compute-unit executes only one work-group at a time
  
  // beignet 1.3, on HD Graphics 5500 BroadWell U-Processor GT2
  // Max work item sizes  512x512x512
  // Max work group size  512
  
  status = clEnqueueNDRangeKernel(coot_runtime.get_queue(), kernel, work_dim, global_work_offset, global_work_size, local_work_size, 0, NULL, NULL);
  
  coot_check_cl_error(status, "get_sum_colwise(): clEnqueueNDRangeKernel()");
  
  clFinish(coot_runtime.get_queue());
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_rowwise(Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  X.set_size((*this).n_rows, 1);
  
  cl_kernel kernel = coot_runtime.get_kernel<float>(kernel_id::sum_rowwise);
  
  cl_int status = 0;
  
  cl_mem X_mem =       X.get_device_mem();
  cl_mem A_mem = (*this).get_device_mem();
  
  coot_runtime_t::adapt_val local_n_rows(n_rows);
  coot_runtime_t::adapt_val local_n_cols(n_cols);
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem),       &X_mem       );
  status |= clSetKernelArg(kernel, 1, sizeof(cl_mem),       &A_mem       );
  status = clSetKernelArg(kernel, 2, local_n_rows.size, local_n_rows.addr);
  status = clSetKernelArg(kernel, 3, local_n_cols.size, local_n_cols.addr);

  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  const size_t  work_dim = 1;
  const size_t  global_work_offset[1] = { 0      };
  const size_t  global_work_size[1]   = { n_rows };
  const size_t* local_work_size = NULL;
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_queue(), kernel, work_dim, global_work_offset, global_work_size, local_work_size, 0, NULL, NULL);
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  clFinish(coot_runtime.get_queue());
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_submat_colwise(Mat<eT>& X, const uword start_row, const uword start_col, const uword end_row, const uword end_col)
  {
  coot_extra_debug_sigprint();
  
  const uword sub_n_rows = end_row - start_row + 1;
  const uword sub_n_cols = end_col - start_col + 1;

  X.set_size(1, sub_n_cols);
  X.fill(-1);
  
  cl_kernel kernel = coot_runtime.get_kernel<float>(kernel_id::submat_sum_colwise);
  
  cl_int status = 0;
  
  cl_mem X_mem =       X.get_device_mem();
  cl_mem A_mem = (*this).get_device_mem();
  
  unsigned int local_n_rows = n_rows;
  unsigned int local_n_cols = n_cols;
  
  unsigned int local_start_col = start_col;
  unsigned int local_start_row = start_row;
  unsigned int local_end_row   = end_row;
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem),       &X_mem          );
  status |= clSetKernelArg(kernel, 1, sizeof(cl_mem),       &A_mem          );
  status |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &local_n_rows   );
  status |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &local_n_cols   );
  status |= clSetKernelArg(kernel, 4, sizeof(unsigned int), &local_start_col);
  status |= clSetKernelArg(kernel, 5, sizeof(unsigned int), &local_start_row);
  status |= clSetKernelArg(kernel, 6, sizeof(unsigned int), &local_end_row  );
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  const size_t  work_dim = 1;
  const size_t  global_work_offset[1] = { start_col  };  // starting column in original matrix
  const size_t  global_work_size[1]   = { sub_n_cols };  // number of columns in the submatrix
  const size_t* local_work_size = NULL;
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_queue(), kernel, work_dim, global_work_offset, global_work_size, local_work_size, 0, NULL, NULL);
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  clFinish(coot_runtime.get_queue());
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_submat_rowwise(Mat<eT>& X, const uword start_row, const uword start_col, const uword end_row, const uword end_col)
  {
  coot_extra_debug_sigprint();
  
  const uword sub_n_rows = end_row - start_row + 1;
  const uword sub_n_cols = end_col - start_col + 1;

  X.set_size(sub_n_rows, 1);
  
  cl_kernel kernel = coot_runtime.get_kernel<float>(kernel_id::submat_sum_rowwise);
  
  cl_int status = 0;
  
  cl_mem X_mem =       X.get_device_mem();
  cl_mem A_mem = (*this).get_device_mem();
  
  unsigned int local_n_rows = n_rows;
  unsigned int local_n_cols = n_cols;
  
  unsigned int local_start_row = start_row;
  unsigned int local_start_col = start_col;
  unsigned int local_end_col   = end_col;
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem),       &X_mem          );
  status |= clSetKernelArg(kernel, 1, sizeof(cl_mem),       &A_mem          );
  status |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &local_n_rows   );
  status |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &local_n_cols   );
  status |= clSetKernelArg(kernel, 4, sizeof(unsigned int), &local_start_row);
  status |= clSetKernelArg(kernel, 5, sizeof(unsigned int), &local_start_col);
  status |= clSetKernelArg(kernel, 6, sizeof(unsigned int), &local_end_col  );
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  const size_t  work_dim = 1;
  const size_t  global_work_offset[1] = { start_row  };
  const size_t  global_work_size[1]   = { sub_n_rows };
  const size_t* local_work_size = NULL;
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_queue(), kernel, work_dim, global_work_offset, global_work_size, local_work_size, 0, NULL, NULL);
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  clFinish(coot_runtime.get_queue());
  }



#ifdef COOT_EXTRA_MAT_MEAT
  #include COOT_INCFILE_WRAP(COOT_EXTRA_MAT_MEAT)
#endif



//! @}
