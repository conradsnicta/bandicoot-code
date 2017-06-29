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



inline
coot_rt_t::~coot_rt_t()
  {
  coot_extra_debug_sigprint_this(this);
  
  internal_cleanup();
  
  valid = false;
  }



inline
coot_rt_t::coot_rt_t()
  {
  coot_extra_debug_sigprint_this(this);
  
  valid   = false;
  plt_id  = NULL;
  dev_id  = NULL;
  ctxt    = NULL;
  cq      = NULL;
  }



inline
bool
coot_rt_t::init(const bool print_info)
  {
  coot_extra_debug_sigprint();
  
  // TODO: investigate reading a config file by default; if a config file exist, use the specifed platform and device within the config file
  // TODO: config file may exist in several places: (1) globally accessible, such as /etc/bandicoot_config, or locally, such as ~/.config/bandicoot_config
  // TODO: use case: user puts code on a server which has a different configuration than the user's workstation
  
  return internal_init(false, 0, 0, print_info);
  }



inline
bool
coot_rt_t::init(const char* filename, const bool print_info)
  {
  coot_extra_debug_sigprint();
  
  return (*this).init(std::string(filename), print_info);
  }



inline
bool
coot_rt_t::init(const std::string filename, const bool print_info)
  {
  coot_extra_debug_sigprint();
  
  // TODO: handling of config files is currently rudimentary
  
  if(print_info)  {std::cout << "coot_rt_t::init(): reading " << filename << std::endl; }
  
  uword wanted_platform = 0;
  uword wanted_device   = 0;
  
  std::ifstream f;
  f.open(filename.c_str(), std::fstream::binary);
  
  if(f.is_open() == false)
    {
    std::cout << "coot_rt_t::init(): couldn't read " << filename << std::endl;
    return false;
    }
  
  f >> wanted_platform;
  f >> wanted_device;
  
  if(f.good() == false)
    {
    wanted_platform = 0;
    wanted_device   = 0;
    
    std::cout << "coot_rt_t::init(): couldn't read " << filename << std::endl;
    return false;
    }
  else
    {
    if(print_info)  { std::cout << "coot_rt_t::init(): wanted_platform = " << wanted_platform << "   wanted_device = " << wanted_device << std::endl; }
    }
  
  return internal_init(true, wanted_platform, wanted_device, print_info);
  }



inline
bool
coot_rt_t::init(const uword wanted_platform, const uword wanted_device, const bool print_info)
  {
  coot_extra_debug_sigprint();
  
  return internal_init(true, wanted_platform, wanted_device, print_info);
  }



inline
void
coot_rt_t::lock()
  {
  coot_extra_debug_sigprint();
  
  #if defined(COOT_USE_CXX11)
    {
    coot_extra_debug_print("calling mutex.lock()");
    mutex.lock();
    }
  #endif
  }



inline
void
coot_rt_t::unlock()
  {
  coot_extra_debug_sigprint();
  
  #if defined(COOT_USE_CXX11)
    {
    coot_extra_debug_print("calling mutex.unlock()");
    mutex.unlock();
    }
  #endif
  }



inline
void
coot_rt_t::internal_cleanup()
  {
  coot_extra_debug_sigprint();
  
  if(cq != NULL)  { clFinish(cq); }
  
  clblasTeardown();
  
  // TODO: go through each kernel vector
  
  const uword f_kernels_size = f_kernels.size();
  
  for(uword i=0; i<f_kernels_size; ++i)  { if(f_kernels.at(i) != NULL)  { clReleaseKernel(f_kernels.at(i)); } }
  
  if(cq   != NULL)  { clReleaseCommandQueue(cq); cq   = NULL; }
  if(ctxt != NULL)  { clReleaseContext(ctxt);    ctxt = NULL; }
  }



inline
bool
coot_rt_t::internal_init(const bool manual_selection, const uword wanted_platform, const uword wanted_device, const bool print_info)
  {
  coot_extra_debug_sigprint();
  
  internal_cleanup();
  valid = false;
  
  bool status = false;
  
  status = search_devices(plt_id, dev_id, manual_selection, wanted_platform, wanted_device, print_info);
  if(status == false)  { coot_debug_warn("coot_rt: couldn't find a suitable device"); return false; }
  
  interrogate_device(dev_info, plt_id, dev_id, print_info);
  
  if(dev_info.opencl_ver < 120)  { coot_debug_warn("coot_rt: selected device has OpenCL version < 1.2"); return false; }
  
  status = setup_queue(ctxt, cq, plt_id, dev_id);
  if(status == false)  { coot_debug_warn("coot_rt: couldn't setuq queue"); return false; }
  
  // setup kernels
  
  status = init_kernels<u32>(u32_kernels, kernel_src::get_source(), kernel_id::get_names());
  if(status == false)  { coot_debug_warn("coot_rt: couldn't setup OpenCL kernels"); return false; }
  
  status = init_kernels<s32>(s32_kernels, kernel_src::get_source(), kernel_id::get_names());
  if(status == false)  { coot_debug_warn("coot_rt: couldn't setup OpenCL kernels"); return false; }
  
  status = init_kernels<u64>(u64_kernels, kernel_src::get_source(), kernel_id::get_names());
  if(status == false)  { coot_debug_warn("coot_rt: couldn't setup OpenCL kernels"); return false; }
  
  status = init_kernels<s64>(s64_kernels, kernel_src::get_source(), kernel_id::get_names());
  if(status == false)  { coot_debug_warn("coot_rt: couldn't setup OpenCL kernels"); return false; }
  
  status = init_kernels<float>(f_kernels, kernel_src::get_source(), kernel_id::get_names());
  if(status == false)  { coot_debug_warn("coot_rt: couldn't setup OpenCL kernels"); return false; }
  
  // TODO: if 64 bit floats are supported, initialise double kernels
  
  
  // setup clBLAS
  
  
  get_stream_err1().flush();
  get_stream_err1() << "setup clBLAS: start" << endl;
  
  cl_int clblas_status = clblasSetup();
  
  get_stream_err1().flush();
  get_stream_err1() << "setup clBLAS: end" << endl;
    
  if(clblas_status != CL_SUCCESS)  { coot_debug_warn("coot_rt: couldn't setup clBLAS"); return false; }
  
  if(status == false)
    {
    internal_cleanup();
    valid = false;
    
    return false;
    }
  
  valid = true;
  
  return true;
  }



inline
bool
coot_rt_t::search_devices(cl_platform_id& out_plt_id, cl_device_id& out_dev_id, const bool manual_selection, const uword wanted_platform, const uword wanted_device, const bool print_info) const
  {
  coot_extra_debug_sigprint();
  
  // first, get a list of platforms and the devices on each platform
  
  cl_int  status      = 0;
  cl_uint n_platforms = 0;
  
  status = clGetPlatformIDs(0, NULL, &n_platforms);
  
  if((status != CL_SUCCESS) || (n_platforms == 0))
    {
    coot_debug_warn("coot_rt_t::search_devices(): no OpenCL platforms available");
    return false;
    }
  
  std::vector<cl_platform_id> platform_ids(n_platforms);
  
  status = clGetPlatformIDs(n_platforms, &(platform_ids[0]), NULL);
  
  if(status != CL_SUCCESS)
    {
    coot_debug_warn("coot_rt_t::search_devices(): couldn't get info on OpenCL platforms");
    return false;
    }
  

  // go through each platform
  
  std::vector< std::vector<cl_device_id> > device_ids(n_platforms);
  std::vector< std::vector<int         > > device_pri(n_platforms);  // device priorities
  
  for(size_t platform_count = 0; platform_count < n_platforms; ++platform_count)
    {
    cl_platform_id tmp_platform_id = platform_ids.at(platform_count);
    
    cl_uint local_n_devices = 0;
    
    status = clGetDeviceIDs(tmp_platform_id, CL_DEVICE_TYPE_ALL, 0, NULL, &local_n_devices);
    
    if((status != CL_SUCCESS) || (local_n_devices == 0))
      {
      continue;  // go to the next platform
      }
    
    std::vector<cl_device_id>& local_device_ids = device_ids.at(platform_count);
    std::vector<int>&          local_device_pri = device_pri.at(platform_count);
    
    local_device_ids.resize(local_n_devices);
    local_device_pri.resize(local_n_devices);
    
    status = clGetDeviceIDs(tmp_platform_id, CL_DEVICE_TYPE_ALL, local_n_devices, &(local_device_ids[0]), NULL);
    
    // go through each device on this platform
    for(size_t local_device_count = 0; local_device_count < local_n_devices; ++local_device_count)
      {
      cl_device_id local_device_id = local_device_ids.at(local_device_count);
      
      if(print_info)
        {
        get_stream_err1().flush();
        get_stream_err1() << "platform: " << platform_count << " / device: " << local_device_count << std::endl;
        }
      
      coot_rt_dev_info tmp_info;
      
      const bool ok = interrogate_device(tmp_info, tmp_platform_id, local_device_id, print_info);
      
      if(print_info)
        {
        if(ok == false)
          {
          get_stream_err1().flush();
          get_stream_err1() << "problem with getting info about device" << std::endl;
          }
        
        get_stream_err1() << std::endl;
        }
      
      local_device_pri.at(local_device_count) = 0;
      
      if(tmp_info.is_gpu)           { local_device_pri.at(local_device_count) +=  2; }
      if(tmp_info.has_float64)      { local_device_pri.at(local_device_count) +=  1; }
      if(tmp_info.opencl_ver < 120) { local_device_pri.at(local_device_count)  = -1; }
      }
    }
  
  
  if(manual_selection)
    {
    if(wanted_platform >= platform_ids.size())
      {
      coot_debug_warn("invalid platform number");
      return false;
      }
    
    std::vector<cl_device_id>& local_device_ids = device_ids.at(wanted_platform);
    
    if(wanted_device >= local_device_ids.size())
      {
      coot_debug_warn("invalid device number");
      return false;
      }
    
    if(print_info)
      {
      get_stream_err1() << "selected: platform: " << wanted_platform << " / device: " << wanted_device << std::endl;
      }
    
    out_plt_id = platform_ids.at(wanted_platform);
    out_dev_id = local_device_ids.at(wanted_device);
    
    return true;
    }
  
  
  // select the device with highest priority
  
  bool found_device = false;
  
  int    best_val          = -1;
  size_t best_platform_num =  0;
  size_t best_device_num   =  0;
  
  for(size_t platform_count = 0; platform_count < n_platforms; ++platform_count)
    {
    std::vector<cl_device_id>& local_device_ids = device_ids.at(platform_count);
    std::vector<int>&          local_device_pri = device_pri.at(platform_count);
    
    size_t local_n_devices = local_device_ids.size();
    
    for(size_t local_device_count = 0; local_device_count < local_n_devices; ++local_device_count)
      {
      const int tmp_val = local_device_pri.at(local_device_count);
      
      // cout << "platform_count: " << platform_count << "  local_device_count: " << local_device_count << "  priority: " << tmp_val << "   best_val: " << best_val << endl;
      
      if(best_val < tmp_val)
        {
        best_val          = tmp_val;
        best_platform_num = platform_count;
        best_device_num   = local_device_count;
        
        found_device = true;
        }
      }
    }
  
  if(found_device)
    {
    if(print_info)
      {
      get_stream_err1() << "selected: platform: " << best_platform_num << " / device: " << best_device_num << std::endl;
      }
    
    std::vector<cl_device_id>& local_device_ids = device_ids.at(best_platform_num);
    
    out_plt_id = platform_ids.at(best_platform_num);
    out_dev_id = local_device_ids.at(best_device_num);
    }
     
  return found_device;
  }



inline
bool
coot_rt_t::interrogate_device(coot_rt_dev_info& out_info, cl_platform_id in_plt_id, cl_device_id in_dev_id, const bool print_info) const
  {
  coot_extra_debug_sigprint();
  
  cl_char dev_name1[1024]; // TODO: use dynamic memory allocation (podarray or std::vector)
  cl_char dev_name2[1024];
  cl_char dev_name3[1024];
  
  dev_name1[0] = cl_char(0);
  dev_name2[0] = cl_char(0);
  dev_name3[0] = cl_char(0);
  
  cl_device_type      dev_type = 0;
  cl_device_fp_config dev_fp64 = 0;
  
  cl_uint dev_n_units     = 0;
  cl_uint dev_sizet_width = 0;
  cl_uint dev_ptr_width   = 0;
  cl_uint dev_opencl_ver  = 0;
  cl_uint dev_align       = 0;
  
  
  clGetDeviceInfo(in_dev_id, CL_DEVICE_VENDOR,              sizeof(dev_name1),           &dev_name1,   NULL);
  clGetDeviceInfo(in_dev_id, CL_DEVICE_NAME,                sizeof(dev_name2),           &dev_name2,   NULL);
  clGetDeviceInfo(in_dev_id, CL_DEVICE_VERSION,             sizeof(dev_name3),           &dev_name3,   NULL);
  clGetDeviceInfo(in_dev_id, CL_DEVICE_TYPE,                sizeof(cl_device_type),      &dev_type,    NULL);
  clGetDeviceInfo(in_dev_id, CL_DEVICE_DOUBLE_FP_CONFIG,    sizeof(cl_device_fp_config), &dev_fp64,    NULL);
  clGetDeviceInfo(in_dev_id, CL_DEVICE_MAX_COMPUTE_UNITS,   sizeof(cl_uint),             &dev_n_units, NULL);
  clGetDeviceInfo(in_dev_id, CL_DEVICE_MEM_BASE_ADDR_ALIGN, sizeof(cl_uint),             &dev_align,   NULL);
  
  // contrary to the official OpenCL specification (OpenCL 1.2, sec 4.2 and sec 6.1.1).
  // certain OpenCL implementations use internal size_t which doesn't correspond to CL_DEVICE_ADDRESS_BITS
  // example: Clover from Mesa 13.0.4, running as AMD OLAND (DRM 2.48.0 / 4.9.14-200.fc25.x86_64, LLVM 3.9.1)
  

  const char* tmp_program_src = \
    "__kernel void coot_interrogate(__global uint* out) \n"
    "  {                                                \n"
    "  const size_t i = get_global_id(0);               \n"
    "  if(i == 0)                                       \n"
    "    {                                              \n"
    "    out[0] = (uint)sizeof(size_t);                 \n"
    "    out[1] = (uint)sizeof(void*);                  \n"
    "    out[2] = (uint)(__OPENCL_VERSION__);           \n"
    "    }                                              \n"
    "  }                                                \n";
  
  cl_context       tmp_context    = NULL;
  cl_command_queue tmp_queue      = NULL;
  cl_program       tmp_program    = NULL;
  cl_kernel        tmp_kernel     = NULL;
  cl_mem           tmp_dev_mem    = NULL;
  cl_uint          tmp_cpu_mem[4] = { 0, 0, 0, 0 };
  
  
  cl_int status = 0;
  
  if(setup_queue(tmp_context, tmp_queue, in_plt_id, in_dev_id))
    {
    tmp_program = clCreateProgramWithSource(tmp_context, 1, (const char **)&(tmp_program_src), NULL, &status);
    
    if(status == CL_SUCCESS)
      {
      status = clBuildProgram(tmp_program, 0, NULL, NULL, NULL, NULL);
      
      // cout << "status: " << coot_cl_error::as_string(status) << endl;
      // 
      // size_t len = 0;
      // char buffer[10240];
      // 
      // clGetProgramBuildInfo(tmp_program, in_dev_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
      // std::cout << "output from clGetProgramBuildInfo():" << std::endl;
      // std::cout << buffer << std::endl;
      
      if(status == CL_SUCCESS)
        {
        tmp_kernel = clCreateKernel(tmp_program, "coot_interrogate", &status);
        
        if(status == CL_SUCCESS)
          {
          tmp_dev_mem = clCreateBuffer(tmp_context, CL_MEM_READ_WRITE, sizeof(cl_uint)*4, NULL, &status);
          
          clSetKernelArg(tmp_kernel, 0, sizeof(cl_mem),  &tmp_dev_mem);
          status = clEnqueueTask(tmp_queue, tmp_kernel, 0, NULL, NULL);  // TODO: replace with clEnqueueNDRangeKernel to avoid deprecation warnings
          
          if(status == CL_SUCCESS)
            {
            clFinish(cq);
            
            status = clEnqueueReadBuffer(tmp_queue, tmp_dev_mem, CL_TRUE, 0, sizeof(cl_uint)*4, tmp_cpu_mem, 0, NULL, NULL);
            
            if(status == CL_SUCCESS)
              {
              clFinish(cq);
              
              dev_sizet_width = tmp_cpu_mem[0];
              dev_ptr_width   = tmp_cpu_mem[1];
              dev_opencl_ver  = tmp_cpu_mem[2];
              }
            }
          }
        }
      }
    }
  
  if(status != CL_SUCCESS)
    {
    coot_debug_warn(coot_cl_error::as_string(status));
    }
  
  if(tmp_dev_mem != NULL)  { clReleaseMemObject   (tmp_dev_mem); }
  if(tmp_kernel  != NULL)  { clReleaseKernel      (tmp_kernel ); }
  if(tmp_program != NULL)  { clReleaseProgram     (tmp_program); }
  if(tmp_queue   != NULL)  { clReleaseCommandQueue(tmp_queue);   }
  if(tmp_context != NULL)  { clReleaseContext     (tmp_context); }
  
  if(print_info)
    {
    get_stream_err1().flush();
    get_stream_err1() << "name1:       " << dev_name1 << std::endl;
    get_stream_err1() << "name2:       " << dev_name2 << std::endl;
    get_stream_err1() << "name3:       " << dev_name3 << std::endl;
    get_stream_err1() << "is_gpu:      " << (dev_type == CL_DEVICE_TYPE_GPU)  << std::endl;
    get_stream_err1() << "fp64:        " << dev_fp64 << std::endl;
    get_stream_err1() << "sizet_width: " << dev_sizet_width  << std::endl;
    get_stream_err1() << "ptr_width:   " << dev_ptr_width << std::endl;
    get_stream_err1() << "n_units:     " << dev_n_units << std::endl;
    get_stream_err1() << "opencl_ver:  " << dev_opencl_ver << std::endl;
  //get_stream_err1() << "align:       " << dev_align  << std::endl;
    }
  
  out_info.is_gpu      = (dev_type == CL_DEVICE_TYPE_GPU);
  out_info.has_float64 = (dev_fp64 != 0);
  out_info.has_sizet64 = (dev_sizet_width >= 8);
  out_info.ptr_width   = uword(dev_ptr_width);
  out_info.n_units     = uword(dev_n_units);
  out_info.opencl_ver  = uword(dev_opencl_ver);
  
  return (status == CL_SUCCESS);
  }




inline
bool
coot_rt_t::setup_queue(cl_context& out_context, cl_command_queue& out_queue, cl_platform_id in_plat_id, cl_device_id in_dev_id) const
  {
  coot_extra_debug_sigprint();
  
  cl_context_properties prop[3] = { CL_CONTEXT_PLATFORM, cl_context_properties(in_plat_id), 0 };
  
  cl_int status = 0;
  
  out_context = clCreateContext(prop, 1, &in_dev_id, NULL, NULL, &status);
  
  if((status != CL_SUCCESS) || (out_context == NULL))
    {
    coot_debug_warn(coot_cl_error::as_string(status));
    return false;
    }
  
  // NOTE: clCreateCommandQueue is deprecated as of OpenCL 2.0, but it will be supported for the "foreseeable future"
  // NOTE: clCreateCommandQueue is replaced with clCreateCommandQueueWithProperties in OpenCL 2.0
  // NOTE: http://stackoverflow.com/questions/28500496/opencl-function-found-deprecated-by-visual-studio
    
  out_queue = clCreateCommandQueue(out_context, in_dev_id, 0, &status);
  
  if((status != CL_SUCCESS) || (out_queue == NULL))
    {
    coot_debug_warn(coot_cl_error::as_string(status));
    return false;
    }
  
  return true;
  }



template<typename eT>
inline
bool
coot_rt_t::init_kernels(std::vector<cl_kernel>& kernels, const std::string& source, const std::vector<std::string>& names)
  {
  coot_extra_debug_sigprint();
  
  cl_int status;
  
  // TODO: get info using clquery ?
  
  coot_rt_t::program_wrapper prog_holder;  // program_wrapper will automatically call clReleaseProgram() when it goes out of scope
  

  // cl_program clCreateProgramWithSource(cl_context context, cl_uint count, const char **strings, const size_t *lengths, cl_int *errcode_ret);
  // strings = An array of N pointers (where N = count) to optionally null-terminated character strings that make up the source code. 
  // lengths = An array with the number of chars in each string (the string length). If an element in lengths is zero, its accompanying string is null-terminated.
  //           If lengths is NULL, all strings in the strings argument are considered null-terminated.
  //           Any length value passed in that is greater than zero excludes the null terminator in its count. 
  
  
  status = 0;
  
  const char* source_c_str = source.c_str();
  
  prog_holder.prog = clCreateProgramWithSource(ctxt, 1, &source_c_str, NULL, &status);
  
  if((status != CL_SUCCESS) || (prog_holder.prog == NULL))
    {
    cout << "status: " << coot_cl_error::as_string(status) << endl;
    
    std::cout << "coot_rt::init_kernels(): couldn't create program" << std::endl;
    return false;
    }
  
  // TODO: the defines need to be progressively built
  // use std::string, concatenation and .c_str()
  
  std::string build_options;
  std::string prefix;
  
  if(is_same_type<eT, u32>::yes)
    {
    prefix = "u32_";
    
    build_options += "-D PREFIX=";
    build_options += prefix;
    build_options += " ";
    build_options += "-D eT=uint";
    build_options += " ";
    build_options += "-D promoted_eT=float";
    build_options += " ";
    }
  else
  if(is_same_type<eT, s32>::yes)
    {
    prefix = "s32_";
    
    build_options += "-D PREFIX=";
    build_options += prefix;
    build_options += " ";
    build_options += "-D eT=int";
    build_options += " ";
    build_options += "-D promoted_eT=float";
    build_options += " ";
    }
  else
  if(is_same_type<eT, u64>::yes)
    {
    prefix = "u64_";
    
    build_options += "-D PREFIX=";
    build_options += prefix;
    build_options += " ";
    build_options += "-D eT=ulong";
    build_options += " ";
    build_options += "-D promoted_eT=float";
    build_options += " ";
    }
  else
  if(is_same_type<eT, s64>::yes)
    {
    prefix = "s64_";
    
    build_options += "-D PREFIX=";
    build_options += prefix;
    build_options += " ";
    build_options += "-D eT=long";
    build_options += " ";
    build_options += "-D promoted_eT=float";
    build_options += " ";
    }
  else
  if(is_same_type<eT, float>::yes)
    {
    prefix = "f_";
    
    build_options += "-D PREFIX=";
    build_options += prefix;
    build_options += " ";
    build_options += "-D eT=float";
    build_options += " ";
    build_options += "-D promoted_eT=float";
    build_options += " ";
    }
  else
  if(is_same_type<eT, double>::yes)
    {
    prefix = "d_";
    
    build_options += "-D PREFIX=";
    build_options += prefix;
    build_options += " ";
    build_options += "-D eT=double";
    build_options += " ";
    build_options += "-D promoted_eT=double";
    build_options += " ";
    }
  
  
  build_options += ((sizeof(uword) >= 8) && dev_info.has_sizet64) ? "-D UWORD=ulong" : "-D UWORD=uint";
    
  status = clBuildProgram(prog_holder.prog, 0, NULL, build_options.c_str(), NULL, NULL);
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << coot_cl_error::as_string(status) << endl;
    
    size_t len = 0;
    char buffer[10240];  // TODO: use std::vector<char> or podarray

    clGetProgramBuildInfo(prog_holder.prog, dev_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
    std::cout << "coot_rt::init_kernels(): couldn't build program;"              << std::endl;
    std::cout << "coot_rt::init_kernels(): output from clGetProgramBuildInfo():" << std::endl;
    std::cout << buffer << std::endl;
    
    return false;
    }
  
  
  const uword n_kernels = names.size();
  
  kernels.resize(n_kernels);
  
  for(uword i=0; i < n_kernels; ++i)
    {
    status = 0;
    
    const std::string kernel_name = prefix + names.at(i);
    
    kernels.at(i) = clCreateKernel(prog_holder.prog, kernel_name.c_str(), &status);
    
    if((status != CL_SUCCESS) || (kernels[i] == NULL))
      {
      std::cout << coot_cl_error::as_string(status) << endl;
      std::cout << "kernel_name: " << kernel_name << endl;
      return false;
      }
    }
  
  return true;
  }



inline
uword
coot_rt_t::get_n_units() const
  {
  return (valid) ? dev_info.n_units : uword(0);
  }



inline
bool
coot_rt_t::is_valid() const
  {
  return valid;
  }



inline
bool
coot_rt_t::has_sizet64() const
  {
  return dev_info.has_sizet64;
  }



inline
bool
coot_rt_t::has_float64() const
  {
  return dev_info.has_float64;
  }



template<typename eT>
inline
cl_mem
coot_rt_t::acquire_memory(const uword n_elem)
  {
  coot_extra_debug_sigprint();
  
  coot_check_runtime_error( (valid == false), "coot_rt::acquire_memory(): runtime not valid" );
  
  if(n_elem == 0)  { return NULL; }
  
  coot_debug_check
   (
   ( size_t(n_elem) > (std::numeric_limits<size_t>::max() / sizeof(eT)) ),
   "coot_rt::acquire_memory(): requested size is too large"
   );
  
  cl_int status = 0;
  cl_mem dev_mem = clCreateBuffer(ctxt, CL_MEM_READ_WRITE, sizeof(eT)*(std::max)(uword(1), n_elem), NULL, &status);
  
  coot_check_bad_alloc( ((status != CL_SUCCESS) || (dev_mem == NULL)), "coot_rt::acquire_memory(): not enough memory on device" );
  
  return dev_mem;
  }



inline
void
coot_rt_t::release_memory(cl_mem dev_mem)
  {
  coot_extra_debug_sigprint();
  
  coot_debug_check( (valid == false), "coot_rt not valid" );
  
  if(dev_mem)  { clReleaseMemObject(dev_mem); }
  }



inline
cl_device_id
coot_rt_t::get_device()
  {
  coot_extra_debug_sigprint();
  
  coot_debug_check( (valid == false), "coot_rt not valid" );
  
  return dev_id;
  }



inline
cl_context
coot_rt_t::get_context()
  {
  coot_extra_debug_sigprint();
  
  coot_debug_check( (valid == false), "coot_rt not valid" );
  
  return ctxt;
  }



inline
cl_command_queue
coot_rt_t::get_cq()
  {
  coot_extra_debug_sigprint();
  
  coot_debug_check( (valid == false), "coot_rt not valid" );
  
  return cq;
  }



inline
bool
coot_rt_t::create_extra_cq(cl_command_queue& out_queue)
  {
  coot_extra_debug_sigprint();
  
  coot_debug_check( (valid == false), "coot_rt not valid" );
  
  cl_int status = 0;
  
  out_queue = clCreateCommandQueue((*this).ctxt, (*this).dev_id, 0, &status);
  
  if((status != CL_SUCCESS) || (out_queue == NULL))
    {
    coot_debug_warn(coot_cl_error::as_string(status));
    return false;
    }
  
  return true;
  }



inline
void
coot_rt_t::delete_extra_cq(cl_command_queue& in_queue)
  {
  coot_extra_debug_sigprint();
  
  coot_debug_check( (valid == false), "coot_rt not valid" );
  
  if(in_queue != NULL)  { clReleaseCommandQueue(in_queue); in_queue = NULL; }
  }



template<typename eT>
inline
cl_kernel
coot_rt_t::get_kernel(const kernel_id::enum_id num)
  {
  coot_extra_debug_sigprint();
  
  coot_debug_check( (valid == false), "coot_rt not valid" );
  
       if(is_same_type<eT,u32   >::yes)  { return u32_kernels.at(num); }
  else if(is_same_type<eT,s32   >::yes)  { return s32_kernels.at(num); }
  else if(is_same_type<eT,u64   >::yes)  { return u64_kernels.at(num); }
  else if(is_same_type<eT,s64   >::yes)  { return s64_kernels.at(num); }
  else if(is_same_type<eT,float >::yes)  { return   f_kernels.at(num); }
  else if(is_same_type<eT,double>::yes)  { return   d_kernels.at(num); }
  else { coot_debug_check(true, "unsupported element type" ); }
  }



// TODO: should this be in the run-time library, to ensure only one copy of the runtime is active?
// TODO: don't instantiate coot_rt_t if COOT_USE_WRAPPER is enabled?
// TODO: put instantiation in an anonymous namespace to avoid linking conflicts?
coot_rt_t coot_rt;



//
// program_wrapper

inline
coot_rt_t::program_wrapper::program_wrapper()
  {
  coot_extra_debug_sigprint();
  
  prog = NULL;
  }



inline
coot_rt_t::program_wrapper::~program_wrapper()
  {
  coot_extra_debug_sigprint();
  
  if(prog != NULL)  { clReleaseProgram(prog); }
  }






//
// cq_guard

inline
coot_rt_t::cq_guard::cq_guard()
  {
  coot_extra_debug_sigprint();
  
  coot_rt.lock();
  
  if(coot_rt.is_valid())
    {
    coot_extra_debug_print("calling clFinish()");
    clFinish(coot_rt.get_cq());  // force synchronisation
    
    //coot_extra_debug_print("calling clFlush()");
    //clFlush(coot_rt.get_cq());  // submit all enqueued commands
    }
  }



inline
coot_rt_t::cq_guard::~cq_guard()
  {
  coot_extra_debug_sigprint();
  
  if(coot_rt.is_valid())
    {
    coot_extra_debug_print("calling clFlush()");
    clFlush(coot_rt.get_cq());  // submit all enqueued commands
    }
  
  coot_rt.unlock();
  }




//
// adapt_uword

inline
coot_rt_t::adapt_uword::adapt_uword(const uword val)
  {
  if((sizeof(uword) >= 8) && coot_rt.has_sizet64())
    {
    size  = sizeof(u64);
    addr  = (void*)(&val64);
    val64 = u64(val);
    }
  else
    {
    size   = sizeof(u32);
    addr   = (void*)(&val32);
    val32  = u32(val);
    
    coot_check_runtime_error( ((sizeof(uword) >= 8) && (val > 0xffffffffU)), "adapt_uword: given value doesn't fit into unsigned 32 bit integer" );
    }
  }
