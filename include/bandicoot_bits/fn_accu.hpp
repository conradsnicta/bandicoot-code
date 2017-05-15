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


//! \addtogroup fn_accu
//! @{


template<typename T1>
coot_warn_unused
inline
typename T1::elem_type
accu(const Base<typename T1::elem_type, T1>& X)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  coot_debug_check( (coot_runtime.is_valid() == false), "coot_runtime not valid" );
  
  const unwrap<T1>   U(X.get_ref());
  const Mat<eT>& A = U.M;
  
  if(A.n_elem == 0)  { return eT(0); }
  
  // work out number of chunks
  // make sure there are at least 4 elements per compunit
  
  uword n_chunks = coot_runtime.get_n_units();
  
  while(n_chunks >= 1)
    {
    if( (A.n_elem / n_chunks) >= uword(4) )  { break; }
    
    n_chunks /= uword(2);
    }
  
  n_chunks = (std::max)(uword(1), n_chunks);
  
  const uword chunk_size = A.n_elem / n_chunks;
  
  Mat<eT> tmp(n_chunks, 1);
  
  coot_runtime_t::cq_guard guard;
  
  cl_int status = 0;
  
  cl_kernel k1 = coot_runtime.get_kernel<eT>(kernel_id::accu_chunked);
  
  cl_mem tmp_mem = tmp.get_device_mem(false);
  cl_mem   A_mem =   A.get_device_mem(false);
  
  coot_runtime_t::adapt_uword dev_chunk_size(chunk_size);
  coot_runtime_t::adapt_uword dev_n_chunks  (n_chunks  );
  
  status = clSetKernelArg(k1, 0, sizeof(cl_mem),      &tmp_mem           );
  status = clSetKernelArg(k1, 1, sizeof(cl_mem),      &  A_mem           );
  status = clSetKernelArg(k1, 2, dev_chunk_size.size, dev_chunk_size.addr);
  status = clSetKernelArg(k1, 3, dev_n_chunks.size,   dev_n_chunks.addr  );
  
  const size_t k1_work_dim       = 1;
  const size_t k1_work_offset[1] = { 0        };
  const size_t k1_work_size[1]   = { n_chunks };
  
  status = clEnqueueNDRangeKernel(coot_runtime.get_cq(), k1, k1_work_dim, k1_work_offset, k1_work_size, NULL, 0, NULL, NULL);
  
  coot_check_cl_error(status, "accu()");
  
  // process any leftovers and combine with chunk sums
  
  clFlush(coot_runtime.get_cq());
  
  cl_kernel k2 = coot_runtime.get_kernel<eT>(kernel_id::accu_twostage);
  
  coot_runtime_t::adapt_uword dev_out_len(tmp.n_elem);
  coot_runtime_t::adapt_uword dev_A_start(n_chunks*chunk_size);
  coot_runtime_t::adapt_uword dev_A_len  (A.n_elem);
  
  status = clSetKernelArg(k2, 0, sizeof(cl_mem),   &tmp_mem        );
  status = clSetKernelArg(k2, 1, dev_out_len.size, dev_out_len.addr);
  status = clSetKernelArg(k2, 2, sizeof(cl_mem),   &A_mem          );
  status = clSetKernelArg(k2, 3, dev_A_start.size, dev_A_start.addr);
  status = clSetKernelArg(k2, 4, dev_A_len.size,   dev_A_len.addr  );
  
  const size_t k2_work_dim       = 1;
  const size_t k2_work_offset[1] = { 0 };
  const size_t k2_work_size[1]   = { 1 };
  
  status = clEnqueueNDRangeKernel(coot_runtime.get_cq(), k2, k2_work_dim, k2_work_offset, k2_work_size, NULL, 0, NULL, NULL);
  
  coot_check_cl_error(status, "accu()");
  
  return tmp(0);
  }



template<typename T1>
coot_warn_unused
inline
typename T1::elem_type
accu_simple(const Base<typename T1::elem_type, T1>& X)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  coot_debug_check( (coot_runtime.is_valid() == false), "coot_runtime not valid" );
  
  const unwrap<T1>   U(X.get_ref());
  const Mat<eT>& A = U.M;
  
  if(A.n_elem == 0)  { return eT(0); }
  
  Mat<eT> tmp(1, 1);
  
  coot_runtime_t::cq_guard guard;
  
  cl_int status = 0;
  
  cl_kernel k1 = coot_runtime.get_kernel<eT>(kernel_id::accu_simple);
  
  cl_mem tmp_mem = tmp.get_device_mem(false);
  cl_mem   A_mem =   A.get_device_mem(false);
  
  coot_runtime_t::adapt_uword dev_A_len(A.n_elem);
  
  status = clSetKernelArg(k1, 0, sizeof(cl_mem), &tmp_mem      );
  status = clSetKernelArg(k1, 1, sizeof(cl_mem), &  A_mem      );
  status = clSetKernelArg(k1, 2, dev_A_len.size, dev_A_len.addr);
  
  const size_t k1_work_dim       = 1;
  const size_t k1_work_offset[1] = { 0 };
  const size_t k1_work_size[1]   = { 1 };
  
  status = clEnqueueNDRangeKernel(coot_runtime.get_cq(), k1, k1_work_dim, k1_work_offset, k1_work_size, NULL, 0, NULL, NULL);
  
  coot_check_cl_error(status, "accu()");
  
  return tmp(0);
  }



template<typename eT>
coot_warn_unused
inline
eT
accu(const subview<eT>& S)
  {
  coot_extra_debug_sigprint();
  
  if(S.n_elem == 0)  { return eT(0); }
  
  Mat<eT> tmp(1, S.n_cols);
  
  coot_runtime_t::cq_guard guard;
  
  cl_kernel k1 = coot_runtime.get_kernel<eT>(kernel_id::submat_sum_colwise);
  
  cl_int status = 0;
  
  cl_mem tmp_mem = tmp.get_device_mem(false);
  cl_mem S_m_mem = S.m.get_device_mem(false);
  
  coot_runtime_t::adapt_uword S_m_n_rows(S.m.n_rows);
  
  coot_runtime_t::adapt_uword start_row(S.aux_row1);
  coot_runtime_t::adapt_uword start_col(S.aux_col1);
  
  coot_runtime_t::adapt_uword S_n_rows(S.n_rows);
  coot_runtime_t::adapt_uword S_n_cols(S.n_cols);
  
  status |= clSetKernelArg(k1, 0,  sizeof(cl_mem), &tmp_mem       );
  status |= clSetKernelArg(k1, 1,  sizeof(cl_mem), &S_m_mem       );
  status |= clSetKernelArg(k1, 2, S_m_n_rows.size, S_m_n_rows.addr);
  status |= clSetKernelArg(k1, 3,  start_row.size,  start_row.addr);
  status |= clSetKernelArg(k1, 4,  start_col.size,  start_col.addr);
  status |= clSetKernelArg(k1, 5,   S_n_rows.size,   S_n_rows.addr);
  status |= clSetKernelArg(k1, 6,   S_n_cols.size,   S_n_cols.addr);
  
  const size_t k1_work_dim       = 1;
  const size_t k1_work_offset[1] = { 0                };
  const size_t k1_work_size[1]   = { size_t(S.n_cols) };
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_cq(), k1, k1_work_dim, k1_work_offset, k1_work_size, NULL, 0, NULL, NULL);
  
  coot_check_cl_error(status, "accu()");
  
  
  // combine the column sums
  
  cl_kernel k2 = coot_runtime.get_kernel<eT>(kernel_id::accu_simple);
  
  status |= clSetKernelArg(k2, 0, sizeof(cl_mem), &tmp_mem     );
  status |= clSetKernelArg(k2, 1, sizeof(cl_mem), &tmp_mem     );
  status |= clSetKernelArg(k2, 2,  S_n_cols.size, S_n_cols.addr);
  
  const size_t k2_work_dim       = 1;
  const size_t k2_work_offset[1] = { 0 };
  const size_t k2_work_size[1]   = { 1 };
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_cq(), k2, k2_work_dim, k2_work_offset, k2_work_size, NULL, 0, NULL, NULL);
  
  coot_check_cl_error(status, "accu()");
  
  return tmp(0);
  }



//! @}
