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


//! \addtogroup MatValProxy
//! @{



template<typename eT>
coot_inline
MatValProxy<eT>::MatValProxy(Mat<eT>& in_M, const uword in_index)
  : M    (in_M    )
  , index(in_index)
  {
  coot_extra_debug_sigprint();
  }



template<typename eT>
coot_inline
MatValProxy<eT>::operator eT()
  {
  return MatValProxy<eT>::get_val(M, index);
  }



template<typename eT>
inline
eT
MatValProxy<eT>::get_val(const Mat<eT>& M, const uword index)
  {
  coot_extra_debug_sigprint();
  
  coot_runtime_t::cq_guard guard;
  
  coot_aligned cl_int status = 0;
  
  coot_aligned void* mapped_ptr = clEnqueueMapBuffer(coot_runtime.get_cq(), M.dev_mem, CL_TRUE, CL_MAP_READ, sizeof(eT)*index, sizeof(eT)*1, 0, NULL, NULL, &status);
  
  eT val = eT(0);
  
  if( (status == CL_SUCCESS) && (mapped_ptr != NULL) )
    {
    val = *((eT*)(mapped_ptr));
    
    status = clEnqueueUnmapMemObject(coot_runtime.get_cq(), M.dev_mem, mapped_ptr, 0, NULL, NULL);
    }
  
  coot_check_runtime_error( (status != CL_SUCCESS), "MatValProxy: couldn't access device memory" );
  
  return val;
  
  
  // coot_aligned eT               val;
  // coot_aligned cl_buffer_region region;
  // 
  // region.origin = sizeof(eT)*ii; 
  // region.size   = sizeof(eT)*1;
  // 
  // cl_int status = 0;
  // // NOTE: the origin must be a multiple of alignment: see coot_runtime_meat how to get the alignment
  // cl_mem sub_mem = clCreateSubBuffer(dev_mem, CL_MEM_READ_WRITE, CL_BUFFER_CREATE_TYPE_REGION, &region, &status);
  // 
  // coot_check_runtime_error( (status != CL_SUCCESS), "Mat::operator(): couldn't create subbuffer" );
  // 
  // status |= clEnqueueReadBuffer(coot_runtime.get_cq(), sub_mem, CL_TRUE, 0, sizeof(eT)*1, &val, 0, NULL, NULL);
  // status |= clFinish(coot_runtime.get_cq());
  // 
  // coot_check_runtime_error( (status != CL_SUCCESS), "Mat::operator(): couldn't read from device memory" );
  // 
  // return val;
  }



template<typename eT>
inline
void
MatValProxy<eT>::operator=(const eT in_val)
  {
  coot_extra_debug_sigprint();
  
  coot_runtime_t::cq_guard guard;
  
  coot_aligned cl_int status = 0;
  
  coot_aligned void* mapped_ptr = clEnqueueMapBuffer(coot_runtime.get_cq(), M.dev_mem, CL_TRUE, CL_MAP_WRITE, sizeof(eT)*index, sizeof(eT)*1, 0, NULL, NULL, &status);
  
  if( (status == CL_SUCCESS) && (mapped_ptr != NULL) )
    {
    *((eT*)(mapped_ptr)) = in_val;
    
    status = clEnqueueUnmapMemObject(coot_runtime.get_cq(), M.dev_mem, mapped_ptr, 0, NULL, NULL);  
    }
  
  coot_check_runtime_error( (status != CL_SUCCESS), "MatValProxy: couldn't access device memory" );
  }



template<typename eT>
inline
void
MatValProxy<eT>::operator+=(const eT in_val)
  {
  coot_extra_debug_sigprint();
  
  coot_runtime_t::cq_guard guard;
  
  coot_aligned cl_int status = 0;
  
  coot_aligned void* mapped_ptr = clEnqueueMapBuffer(coot_runtime.get_cq(), M.dev_mem, CL_TRUE, (CL_MAP_READ | CL_MAP_WRITE), sizeof(eT)*index, sizeof(eT)*1, 0, NULL, NULL, &status);
  
  if( (status == CL_SUCCESS) && (mapped_ptr != NULL) )
    {
    *((eT*)(mapped_ptr)) += in_val;
    
    status = clEnqueueUnmapMemObject(coot_runtime.get_cq(), M.dev_mem, mapped_ptr, 0, NULL, NULL);  
    }
  
  coot_check_runtime_error( (status != CL_SUCCESS), "MatValProxy: couldn't access device memory" );
  }



template<typename eT>
inline
void
MatValProxy<eT>::operator-=(const eT in_val)
  {
  coot_extra_debug_sigprint();
  
  coot_runtime_t::cq_guard guard;
  
  coot_aligned cl_int status = 0;
  
  coot_aligned void* mapped_ptr = clEnqueueMapBuffer(coot_runtime.get_cq(), M.dev_mem, CL_TRUE, (CL_MAP_READ | CL_MAP_WRITE), sizeof(eT)*index, sizeof(eT)*1, 0, NULL, NULL, &status);
  
  if( (status == CL_SUCCESS) && (mapped_ptr != NULL) )
    {
    *((eT*)(mapped_ptr)) -= in_val;
    
    status = clEnqueueUnmapMemObject(coot_runtime.get_cq(), M.dev_mem, mapped_ptr, 0, NULL, NULL);  
    }
  
  coot_check_runtime_error( (status != CL_SUCCESS), "MatValProxy: couldn't access device memory" );
  }



template<typename eT>
inline
void
MatValProxy<eT>::operator*=(const eT in_val)
  {
  coot_extra_debug_sigprint();
  
  coot_runtime_t::cq_guard guard;
  
  coot_aligned cl_int status = 0;
  
  coot_aligned void* mapped_ptr = clEnqueueMapBuffer(coot_runtime.get_cq(), M.dev_mem, CL_TRUE, (CL_MAP_READ | CL_MAP_WRITE), sizeof(eT)*index, sizeof(eT)*1, 0, NULL, NULL, &status);
  
  if( (status == CL_SUCCESS) && (mapped_ptr != NULL) )
    {
    *((eT*)(mapped_ptr)) *= in_val;
    
    status = clEnqueueUnmapMemObject(coot_runtime.get_cq(), M.dev_mem, mapped_ptr, 0, NULL, NULL);  
    }
  
  coot_check_runtime_error( (status != CL_SUCCESS), "MatValProxy: couldn't access device memory" );
  }



template<typename eT>
inline
void
MatValProxy<eT>::operator/=(const eT in_val)
  {
  coot_extra_debug_sigprint();
  
  coot_runtime_t::cq_guard guard;
  
  coot_aligned cl_int status = 0;
  
  coot_aligned void* mapped_ptr = clEnqueueMapBuffer(coot_runtime.get_cq(), M.dev_mem, CL_TRUE, (CL_MAP_READ | CL_MAP_WRITE), sizeof(eT)*index, sizeof(eT)*1, 0, NULL, NULL, &status);
  
  if( (status == CL_SUCCESS) && (mapped_ptr != NULL) )
    {
    *((eT*)(mapped_ptr)) /= in_val;
    
    status = clEnqueueUnmapMemObject(coot_runtime.get_cq(), M.dev_mem, mapped_ptr, 0, NULL, NULL);  
    }
  
  coot_check_runtime_error( (status != CL_SUCCESS), "MatValProxy: couldn't access device memory" );
  }



//! @}
