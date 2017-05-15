


// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_submatrix(Mat<eT>& X, const uword start_row, const uword start_col, const uword end_row, const uword end_col)
  {
  coot_extra_debug_sigprint();
  
  // NOTE: need to detect if the entire range is selected;
  // if entire range is selected, clEnqueueCopyBufferRect() crashes when using beignet 1.3.0
  
  const uword sub_n_rows = end_row - start_row + 1;
  const uword sub_n_cols = end_col - start_col + 1;
  
  X.set_size(sub_n_rows, sub_n_cols);
  
  // treat the matrix as an image rotated 90 degrees
  // width  of img = number of rows
  // height of img = number of cols
  
  // whoever designed the API for clEnqueueCopyBufferRect() should be permanently removed from the gene pool
  
  // the starting row needs to be multiplied by the size of each element,
  // because khronos folks were too lazy to add a separate "size of element" argument
  
  size_t src_origin[3] = { start_row*sizeof(eT), start_col, 0 };
  size_t dst_origin[3] = { 0, 0, 0 };
  
  // region = 
  // The (width in bytes, height in rows, depth in slices) of the 2D or 3D rectangle being read or written.
  // For a 2D rectangle copy, the depth value given by region[2] should be 1. The values in region cannot be 0. 
  
  size_t region[3] = { X.n_rows*sizeof(eT), X.n_cols, 1 };
  
  size_t src_row_pitch   = sizeof(eT) * n_rows;          // length of each row in bytes to be used for the memory region associated with src_buffer. If src_row_pitch is 0, src_row_pitch is computed as region[0]. 
  size_t src_slice_pitch = sizeof(eT) * n_cols * n_rows; // length of each 2D slice in bytes to be used for the memory region associated with src_buffer. If src_slice_pitch is 0, src_slice_pitch is computed as region[1] * src_row_pitch. 
  
  //size_t dst_row_pitch   = sizeof(eT) * out.n_cols; // length of each row in bytes to be used for the memory region associated with dst_buffer. If dst_row_pitch is 0, dst_row_pitch is computed as region[0].
  //size_t dst_slice_pitch = sizeof(eT) * out.n_cols * out.n_rows; // length of each 2D slice in bytes to be used for the memory region associated with dst_buffer. If dst_slice_pitch is 0, dst_slice_pitch is computed as region[1] * dst_row_pitch.
  
  size_t dst_row_pitch   = 0;
  size_t dst_slice_pitch = 0;
  
  cl_int status = clEnqueueCopyBufferRect(coot_runtime.get_cq(), (*this).get_device_mem(false), X.get_device_mem(false), src_origin, dst_origin, region, src_row_pitch, src_slice_pitch, dst_row_pitch, dst_slice_pitch, 0, NULL, NULL);
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  clFinish(coot_runtime.get_cq());
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::set_submatrix(Mat<eT>& X, const uword start_row, const uword start_col)
  {
  coot_extra_debug_sigprint();
  
  // NOTE: need to detect if the entire range is selected;
  // if entire range is selected, clEnqueueCopyBufferRect() crashes when using beignet 1.3.0
  
  // treat the matrix as an image rotated 90 degrees
  // width  of img = number of rows
  // height of img = number of cols
  
  // whoever designed the API for clEnqueueCopyBufferRect() should be permanently removed from the gene pool
  
  // the starting row needs to be multiplied by the size of each element,
  // because khronos folks were too lazy to add a separate "size of element" argument
  
  size_t src_origin[3] = { 0, 0, 0 };
  size_t dst_origin[3] = { start_row*sizeof(eT), start_col, 0 };
  
  // region = 
  // The (width in bytes, height in rows, depth in slices) of the 2D or 3D rectangle being read or written.
  // For a 2D rectangle copy, the depth value given by region[2] should be 1. The values in region cannot be 0. 
  
  size_t region[3] = { X.n_rows*sizeof(eT), X.n_cols, 1 };
  
  size_t src_row_pitch   = 0; // length of each row in bytes to be used for the memory region associated with src_buffer. If src_row_pitch is 0, src_row_pitch is computed as region[0]. 
  size_t src_slice_pitch = 0; // length of each 2D slice in bytes to be used for the memory region associated with src_buffer. If src_slice_pitch is 0, src_slice_pitch is computed as region[1] * src_row_pitch. 
  
  size_t dst_row_pitch   = sizeof(eT) * n_rows;          // length of each row in bytes to be used for the memory region associated with dst_buffer. If dst_row_pitch is 0, dst_row_pitch is computed as region[0].
  size_t dst_slice_pitch = sizeof(eT) * n_cols * n_rows; // length of each 2D slice in bytes to be used for the memory region associated with dst_buffer. If dst_slice_pitch is 0, dst_slice_pitch is computed as region[1] * dst_row_pitch.
  
  cl_int status = clEnqueueCopyBufferRect(coot_runtime.get_cq(), X.get_device_mem(false), (*this).get_device_mem(false), src_origin, dst_origin, region, src_row_pitch, src_slice_pitch, dst_row_pitch, dst_slice_pitch, 0, NULL, NULL);
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  clFinish(coot_runtime.get_cq());
  }






// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_all_1(Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  X.set_size(1, 1);
  
  cl_kernel kernel = coot_runtime.get_kernel<float>(kernel_id::sum_all);
  
  cl_int status = 0;
  
  cl_mem X_mem =       X.get_device_mem(false);
  cl_mem A_mem = (*this).get_device_mem(false);
  
  coot_runtime_t::adapt_uword start(0);
  coot_runtime_t::adapt_uword end  (n_elem-1);
  
  status = clSetKernelArg(kernel, 0, sizeof(cl_mem),   &X_mem);
  status = clSetKernelArg(kernel, 1, sizeof(cl_mem),   &A_mem);
  status = clSetKernelArg(kernel, 2, start.size,       start.addr);
  status = clSetKernelArg(kernel, 3, start.size,         end.addr);
  
  coot_check_cl_error(status, "get_sum_all_1(): clSetKernelArg()");
  
  const size_t  work_dim = 1;
  const size_t  global_work_offset[1] = { 0 };
  const size_t  global_work_size[1]   = { 1 };
  const size_t* local_work_size = NULL;
  
  status = clEnqueueNDRangeKernel(coot_runtime.get_cq(), kernel, work_dim, global_work_offset, global_work_size, local_work_size, 0, NULL, NULL);
  
  coot_check_cl_error(status, "get_sum_all_1(): clEnqueueNDRangeKernel()");
  
  clFinish(coot_runtime.get_cq());
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_all_2(Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  // (*this).print("blah:");
  
  Mat<eT> tmp;
  (*this).get_sum_colwise(tmp);
  
  // tmp.print("tmp:");
  
  tmp.get_sum_all_1(X);
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_all_3(Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  const cl_uint n_units = coot_runtime.get_n_compunits();
  
  // create a fake matrix which is not so thin or thick
  
  // examine integers between 2 and sqrt(N);
  // smallest factor must lie between those two;
  // largest factor is simply N / the smallest factor.
  
  // First thought you can find the smallest divisor d (not equal to 1 of course),
  // then N/d will be the largest divisor you're looking for.
  // For example if N is divisible by 3 then you'll need 2 iterations to find the answer
  //
  // To further improve your algorithm you can iterate through odd numbers only
  // (after checking if you number is even) or, even better,
  // if you have the list of primes pre-calculated then you can iterate
  // through them only because smallest divisor is obviously is a prime number.

  uword fake_n_rows = n_rows;
  uword fake_n_cols = n_cols;
  
  if( (n_rows > 64) && (n_cols < n_units) )
    {
    if((n_rows % 2) == 0)
      {
      fake_n_rows = n_rows / 2;
      fake_n_cols = n_cols * 2;
      }
    else
      {
      // can use precalculated primes instead of going through every odd value
      const uword sqrt_n_rows = uword(std::floor(std::sqrt(double(n_rows))));
      
      uword divisor = 0;
      
      for(uword i=3; i <= sqrt_n_rows; i+=2)
        {
        if((n_rows % i) == 0)  { divisor = i; break; }
        }
      
      if(divisor != 0)
        {
        fake_n_rows = n_rows / divisor;
        fake_n_cols = n_cols * divisor;
        }
      }
    }
  else
  if( (n_rows <= 64) && (n_cols >= n_units) )
    {
    if((n_cols % 2) == 0)
      {
      fake_n_rows = n_rows * 2;
      fake_n_cols = n_cols / 2;
      }
    else
      {
      // can use precalculated primes instead of going through every odd value
      const uword sqrt_n_cols = uword(std::floor(std::sqrt(double(n_cols))));
      
      uword divisor = 0;
      
      for(uword i=3; i <= sqrt_n_cols; i+=2)
        {
        if((n_cols % i) == 0)  { divisor = i; break; }
        }
      
      if(divisor != 0)
        {
        fake_n_rows = n_rows * divisor;
        fake_n_cols = n_cols / divisor;
        }
      }
    }
  
  
  // make sure there is enough to process in each column
  if(fake_n_rows <= 64)
    {
    fake_n_rows = n_rows;
    fake_n_cols = n_cols;
    }
  
  
  // sanity check
  
  if( (fake_n_rows * fake_n_cols) != (n_rows * n_cols) )
    {
    cout << "sanity check failed: orig: " << n_rows << "x" << n_cols << "  proposed: " << fake_n_rows << "x" << fake_n_cols << endl;
    fake_n_rows = n_rows;
    fake_n_cols = n_cols;
    }
  
  Mat<eT> fake(get_device_mem(false), fake_n_rows, fake_n_cols);
  
  Mat<eT> tmp;
  fake.get_sum_colwise(tmp);
  
  tmp.get_sum_all_1(X);
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_colwise(Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  X.set_size(1, (*this).n_cols);
  
  cl_kernel kernel = coot_runtime.get_kernel<float>(kernel_id::sum_colwise);
  
  cl_int status = 0;
  
  cl_mem X_mem =       X.get_device_mem(false);
  cl_mem A_mem = (*this).get_device_mem(false);
  
  coot_runtime_t::adapt_uword local_n_rows(n_rows);
  coot_runtime_t::adapt_uword local_n_cols(n_cols);
  
  status = clSetKernelArg(kernel, 0, sizeof(cl_mem),    &X_mem       );
  status = clSetKernelArg(kernel, 1, sizeof(cl_mem),    &A_mem       );
  status = clSetKernelArg(kernel, 2, local_n_rows.size, local_n_rows.addr);
  status = clSetKernelArg(kernel, 3, local_n_cols.size, local_n_cols.addr);

  coot_check_cl_error(status, "get_sum_colwise(): clSetKernelArg()");
  
  const size_t  work_dim = 1;
  const size_t  global_work_offset[1] = { 0      };
  const size_t  global_work_size[1]   = { n_cols };
  const size_t* local_work_size = NULL;
  //const size_t local_work_size[1] = { 512 };
  // local_work_size = number of work-items in a work-group, in each dimension
  // a work-group is a combination of work-items;
  // each work-group executes on a single compute-unit;
  // each compute-unit executes only one work-group at a time
  
  // beignet 1.3, on HD Graphics 5500 BroadWell U-Processor GT2
  // Max work item sizes  512x512x512
  // Max work group size  512
  
  status = clEnqueueNDRangeKernel(coot_runtime.get_cq(), kernel, work_dim, global_work_offset, global_work_size, local_work_size, 0, NULL, NULL);
  
  coot_check_cl_error(status, "get_sum_colwise(): clEnqueueNDRangeKernel()");
  
  clFinish(coot_runtime.get_cq());
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_rowwise(Mat<eT>& X)
  {
  coot_extra_debug_sigprint();
  
  X.set_size((*this).n_rows, 1);
  
  cl_kernel kernel = coot_runtime.get_kernel<float>(kernel_id::sum_rowwise);
  
  cl_int status = 0;
  
  cl_mem X_mem =       X.get_device_mem(false);
  cl_mem A_mem = (*this).get_device_mem(false);
  
  coot_runtime_t::adapt_uword local_n_rows(n_rows);
  coot_runtime_t::adapt_uword local_n_cols(n_cols);
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem),       &X_mem       );
  status |= clSetKernelArg(kernel, 1, sizeof(cl_mem),       &A_mem       );
  status = clSetKernelArg(kernel, 2, local_n_rows.size, local_n_rows.addr);
  status = clSetKernelArg(kernel, 3, local_n_cols.size, local_n_cols.addr);

  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  const size_t  work_dim = 1;
  const size_t  global_work_offset[1] = { 0      };
  const size_t  global_work_size[1]   = { n_rows };
  const size_t* local_work_size = NULL;
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_cq(), kernel, work_dim, global_work_offset, global_work_size, local_work_size, 0, NULL, NULL);
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  clFinish(coot_runtime.get_cq());
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_submat_colwise(Mat<eT>& X, const uword start_row, const uword start_col, const uword end_row, const uword end_col)
  {
  coot_extra_debug_sigprint();
  
  const uword sub_n_rows = end_row - start_row + 1;
  const uword sub_n_cols = end_col - start_col + 1;

  X.set_size(1, sub_n_cols);
  X.fill(-1);
  
  cl_kernel kernel = coot_runtime.get_kernel<float>(kernel_id::submat_sum_colwise);
  
  cl_int status = 0;
  
  cl_mem X_mem =       X.get_device_mem(false);
  cl_mem A_mem = (*this).get_device_mem(false);
  
  unsigned int local_n_rows = n_rows;
  unsigned int local_n_cols = n_cols;
  
  unsigned int local_start_col = start_col;
  unsigned int local_start_row = start_row;
  unsigned int local_end_row   = end_row;
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem),       &X_mem          );
  status |= clSetKernelArg(kernel, 1, sizeof(cl_mem),       &A_mem          );
  status |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &local_n_rows   );
  status |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &local_n_cols   );
  status |= clSetKernelArg(kernel, 4, sizeof(unsigned int), &local_start_col);
  status |= clSetKernelArg(kernel, 5, sizeof(unsigned int), &local_start_row);
  status |= clSetKernelArg(kernel, 6, sizeof(unsigned int), &local_end_row  );
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  const size_t  work_dim = 1;
  const size_t  global_work_offset[1] = { start_col  };  // starting column in original matrix
  const size_t  global_work_size[1]   = { sub_n_cols };  // number of columns in the submatrix
  const size_t* local_work_size = NULL;
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_cq(), kernel, work_dim, global_work_offset, global_work_size, local_work_size, 0, NULL, NULL);
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  clFinish(coot_runtime.get_cq());
  }



// BUG: for experimental purposes only
template<typename eT>
inline
void
Mat<eT>::get_sum_submat_rowwise(Mat<eT>& X, const uword start_row, const uword start_col, const uword end_row, const uword end_col)
  {
  coot_extra_debug_sigprint();
  
  const uword sub_n_rows = end_row - start_row + 1;
  const uword sub_n_cols = end_col - start_col + 1;

  X.set_size(sub_n_rows, 1);
  
  cl_kernel kernel = coot_runtime.get_kernel<float>(kernel_id::submat_sum_rowwise);
  
  cl_int status = 0;
  
  cl_mem X_mem =       X.get_device_mem(false);
  cl_mem A_mem = (*this).get_device_mem(false);
  
  unsigned int local_n_rows = n_rows;
  unsigned int local_n_cols = n_cols;
  
  unsigned int local_start_row = start_row;
  unsigned int local_start_col = start_col;
  unsigned int local_end_col   = end_col;
  
  status |= clSetKernelArg(kernel, 0, sizeof(cl_mem),       &X_mem          );
  status |= clSetKernelArg(kernel, 1, sizeof(cl_mem),       &A_mem          );
  status |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &local_n_rows   );
  status |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &local_n_cols   );
  status |= clSetKernelArg(kernel, 4, sizeof(unsigned int), &local_start_row);
  status |= clSetKernelArg(kernel, 5, sizeof(unsigned int), &local_start_col);
  status |= clSetKernelArg(kernel, 6, sizeof(unsigned int), &local_end_col  );
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  const size_t  work_dim = 1;
  const size_t  global_work_offset[1] = { start_row  };
  const size_t  global_work_size[1]   = { sub_n_rows };
  const size_t* local_work_size = NULL;
  
  status |= clEnqueueNDRangeKernel(coot_runtime.get_cq(), kernel, work_dim, global_work_offset, global_work_size, local_work_size, 0, NULL, NULL);
  
  if(status != CL_SUCCESS)
    {
    cout << "status: " << status << endl;
    }
  
  clFinish(coot_runtime.get_cq());
  }



