/**
 * generate a kernel for elementwise operations
 */

template<typename T1>
inline std::string generate_name(const T1&)
  {
  return "";
  }

template<typename T1, typename eop_type>
inline std::string generate_name(const eOp<T1, eop_type>& a)
  {
  return eop_name<eop_type>() + generate_name(a.m);
  }

template<typename T1>
inline std::string generate_operation(const T1&)
  {
  return "A[i]";
  }

template<typename T1, typename eop_type>
inline std::string generate_operation(const eOp<T1, eop_type>& a)
  {
  return eop_op<eop_type>() + "(" + generate_operation(a.m) + ")";
  }

template<typename T1>
inline std::string generate_kernel(T1& t)
  {
  // First generate the name...
  std::string name = generate_name(t);

  std::string operation = generate_operation(t);

  std::string kernel =
      "#ifdef cl_khr_pragma_unroll \n"
      "#pragma OPENCL EXTENSION cl_khr_pragma_unroll : enable \n"
      "#endif \n"
      "#ifdef cl_amd_pragma_unroll \n"
      "#pragma OPENCL EXTENSION cl_amd_pragma_unroll : enable \n"
      "#endif \n"
      "#ifdef cl_nv_pragma_unroll \n"
      "#pragma OPENCL EXTENSION cl_nv_pragma_unroll : enable \n"
      "#endif \n"
      "#ifdef cl_intel_pragma_unroll \n"
      "#pragma OPENCL EXTENSION cl_intel_pragma_unroll : enable \n"
      "#endif \n"
      "\n"
      "#define COOT_FN2(ARG1,ARG2)  ARG1 ## ARG2 \n"
      "#define COOT_FN(ARG1,ARG2) COOT_FN2(ARG1,ARG2) \n"
      "\n"
      "__kernel void COOT_FN(PREFIX,elem_" + name + ")(__global eT* out, __global const eT* A, const UWORD N)\n"
      "  {\n"
      "  const UWORD i = get_global_id(0);\n"
      "  if (i < N) { out[i] = " + operation + "; }\n"
      "  }\n";

  return kernel;
  }
