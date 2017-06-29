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


//! \addtogroup coot_cl_error
//! @{


struct coot_cl_error
  {
  coot_cold
  static
  inline
  std::string
  as_string(const cl_int error_code)
    {
    switch(error_code)
      {
      case   0: return "cl_success";
      case  -1: return "cl_device_not_found";
      case  -2: return "cl_device_not_available";
      case  -3: return "cl_compiler_not_available";
      case  -4: return "cl_mem_object_allocation_failure";
      case  -5: return "cl_out_of_resources";
      case  -6: return "cl_out_of_host_memory";
      case  -7: return "cl_profiling_info_not_available";
      case  -8: return "cl_mem_copy_overlap";
      case  -9: return "cl_image_format_mismatch";
      case -10: return "cl_image_format_not_supported";
      case -11: return "cl_build_program_failure";
      case -12: return "cl_map_failure";
      case -13: return "cl_misaligned_sub_buffer_offset";
      case -14: return "cl_exec_status_error_for_events_in_wait_list";
      case -15: return "cl_compile_program_failure";
      case -16: return "cl_linker_not_available";
      case -17: return "cl_link_program_failure";
      case -18: return "cl_device_partition_failed";
      case -19: return "cl_kernel_arg_info_not_available";
      case -30: return "cl_invalid_value";
      case -31: return "cl_invalid_device_type";
      case -32: return "cl_invalid_platform";
      case -33: return "cl_invalid_device";
      case -34: return "cl_invalid_context";
      case -35: return "cl_invalid_queue_properties";
      case -36: return "cl_invalid_command_queue";
      case -37: return "cl_invalid_host_ptr";
      case -38: return "cl_invalid_mem_object";
      case -39: return "cl_invalid_image_format_descriptor";
      case -40: return "cl_invalid_image_size";
      case -41: return "cl_invalid_sampler";
      case -42: return "cl_invalid_binary";
      case -43: return "cl_invalid_build_options";
      case -44: return "cl_invalid_program";
      case -45: return "cl_invalid_program_executable";
      case -46: return "cl_invalid_kernel_name";
      case -47: return "cl_invalid_kernel_definition";
      case -48: return "cl_invalid_kernel";
      case -49: return "cl_invalid_arg_index";
      case -50: return "cl_invalid_arg_value";
      case -51: return "cl_invalid_arg_size";
      case -52: return "cl_invalid_kernel_args";
      case -53: return "cl_invalid_work_dimension";
      case -54: return "cl_invalid_work_group_size";
      case -55: return "cl_invalid_work_item_size";
      case -56: return "cl_invalid_global_offset";
      case -57: return "cl_invalid_event_wait_list";
      case -58: return "cl_invalid_event";
      case -59: return "cl_invalid_operation";
      case -60: return "cl_invalid_gl_object";
      case -61: return "cl_invalid_buffer_size";
      case -62: return "cl_invalid_mip_level";
      case -63: return "cl_invalid_global_work_size";
      case -64: return "cl_invalid_property";
      case -65: return "cl_invalid_image_descriptor";
      case -66: return "cl_invalid_compiler_options";
      case -67: return "cl_invalid_linker_options";
      case -68: return "cl_invalid_device_partition_count";
      case -69: return "cl_invalid_pipe_size";
      case -70: return "cl_invalid_device_queue";
      default:  return "unknown error code";
      }
    }
  };



struct coot_clblas_error
  {
  coot_cold
  static
  inline
  std::string
  as_string(const cl_int error_code)
    {
    switch(error_code)
      {
      case clblasSuccess             : return "cl_success";
      case clblasInvalidValue        : return "cl_invalid_value";
      case clblasInvalidCommandQueue : return "cl_invalid_command_queue";
      case clblasInvalidContext      : return "cl_invalid_context";
      case clblasInvalidMemObject    : return "cl_invalid_mem_object";
      case clblasInvalidDevice       : return "cl_invalid_device";
      case clblasInvalidEventWaitList: return "cl_invalid_event_wait_list";
      case clblasOutOfResources      : return "cl_out_of_resources";
      case clblasOutOfHostMemory     : return "cl_out_of_host_memory";
      case clblasInvalidOperation    : return "cl_invalid_operation";
      case clblasCompilerNotAvailable: return "cl_compiler_not_available";
      case clblasBuildProgramFailure : return "cl_build_program_failure";
      // extended codes onwards
      case clblasNotImplemented      : return "functionality is not implemented";
      case clblasNotInitialized      : return "clblas library is not initialized yet";
      case clblasInvalidMatA         : return "matrix A is not a valid memory object";
      case clblasInvalidMatB         : return "matrix B is not a valid memory object";
      case clblasInvalidMatC         : return "matrix C is not a valid memory object";
      case clblasInvalidVecX         : return "vector X is not a valid memory object";
      case clblasInvalidVecY         : return "vector Y is not a valid memory object";
      case clblasInvalidDim          : return "an input dimension (M,N,K) is invalid";
      case clblasInvalidLeadDimA     : return "leading dimension A must not be less than the size of the first dimension";
      case clblasInvalidLeadDimB     : return "leading dimension B must not be less than the size of the second dimension";
      case clblasInvalidLeadDimC     : return "leading dimension C must not be less than the size of the third dimension";
      case clblasInvalidIncX         : return "the increment for a vector X must not be 0";
      case clblasInvalidIncY         : return "the increment for a vector Y must not be 0";
      case clblasInsufficientMemMatA : return "the memory object for Matrix A is too small";
      case clblasInsufficientMemMatB : return "the memory object for Matrix B is too small";
      case clblasInsufficientMemMatC : return "the memory object for Matrix C is too small";
      case clblasInsufficientMemVecX : return "the memory object for Vector X is too small";
      case clblasInsufficientMemVecY : return "the memory object for Vector Y is too small";
      default:                         return "unknown clBLAS error code";
      }
    }
  };


//! @}
