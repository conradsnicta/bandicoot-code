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



struct coot_rt_dev_info
  {
  public:
  
  coot_aligned bool  is_gpu;
  coot_aligned bool  has_float64;
  coot_aligned bool  has_sizet64;
  coot_aligned uword ptr_width;
  coot_aligned uword n_units;
  coot_aligned uword opencl_ver;
  
  inline
  void
  reset()
    {
    is_gpu      = false;
    has_float64 = false;
    has_sizet64 = false;
    n_units     = 0;
    ptr_width   = 0;
    opencl_ver  = 0;
    }
  
  inline coot_rt_dev_info()  { reset(); }
  };



// TODO: if this is placed into a run-time library and executed there, what happens when two programs use the run-time library at the same time?
class coot_rt_t
  {
  private:
  
  coot_aligned bool             valid;
  
  coot_aligned cl_platform_id   plt_id;
  coot_aligned cl_device_id     dev_id;
  coot_aligned cl_context       ctxt;
  coot_aligned cl_command_queue cq;
  
  coot_aligned coot_rt_dev_info dev_info;
  
  coot_aligned std::vector<cl_kernel>  u32_kernels;
  coot_aligned std::vector<cl_kernel>  s32_kernels;
  coot_aligned std::vector<cl_kernel>  u64_kernels;
  coot_aligned std::vector<cl_kernel>  s64_kernels;
  coot_aligned std::vector<cl_kernel>    f_kernels;
  coot_aligned std::vector<cl_kernel>    d_kernels;
  coot_aligned std::vector<cl_kernel> cx_f_kernels;
  coot_aligned std::vector<cl_kernel> cx_d_kernels;
  
  #if defined(COOT_USE_CXX11)
  coot_aligned std::recursive_mutex mutex;
  #endif
  
  inline void   lock();  //! NOTE: do not call this function directly; instead instantiate the cq_guard class inside a relevant scope
  inline void unlock();  //! NOTE: do not call this function directly; it's automatically called when an instance of cq_guard goes out of scope
  
  inline void internal_cleanup();
  inline bool internal_init(const bool manual_selection, const uword wanted_platform, const uword wanted_device, const bool print_info);
  
  inline bool search_devices(cl_platform_id& out_plat_id, cl_device_id& out_dev_id, const bool manual_selection, const uword wanted_platform, const uword wanted_device, const bool print_info) const;
  
  inline bool interrogate_device(coot_rt_dev_info& out_info, cl_platform_id in_plat_id, cl_device_id in_dev_id, const bool print_info) const;
  
  inline bool setup_queue(cl_context& out_context, cl_command_queue& out_queue, cl_platform_id in_plat_id, cl_device_id in_dev_id) const;
  
  template<typename eT>
  inline bool init_kernels(std::vector<cl_kernel>& kernels, const std::string& source, const std::vector<std::string>& names);
  
  
  
  public:
  
  inline ~coot_rt_t();
  inline  coot_rt_t();
  
  inline bool init(const bool print_info = false);
  inline bool init(const char*       filename, const bool print_info = false);
  inline bool init(const std::string filename, const bool print_info = false);
  inline bool init(const uword wanted_platform, const uword wanted_device, const bool print_info = false);
  
  #if defined(COOT_USE_CXX11)
                   coot_rt_t(const coot_rt_t&) = delete;
  coot_rt_t&       operator=(const coot_rt_t&) = delete;
  #endif
  
  inline uword get_n_units() const;
  
  inline bool is_valid()    const;
  inline bool has_sizet64() const;
  inline bool has_float64() const;
  
  template<typename eT>
  inline cl_mem acquire_memory(const uword n_elem);
  
  inline void release_memory(cl_mem dev_mem);
  
  inline cl_device_id     get_device();
  inline cl_context       get_context();
  inline cl_command_queue get_cq();
  
  inline bool create_extra_cq(cl_command_queue& out_queue);
  inline void delete_extra_cq(cl_command_queue&  in_queue);
  
  // TODO: add function to return info about device as a string
  
  template<typename eT> inline cl_kernel get_kernel(const kernel_id::enum_id num);
  
  class program_wrapper;
  class cq_guard;
  class adapt_uword;
  
  friend class cq_guard;  // explicitly allows cq_guard to call lock() and unlock()
  };



class coot_rt_t::program_wrapper
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


class coot_rt_t::cq_guard
  {
  public:
  
  inline  cq_guard();
  inline ~cq_guard();
  
  #if defined(COOT_USE_CXX11)
             cq_guard(const cq_guard&) = delete;
  cq_guard& operator=(const cq_guard&) = delete;
  #endif
  };



class coot_rt_t::adapt_uword
  {
  public:
  
  coot_aligned size_t size;
  coot_aligned void*  addr;
  coot_aligned u64    val64;
  coot_aligned u32    val32;
  
  inline adapt_uword(const uword val);
  };


