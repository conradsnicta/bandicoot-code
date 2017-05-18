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


//! \addtogroup gemm
//! @{



template<const bool do_trans_A=false, const bool do_trans_B=false, const bool use_alpha=false, const bool use_beta=false>
class gemm
  {
  public:
  
  template<typename eT>
  inline
  static
  void
  apply( Mat<eT>& C, const Mat<eT>& A, const Mat<eT>& B, const eT alpha = eT(1), const eT beta = eT(0) )
    {
    coot_stop_runtime_error("gemm::apply(): unimplemented for unhandled element type");
    }
  
  
  
  inline
  static
  void
  apply
    (
          Mat<float>& C,
    const Mat<float>& A,
    const Mat<float>& B,
    const float alpha = float(1),
    const float beta  = float(0)
    )
    {
    coot_extra_debug_sigprint();
    
    // coot_debug_assert_blas_size(A,B);  // TODO: adapt this assert for size_t
    
    const clblasTranspose transA = (do_trans_A) ? clblasTrans : clblasNoTrans;
    const clblasTranspose transB = (do_trans_B) ? clblasTrans : clblasNoTrans;
    
    const size_t M = size_t(C.n_rows);
    const size_t N = size_t(C.n_cols);
    const size_t K = (do_trans_A) ? size_t(A.n_rows) : size_t(A.n_cols);
    
    const float local_alpha = (use_alpha) ? alpha : eT(1);
    
    const size_t lda = (do_trans_A) ? K : M;
    const size_t ldb = (do_trans_B) ? N : K;
    const size_t ldc = size_t(C.n_rows);
    
    const eT local_beta = (use_beta) ? beta : eT(0);
    
    cl_mem A_mem = A.get_dev_mem(false);
    cl_mem B_mem = B.get_dev_mem(false);
    cl_mem C_mem = C.get_dev_mem(false);
    
    cl_command_queue queue = bandicoot_rt.get_cq();
    
    cl_int status = 0;
    
    status |= clblasSgemm(clblasColumnMajor, transA, transB, M, N, K, local_alpha, A_mem, 0, lda, B_mem, 0, ldb, local_beta, C_mem, 0, ldc, 1, &queue, 0, NULL, NULL);
    status |= clFlush(queue);
    
    coot_check_cl_error(status, "gemm::apply(): eT = float");
    }
  
  
  
  inline
  static
  void
  apply
    (
          Mat<double>& C,
    const Mat<double>& A,
    const Mat<double>& B,
    const double alpha = double(1),
    const double beta  = double(0)
    )
    {
    coot_stop_runtime_error("gemm::apply(): unimplemented for eT = double");
    }
  
  
  
  inline
  static
  void
  apply
    (
          Mat< std::complex<float> >& C,
    const Mat< std::complex<float> >& A,
    const Mat< std::complex<float> >& B,
    const std::complex<float> alpha = std::complex<float>(1),
    const std::complex<float> beta  = std::complex<float>(0)
    )
    {
    coot_stop_runtime_error("gemm::apply(): unimplemented for eT = cx_float");
    }
  
  
  
  inline
  static
  void
  apply
    (
          Mat< std::complex<double> >& C,
    const Mat< std::complex<double> >& A,
    const Mat< std::complex<double> >& B,
    const std::complex<double> alpha = std::complex<double>(1),
    const std::complex<double> beta  = std::complex<double>(0)
    )
    {
    coot_stop_runtime_error("gemm::apply(): unimplemented for eT = cx_double");
    }
  
  };



//! @}
