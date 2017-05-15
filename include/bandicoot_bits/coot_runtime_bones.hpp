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



// TODO: if this is placed into a run-time library and executed there, what happens when two programs use the run-time library at the same time?
class coot_runtime_t
  {
  private:
  
  coot_aligned cl_platform_id   platform_id;
  coot_aligned cl_device_id     device_id;
  coot_aligned cl_context       context;
  coot_aligned cl_command_queue cq;
  coot_aligned uword            n_units;
  
  // cl_platform_id   is typedef for struct _cl_platform_id*
  // cl_device_id     is typedef for struct _cl_device_id*
  // cl_context       is typedef for struct _cl_context*
  // cl_command_queue is typedef for struct _cl_command_queue*
  // cl_kernel        is typedef for struct _cl_kernel*
  
  coot_aligned std::vector<cl_kernel>  u32_kernels;
  coot_aligned std::vector<cl_kernel>  s32_kernels;
  coot_aligned std::vector<cl_kernel>  u64_kernels;
  coot_aligned std::vector<cl_kernel>  s64_kernels;
  coot_aligned std::vector<cl_kernel>    f_kernels;
  coot_aligned std::vector<cl_kernel>    d_kernels;
  coot_aligned std::vector<cl_kernel> cx_f_kernels;
  coot_aligned std::vector<cl_kernel> cx_d_kernels;
  
  coot_aligned bool valid;
  coot_aligned bool device_64bit_sizet;
  coot_aligned bool device_64bit_float;
  
  #if defined(COOT_USE_CXX11)
  coot_aligned std::recursive_mutex mutex;
  #endif
  
  inline void   lock();  //! NOTE: do not call this function directly; instead instantiate the cq_guard class inside a relevant scope
  inline void unlock();  //! NOTE: do not call this function directly; it's automatically called when an instance of cq_guard goes out of scope
  
  inline void cleanup_cl();
  
  inline bool init_cl(std::string& out_errmsg, const bool manual_selection = false, const uword wanted_platform_id = 0, const uword wanted_device_id = 0);
  
  inline bool interrogate_device(const bool print_details);
  
  template<typename eT>
  inline bool init_kernels(std::vector<cl_kernel>& kernels, const std::string& source, const std::vector<std::string>& names);
  
  
  public:
  
  inline ~coot_runtime_t();
  inline  coot_runtime_t();
  
  #if defined(COOT_USE_CXX11)
                   coot_runtime_t(const coot_runtime_t&) = delete;
  coot_runtime_t&       operator=(const coot_runtime_t&) = delete;
  #endif
  
  inline uword get_n_units() const;
  
  inline bool is_valid()        const;
  inline bool has_64bit_sizet() const;
  inline bool has_64bit_float() const;
  
  template<typename eT>
  inline cl_mem acquire_memory(const uword n_elem);
  
  inline void release_memory(cl_mem device_mem);
  
  inline cl_context       get_context();
  inline cl_command_queue get_cq();
  
  // TODO: add function to return info about device as a string
  
  template<typename eT> inline cl_kernel get_kernel(const kernel_id::enum_id num);
  
  class program_wrapper;
  class cq_guard;
  class adapt_uword;
  
  friend class cq_guard;  // explicitly allows cq_guard to call lock() and unlock()
  };



class coot_runtime_t::program_wrapper
  {
  public:
  
  coot_aligned cl_program prog;  // cl_program is typedef for struct _cl_program*
  
  inline  program_wrapper();
  inline ~program_wrapper();
  
  #if defined(COOT_USE_CXX11)
                   program_wrapper(const program_wrapper&) = delete;
  program_wrapper&       operator=(const program_wrapper&) = delete;
  #endif
  };


class coot_runtime_t::cq_guard
  {
  public:
  
  inline  cq_guard();
  inline ~cq_guard();
  
  #if defined(COOT_USE_CXX11)
             cq_guard(const cq_guard&) = delete;
  cq_guard& operator=(const cq_guard&) = delete;
  #endif
  };



class coot_runtime_t::adapt_uword
  {
  public:
  
  coot_aligned size_t size;
  coot_aligned void*  addr;
  coot_aligned u64    val64;
  coot_aligned u32    val32;
  
  inline adapt_uword(const uword val);
  };


