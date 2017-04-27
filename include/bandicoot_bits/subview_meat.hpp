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


//! \addtogroup subview
//! @{


template<typename eT>
inline
subview<eT>::~subview()
  {
  coot_extra_debug_sigprint();
  }


template<typename eT>
inline
subview<eT>::subview(const Mat<eT>& in_m, const uword in_row1, const uword in_col1, const uword in_n_rows, const uword in_n_cols)
  : m(in_m)
  , aux_row1(in_row1)
  , aux_col1(in_col1)
  , n_rows(in_n_rows)
  , n_cols(in_n_cols)
  , n_elem(in_n_rows*in_n_cols)
  {
  coot_extra_debug_sigprint();
  }



template<typename eT>
inline
void
subview<eT>::operator= (const subview<eT>& x)
  {
  coot_extra_debug_sigprint();
  
  // TODO: this is currently a "better-than-nothing" solution; replace with code using a dedicated kernel
  
  const Mat<eT> tmp(x);
  
  (*this).operator=(tmp);
  
  
  // if(check_overlap(x))
  //   {
  //   const Mat<eT> tmp(x);
  //   
  //   (*this).operator=(tmp);
  //   }
  // else
  //   {
  //   // TODO: implement kernel to copy from submatrix to submatrix
  //   }
  }



template<typename eT>
inline
void
subview<eT>::inplace_op(const eT val, cl_kernel kernel)
  {
  coot_extra_debug_sigprint();
  
  if(n_elem == 0)  { return; }
  
  coot_runtime_t::queue_guard guard;
  
  const uword end_row = aux_row1 + n_rows - 1;
  const uword end_col = aux_col1 + n_cols - 1;
  
  coot_runtime_t::adapt_val m_end_row(end_row);
  coot_runtime_t::adapt_val m_end_col(end_col);
  
  coot_runtime_t::adapt_val m_n_rows(m.n_rows);
  
  cl_int status = 0;
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem),    &m.device_mem );
  status |= clSetKernelArg(kernel, 1, sizeof(eT),        &val          );
  status |= clSetKernelArg(kernel, 2, m_end_row.size,    m_end_row.addr);
  status |= clSetKernelArg(kernel, 3, m_end_col.size,    m_end_col.addr);
  status |= clSetKernelArg(kernel, 4,  m_n_rows.size,     m_n_rows.addr);
  
  size_t global_work_offset[2] = { size_t(aux_row1), size_t(aux_col1) };  // starting point in parent matrix
  size_t global_work_size[2]   = { size_t(n_rows),   size_t(n_cols)   };  // size of submatrix
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_queue(), kernel, 2, global_work_offset, global_work_size, NULL, 0, NULL, NULL);
  
  coot_check_runtime_error( (status != 0), "subview::inplace_op(): couldn't execute kernel" );
  }



template<typename eT>
inline
void
subview<eT>::operator= (const eT val)
  {
  coot_extra_debug_sigprint();
  
  if(n_elem == 1)
    {
    Mat<eT>& X = const_cast< Mat<eT>& >(m);
    
    X.at(aux_row1, aux_col1) = val;
    }
  else
    {
    coot_debug_assert_same_size(n_rows, n_cols, 1, 1, "subview::operator=");
    }
  }



template<typename eT>
inline
void
subview<eT>::operator+= (const eT val)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::submat_inplace_plus_scalar);
  
  (*this).inplace_op(val, kernel);
  }



template<typename eT>
inline
void
subview<eT>::operator-= (const eT val)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::submat_inplace_minus_scalar);
  
  (*this).inplace_op(val, kernel);
  }



template<typename eT>
inline
void
subview<eT>::operator*= (const eT val)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::submat_inplace_mul_scalar);
  
  (*this).inplace_op(val, kernel);
  }



template<typename eT>
inline
void
subview<eT>::operator/= (const eT val)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::submat_inplace_div_scalar);
  
  (*this).inplace_op(val, kernel);
  }



template<typename eT>
template<typename T1>
inline
void
subview<eT>::inplace_op(const Base<eT,T1>& in, cl_kernel kernel, const char* identifier)
  {
  coot_extra_debug_sigprint();
  
  const unwrap<T1>   U(in.get_ref());
  const Mat<eT>& X = U.M;
  
  coot_assert_same_size(n_rows, n_cols, X.n_rows, X.n_cols, identifier);
  
  if(n_elem == 0)  { return; }
  
  coot_runtime_t::queue_guard guard;
  
  coot_runtime_t::adapt_val start_row(aux_row1);
  coot_runtime_t::adapt_val start_col(aux_col1);
  
  coot_runtime_t::adapt_val m_n_rows(m.n_rows);
  
  coot_runtime_t::adapt_val X_n_rows(X.n_rows);
  coot_runtime_t::adapt_val X_n_cols(X.n_cols);
  
  cl_int status = 0;
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &m.device_mem );
  status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &X.device_mem );
  status |= clSetKernelArg(kernel, 2, start_row.size, start_row.addr);
  status |= clSetKernelArg(kernel, 3, start_col.size, start_col.addr);
  status |= clSetKernelArg(kernel, 4,  m_n_rows.size,  m_n_rows.addr);
  status |= clSetKernelArg(kernel, 5,  X_n_rows.size,  X_n_rows.addr);
  status |= clSetKernelArg(kernel, 6,  X_n_cols.size,  X_n_cols.addr);
  
  size_t global_work_size[2] = { size_t(X.n_rows), size_t(X.n_cols) };
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_queue(), kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
  
  coot_check_runtime_error( (status != 0), "subview::inplace_op(): couldn't execute kernel" );
  }



template<typename eT>
template<typename T1>
inline
void
subview<eT>::operator= (const Base<eT,T1>& in)
  {
  coot_extra_debug_sigprint();
  
  if(false)
    {
    cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::submat_inplace_set_mat);
    
    inplace_op(in, kernel, "subview::operator=");
    }
  else
    {
    const unwrap<T1>   U(in.get_ref());
    const Mat<eT>& X = U.M;
    
    coot_assert_same_size(n_rows, n_cols, X.n_rows, X.n_cols, "subview::operator=");
    
    // if the entire range is selected, use simple copy
    // (beignet 1.3 crashes if clEnqueueCopyBufferRect() is used on entire range)
    if( (n_rows == m.n_rows) && (n_cols == m.n_cols) )
      {
      Mat<eT>& X = const_cast< Mat<eT>& >(m);
      X = in.get_ref();
      return;
      }
    
    size_t src_origin[3] = { 0, 0, 0 };
    size_t dst_origin[3] = { aux_row1*sizeof(eT), aux_col1, 0 };
    
    size_t region[3] = { n_rows*sizeof(eT), n_cols, 1 };
    
    size_t src_row_pitch   = 0;
    size_t src_slice_pitch = 0;
    
    size_t dst_row_pitch   = sizeof(eT) * m.n_rows;
    size_t dst_slice_pitch = sizeof(eT) * m.n_cols * m.n_rows;
    
    cl_int status = clEnqueueCopyBufferRect(coot_runtime.get_queue(), X.device_mem, m.device_mem, src_origin, dst_origin, region, src_row_pitch, src_slice_pitch, dst_row_pitch, dst_slice_pitch, 0, NULL, NULL);
    
    coot_check_runtime_error( (status != 0), "subview::extract: couldn't copy buffer" );
    }
  }



template<typename eT>
template<typename T1>
inline
void
subview<eT>::operator+= (const Base<eT,T1>& in)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::submat_inplace_plus_mat);
  
  inplace_op(in, kernel, "subview::operator+=");
  }



template<typename eT>
template<typename T1>
inline
void
subview<eT>::operator-= (const Base<eT,T1>& in)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::submat_inplace_minus_mat);
  
  inplace_op(in, kernel, "subview::operator-=");
  }



template<typename eT>
template<typename T1>
inline
void
subview<eT>::operator%= (const Base<eT,T1>& in)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::submat_inplace_schur_mat);
  
  inplace_op(in, kernel, "subview::operator%=");
  }



template<typename eT>
template<typename T1>
inline
void
subview<eT>::operator/= (const Base<eT,T1>& in)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::submat_inplace_div_mat);
  
  inplace_op(in, kernel, "subview::operator/=");
  }



template<typename eT>
inline
void
subview<eT>::fill(const eT val)
  {
  coot_extra_debug_sigprint();
  
  cl_kernel kernel = coot_runtime.get_kernel<eT>(kernel_id::submat_inplace_set_scalar);
  
  (*this).inplace_op(val, kernel);
  }



template<typename eT>
inline
void
subview<eT>::zeros()
  {
  coot_extra_debug_sigprint();
  
  (*this).fill(eT(0));
  }



template<typename eT>
inline
void
subview<eT>::ones()
  {
  coot_extra_debug_sigprint();
  
  (*this).fill(eT(1));
  }



template<typename eT>
inline
void
subview<eT>::eye()
  {
  coot_extra_debug_sigprint();
  
  // TODO: this is currently a "better-than-nothing" solution; replace with code using a dedicated kernel
  
  Mat<eT> tmp(n_rows, n_cols);
  tmp.eye();
  
  (*this).operator=(tmp);
  }



template<typename eT>
inline
bool
subview<eT>::check_overlap(const subview<eT>& x) const
  {
  const subview<eT>& s = *this;
  
  if(&s.m != &x.m)
    {
    return false;
    }
  else
    {
    if( (s.n_elem == 0) || (x.n_elem == 0) )
      {
      return false;
      }
    else
      {
      const uword s_row_start  = s.aux_row1;
      const uword s_row_end_p1 = s_row_start + s.n_rows;
      
      const uword s_col_start  = s.aux_col1;
      const uword s_col_end_p1 = s_col_start + s.n_cols;
      
      
      const uword x_row_start  = x.aux_row1;
      const uword x_row_end_p1 = x_row_start + x.n_rows;
      
      const uword x_col_start  = x.aux_col1;
      const uword x_col_end_p1 = x_col_start + x.n_cols;
      
      
      const bool outside_rows = ( (x_row_start >= s_row_end_p1) || (s_row_start >= x_row_end_p1) );
      const bool outside_cols = ( (x_col_start >= s_col_end_p1) || (s_col_start >= x_col_end_p1) );
      
      return ( (outside_rows == false) && (outside_cols == false) );
      }
    }
  }



template<typename eT>
coot_warn_unused
inline
bool
subview<eT>::is_vec() const
  {
  return ( (n_rows == 1) || (n_cols == 1) );
  }



template<typename eT>
coot_warn_unused
inline
bool
subview<eT>::is_colvec() const
  {
  return (n_cols == 1);
  }



template<typename eT>
coot_warn_unused
inline
bool
subview<eT>::is_rowvec() const
  {
  return (n_rows == 1);
  }



template<typename eT>
coot_warn_unused
inline
bool
subview<eT>::is_square() const
  {
  return (n_rows == n_cols);
  }



template<typename eT>
coot_warn_unused
inline
bool
subview<eT>::is_empty() const
  {
  return (n_elem == 0);
  }



//! X = Y.submat(...)
template<typename eT>
inline
void
subview<eT>::extract(Mat<eT>& out, const subview<eT>& in)
  {
  coot_extra_debug_sigprint();
  
  // NOTE: we're assuming that the matrix has already been set to the correct size and there is no aliasing;
  // size setting and alias checking is done by either the Mat contructor or operator=()
  
  coot_extra_debug_print(coot_str::format("out.n_rows = %d   out.n_cols = %d    in.m.n_rows = %d  in.m.n_cols = %d") % out.n_rows % out.n_cols % in.m.n_rows % in.m.n_cols );
  
  if(in.n_elem == 0)  { return; }
  
  // if the entire range is selected, use simple copy
  // (beignet 1.3 crashes if clEnqueueCopyBufferRect() is used on entire range)
  if( (in.n_rows == in.m.n_rows) && (in.n_cols == in.m.n_cols) )
    {
    out = in.m;
    return;
    }
  
  coot_runtime_t::queue_guard guard;
  
  // treat the matrix as an image rotated 90 degrees
  // width  of img = number of rows
  // height of img = number of cols
  
  // whoever designed the API for clEnqueueCopyBufferRect() should be permanently removed from the gene pool;
  // the starting row needs to be multiplied by the element size,
  // because it was too logical to add a separate "size of element" argument
  
  size_t src_origin[3] = { in.aux_row1*sizeof(eT), in.aux_col1, 0 };
  size_t dst_origin[3] = { 0, 0, 0 };
  
  size_t region[3] = { in.n_rows*sizeof(eT), in.n_cols, 1 };
  
  size_t src_row_pitch   = sizeof(eT) * in.m.n_rows;
  size_t src_slice_pitch = sizeof(eT) * in.m.n_cols * in.m.n_rows;
  
  size_t dst_row_pitch   = 0;
  size_t dst_slice_pitch = 0;
  
  cl_int status = clEnqueueCopyBufferRect(coot_runtime.get_queue(), in.m.device_mem, out.device_mem, src_origin, dst_origin, region, src_row_pitch, src_slice_pitch, dst_row_pitch, dst_slice_pitch, 0, NULL, NULL);
  
  coot_check_runtime_error( (status != 0), "subview::extract: couldn't copy buffer" );
  }



//! X += Y.submat(...)
template<typename eT>
inline
void
subview<eT>::plus_inplace(Mat<eT>& out, const subview<eT>& in)
  {
  coot_extra_debug_sigprint();
  
  // TODO: this is currently a "better-than-nothing" solution; replace with code using a dedicated kernel
  
  const Mat<eT> tmp(in);
  
  out += tmp;
  }



//! X -= Y.submat(...)
template<typename eT>
inline
void
subview<eT>::minus_inplace(Mat<eT>& out, const subview<eT>& in)
  {
  coot_extra_debug_sigprint();
  
  // TODO: this is currently a "better-than-nothing" solution; replace with code using a dedicated kernel
  
  const Mat<eT> tmp(in);
  
  out -= tmp;
  }



//! X %= Y.submat(...)
template<typename eT>
inline
void
subview<eT>::schur_inplace(Mat<eT>& out, const subview<eT>& in)
  {
  coot_extra_debug_sigprint();
  
  // TODO: this is currently a "better-than-nothing" solution; replace with code using a dedicated kernel
  
  const Mat<eT> tmp(in);
  
  out %= tmp;
  }



//! X /= Y.submat(...)
template<typename eT>
inline
void
subview<eT>::div_inplace(Mat<eT>& out, const subview<eT>& in)
  {
  coot_extra_debug_sigprint();
  
  // TODO: this is currently a "better-than-nothing" solution; replace with code using a dedicated kernel
  
  const Mat<eT> tmp(in);
  
  out /= tmp;
  }



//
// subview_col


template<typename eT>
inline
subview_col<eT>::subview_col(const Mat<eT>& in_m, const uword in_col)
  : subview<eT>(in_m, 0, in_col, in_m.n_rows, 1)
  {
  coot_extra_debug_sigprint();
  }



template<typename eT>
inline
subview_col<eT>::subview_col(const Mat<eT>& in_m, const uword in_col, const uword in_row1, const uword in_n_rows)
  : subview<eT>(in_m, in_row1, in_col, in_n_rows, 1)
  {
  coot_extra_debug_sigprint();
  }



template<typename eT>
inline
void
subview_col<eT>::operator=(const subview<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  subview<eT>::operator=(X);
  }



template<typename eT>
inline
void
subview_col<eT>::operator=(const subview_col<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  subview<eT>::operator=(X); // interprets 'subview_col' as 'subview'
  }



template<typename eT>
inline
void
subview_col<eT>::operator=(const eT val)
  {
  coot_extra_debug_sigprint();
  
  subview<eT>::operator=(val); // interprets 'subview_col' as 'subview'
  }



template<typename eT>
template<typename T1>
inline
void
subview_col<eT>::operator=(const Base<eT,T1>& X)
  {
  coot_extra_debug_sigprint();
  
  subview<eT>::operator=(X); // interprets 'subview_col' as 'subview'
  }



//
// subview_row


template<typename eT>
inline
subview_row<eT>::subview_row(const Mat<eT>& in_m, const uword in_row)
  : subview<eT>(in_m, in_row, 0, 1, in_m.n_cols)
  {
  coot_extra_debug_sigprint();
  }



template<typename eT>
inline
subview_row<eT>::subview_row(const Mat<eT>& in_m, const uword in_row, const uword in_col1, const uword in_n_cols)
  : subview<eT>(in_m, in_row, in_col1, 1, in_n_cols)
  {
  coot_extra_debug_sigprint();
  }



template<typename eT>
inline
void
subview_row<eT>::operator=(const subview<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  subview<eT>::operator=(X);
  }



template<typename eT>
inline
void
subview_row<eT>::operator=(const subview_row<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  subview<eT>::operator=(X); // interprets 'subview_row' as 'subview'
  }



template<typename eT>
inline
void
subview_row<eT>::operator=(const eT val)
  {
  coot_extra_debug_sigprint();
  
  subview<eT>::operator=(val); // interprets 'subview_row' as 'subview'
  }



template<typename eT>
template<typename T1>
inline
void
subview_row<eT>::operator=(const Base<eT,T1>& X)
  {
  coot_extra_debug_sigprint();
  
  subview<eT>::operator=(X);
  }



//! @}
