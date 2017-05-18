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


//! \addtogroup gemv
//! @{



template<const bool do_trans_A=false, const bool use_alpha=false, const bool use_beta=false>
class gemv
  {
  public:
  
  template<typename eT>
  inline
  static
  void
  apply( Mat<eT>& y, const Mat<eT>& A, const Mat<eT>& x, const eT alpha = eT(1), const eT beta = eT(0) )
    {
    coot_stop_runtime_error("gemv::apply(): unimplemented for unhandled element type");
    }
  
  
  
  inline
  static
  void
  apply
    (
          Mat<float>& y,
    const Mat<float>& A,
    const Mat<float>& x,
    const float       alpha = float(1),
    const float       beta  = float(0)
    )
    {
    coot_extra_debug_sigprint();
    
    // coot_debug_assert_blas_size(A);  // TODO: adapt this assert for size_t
    
    const clblasTranspose transA = (do_trans_A) ? clblasTrans : clblasNoTrans;
    
    const size_t M = size_t(A.n_rows);
    const size_t N = size_t(A.n_cols);
    
    const float local_alpha = (use_alpha) ? alpha : eT(1);
    const float local_beta  = (use_beta)  ? beta  : eT(0);
    
    const size_t lda = size_t(A.n_rows);
    const size_t inc = size_t(1);
    
    cl_mem A_mem = A.get_dev_mem(false);
    cl_mem x_mem = x.get_dev_mem(false);
    cl_mem y_mem = y.get_dev_mem(false);
    
    cl_command_queue queue = bandicoot_rt.get_cq();
    
    cl_int status = 0;
    
    status |= clblasSgemv(clblasColumnMajor, transA, M, N, local_alpha, A_mem, 0, lda, x_mem, 0, inc, local_beta, y_mem, 0, inc, 1, &queue, 0, NULL, NULL);
    status |= clFlush(queue);
    
    coot_check_cl_error(status, "gemv::apply(): eT = float");
    }
  
  
  
  inline
  static
  void
  apply
    (
          Mat<double>& y,
    const Mat<double>& A,
    const Mat<double>& x,
    const double       alpha = double(1),
    const double       beta  = double(0)
    )
    {
    coot_stop_runtime_error("gemv::apply(): unimplemented for eT = double");
    }
  
  
  
  inline
  static
  void
  apply
    (
          Mat< std::complex<float> >& y,
    const Mat< std::complex<float> >& A,
    const Mat< std::complex<float> >& x,
    const std::complex<float>         alpha = std::complex<float>(1),
    const std::complex<float>         beta  = std::complex<float>(0)
    )
    {
    coot_stop_runtime_error("gemv::apply(): unimplemented for eT = cx_float");
    }
  
  
  
  inline
  static
  void
  apply
    (
          Mat< std::complex<double> >& y,
    const Mat< std::complex<double> >& A,
    const Mat< std::complex<double> >& x,
    const std::complex<double>         alpha = std::complex<double>(1),
    const std::complex<double>         beta  = std::complex<double>(0)
    )
    {
    coot_stop_runtime_error("gemv::apply(): unimplemented for eT = cx_double");
    }


  
  };


//! @}
