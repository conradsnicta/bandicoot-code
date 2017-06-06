#include <iostream>
#include <bandicoot>

using namespace std;
using namespace coot;

int
main(int argc, char** argv)
  {
  coot::coot_rt.init(true);
  cout << coot::coot_version::as_string() << endl;
  
  if(argc < 3)
    {
    cout << "usage: " << argv[0] << " nrows ncols" << endl;
    return -1;
    }
  
  uword nrows = atoi(argv[1]);
  uword ncols = atoi(argv[2]);
  
  cout << "nrows: " << nrows << endl;
  cout << "ncols: " << ncols << endl;
  
  coot::fmat A(nrows, ncols); 
  A.fill(2);

  std::string kernel = generate_kernel(-A);

  std::cout << "Kernel:\n\n" << kernel;

  kernel = generate_kernel(-(-A));

  std::cout << "\n\nKernel for -(-A):\n\n" << kernel;

  kernel = generate_kernel(-(sqrt(-(-A))));

  std::cout << "\n\nKernel for -(sqrt(-(-A))):\n\n" << kernel;

  coot::fmat out(nrows, ncols);

  // Get the kernel.
  cl_kernel k = coot_rt.build_kernel<float>("elem_negsqrtnegneg", kernel);

  // Now run the kernel.
  cl_mem out_dev_mem = out.get_dev_mem(false);
  cl_mem A_dev_mem = A.get_dev_mem(false);

  uword n_elem = out.get_n_elem();
  coot_rt_t::adapt_uword N(n_elem);

  cl_int status = 0;
  status |= clSetKernelArg(k, 0, sizeof(cl_mem), &out_dev_mem);
  status |= clSetKernelArg(k, 1, sizeof(cl_mem), &A_dev_mem);
  status |= clSetKernelArg(k, 2, N.size, N.addr);

  size_t work_size = size_t(n_elem);

  status |= clEnqueueNDRangeKernel(coot_rt.get_cq(), k, 1, NULL,
      &work_size, NULL, 0, NULL, NULL);

  coot_check_runtime_error((status != CL_SUCCESS),
      "eop_generate: couldn't execute kernel");

  A.print("A");
  out.print("out");

  return 0;
  }
