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


//! \addtogroup op_sum
//! @{



template<typename T1>
inline
void
op_sum::apply(Mat<typename T1::elem_type>& out, const Op<T1,op_sum>& in)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  const uword dim = in.aux_uword_a;
  
  coot_debug_check( (dim > 1), "sum(): parameter 'dim' must be 0 or 1" );
  
  const unwrap<T1> U(in.m);
  
  if(U.is_alias(out) == false)
    {
    op_sum::apply_noalias(out, U.M, dim);
    }
  else
    {
    Mat<eT> tmp;
    
    op_sum::apply_noalias(tmp, U.M, dim);
    
    out.steal_mem(tmp);
    }
  }



template<typename eT>
inline
void
op_sum::apply(Mat<eT>& out, const Op<subview<eT>,op_sum>& in)
  {
  coot_extra_debug_sigprint();
  
  const uword dim = in.aux_uword_a;
  
  coot_debug_check( (dim > 1), "sum(): parameter 'dim' must be 0 or 1" );
  
  if(&out != &(in.m.m))
    {
    op_sum::apply_noalias(out, in.m, dim);
    }
  else
    {
    Mat<eT> tmp;
    
    op_sum::apply_noalias(tmp, in.m, dim);
    
    out.steal_mem(tmp);
    }
  }



template<typename eT>
inline
void
op_sum::apply_noalias(Mat<eT>& out, const Mat<eT>& A, const uword dim)
  {
  coot_extra_debug_sigprint();
  
  coot_debug_check( (coot_rt.is_valid() == false), "coot_rt not valid" );
  
  if(dim == 0)
    {
    out.set_size(1, A.n_cols);
    }
  else
  if(dim == 1)
    {
    out.set_size(A.n_rows, 1);
    }
  
  if(A.n_elem == 0)
    {
    out.zeros();
    return;
    }
  
  
  coot_rt_t::cq_guard guard;
  
  if(dim == 0)
    {
    cl_kernel k1 = coot_rt.get_kernel<eT>(kernel_id::sum_colwise);
    
    cl_int status = 0;
    
    cl_mem out_mem = out.get_dev_mem(false);
    cl_mem   A_mem =   A.get_dev_mem(false);
    
    coot_rt_t::adapt_uword A_n_rows(A.n_rows);
    coot_rt_t::adapt_uword A_n_cols(A.n_cols);
    
    status |= clSetKernelArg(k1, 0, sizeof(cl_mem), &out_mem     );
    status |= clSetKernelArg(k1, 1, sizeof(cl_mem), &A_mem       );
    status |= clSetKernelArg(k1, 2,  A_n_rows.size, A_n_rows.addr);
    status |= clSetKernelArg(k1, 3,  A_n_cols.size, A_n_cols.addr);
    
    const size_t k1_work_dim       = 1;
    const size_t k1_work_offset[1] = { 0                };
    const size_t k1_work_size[1]   = { size_t(A.n_cols) };
    
    status |= clEnqueueNDRangeKernel(coot_rt.get_cq(), k1, k1_work_dim, k1_work_offset, k1_work_size, NULL, 0, NULL, NULL);
    
    coot_check_cl_error(status, "sum()");
    }
  else
  if(dim == 1)
    {
    cl_kernel k1 = coot_rt.get_kernel<eT>(kernel_id::sum_rowwise);
    
    cl_int status = 0;
    
    cl_mem out_mem = out.get_dev_mem(false);
    cl_mem   A_mem =   A.get_dev_mem(false);
    
    coot_rt_t::adapt_uword A_n_rows(A.n_rows);
    coot_rt_t::adapt_uword A_n_cols(A.n_cols);
    
    status |= clSetKernelArg(k1, 0, sizeof(cl_mem), &out_mem     );
    status |= clSetKernelArg(k1, 1, sizeof(cl_mem), &A_mem       );
    status |= clSetKernelArg(k1, 2,  A_n_rows.size, A_n_rows.addr);
    status |= clSetKernelArg(k1, 3,  A_n_cols.size, A_n_cols.addr);
    
    const size_t k1_work_dim       = 1;
    const size_t k1_work_offset[1] = { 0                };
    const size_t k1_work_size[1]   = { size_t(A.n_rows) };
    
    status |= clEnqueueNDRangeKernel(coot_rt.get_cq(), k1, k1_work_dim, k1_work_offset, k1_work_size, NULL, 0, NULL, NULL);
    
    coot_check_cl_error(status, "sum()");
    }
  }



template<typename eT>
inline
void
op_sum::apply_noalias(Mat<eT>& out, const subview<eT>& sv, const uword dim)
  {
  coot_extra_debug_sigprint();
  
  coot_debug_check( (coot_rt.is_valid() == false), "coot_rt not valid" );
  
  if(dim == 0)
    {
    out.set_size(1, sv.n_cols);
    }
  else
  if(dim == 1)
    {
    out.set_size(sv.n_rows, 1);
    }
  
  if(sv.n_elem == 0)
    {
    out.zeros();
    return;
    }
  
  
  coot_rt_t::cq_guard guard;
  
  if(dim == 0)
    {
    cl_kernel k1 = coot_rt.get_kernel<eT>(kernel_id::submat_sum_colwise);
    
    cl_int status = 0;
    
    cl_mem  out_mem =  out.get_dev_mem(false);
    cl_mem sv_m_mem = sv.m.get_dev_mem(false);
    
    coot_rt_t::adapt_uword sv_m_n_rows(sv.m.n_rows);
    
    coot_rt_t::adapt_uword start_row(sv.aux_row1);
    coot_rt_t::adapt_uword start_col(sv.aux_col1);
    
    coot_rt_t::adapt_uword sub_n_rows(sv.n_rows);
    coot_rt_t::adapt_uword sub_n_cols(sv.n_cols);
    
    status |= clSetKernelArg(k1, 0,   sizeof(cl_mem), &out_mem        );
    status |= clSetKernelArg(k1, 1,   sizeof(cl_mem), &sv_m_mem       );
    status |= clSetKernelArg(k1, 2, sv_m_n_rows.size, sv_m_n_rows.addr);
    status |= clSetKernelArg(k1, 3,   start_row.size,   start_row.addr);
    status |= clSetKernelArg(k1, 4,   start_col.size,   start_col.addr);
    status |= clSetKernelArg(k1, 5,  sub_n_rows.size,  sub_n_rows.addr);
    status |= clSetKernelArg(k1, 6,  sub_n_cols.size,  sub_n_cols.addr);
    
    const size_t k1_work_dim       = 1;
    const size_t k1_work_offset[1] = { 0                 };
    const size_t k1_work_size[1]   = { size_t(sv.n_cols) };
    
    status |= clEnqueueNDRangeKernel(coot_rt.get_cq(), k1, k1_work_dim, k1_work_offset, k1_work_size, NULL, 0, NULL, NULL);
    
    coot_check_cl_error(status, "sum()");
    }
  else
  if(dim == 1)
    {
    cl_kernel k1 = coot_rt.get_kernel<eT>(kernel_id::submat_sum_rowwise);
    
    cl_int status = 0;
    
    cl_mem  out_mem =  out.get_dev_mem(false);
    cl_mem sv_m_mem = sv.m.get_dev_mem(false);
    
    coot_rt_t::adapt_uword sv_m_n_rows(sv.m.n_rows);
    
    coot_rt_t::adapt_uword start_row(sv.aux_row1);
    coot_rt_t::adapt_uword start_col(sv.aux_col1);
    
    coot_rt_t::adapt_uword sub_n_rows(sv.n_rows);
    coot_rt_t::adapt_uword sub_n_cols(sv.n_cols);
    
    status |= clSetKernelArg(k1, 0,   sizeof(cl_mem), &out_mem        );
    status |= clSetKernelArg(k1, 1,   sizeof(cl_mem), &sv_m_mem       );
    status |= clSetKernelArg(k1, 2, sv_m_n_rows.size, sv_m_n_rows.addr);
    status |= clSetKernelArg(k1, 3,   start_row.size,   start_row.addr);
    status |= clSetKernelArg(k1, 4,   start_col.size,   start_col.addr);
    status |= clSetKernelArg(k1, 5,  sub_n_rows.size,  sub_n_rows.addr);
    status |= clSetKernelArg(k1, 6,  sub_n_cols.size,  sub_n_cols.addr);
    
    const size_t k1_work_dim       = 1;
    const size_t k1_work_offset[1] = { 0                 };
    const size_t k1_work_size[1]   = { size_t(sv.n_rows) };
    
    status |= clEnqueueNDRangeKernel(coot_rt.get_cq(), k1, k1_work_dim, k1_work_offset, k1_work_size, NULL, 0, NULL, NULL);
    
    coot_check_cl_error(status, "sum()");
    }
  }



//! @}
