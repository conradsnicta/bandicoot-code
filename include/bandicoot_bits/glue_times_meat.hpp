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


//! \addtogroup glue_times
//! @{



template<uword N>
template<typename T1, typename T2>
inline
void
glue_times_redirect<N>::apply(Mat<typename T1::elem_type>& out, const Glue<T1,T2,glue_times>& X)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  const partial_unwrap<T1> tmp1(X.A);
  const partial_unwrap<T2> tmp2(X.B);
  
  const typename partial_unwrap<T1>::stored_type& A = tmp1.M;
  const typename partial_unwrap<T2>::stored_type& B = tmp2.M;
  
  const bool use_alpha = partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times;
  const eT       alpha = use_alpha ? (tmp1.get_val() * tmp2.get_val()) : eT(0);
  
  const bool alias = tmp1.is_alias(out) || tmp2.is_alias(out);
  
  if(alias == false)
    {
    glue_times::apply
      <
      eT,
      partial_unwrap<T1>::do_trans,
      partial_unwrap<T2>::do_trans,
      (partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times)
      >
      (out, A, B, alpha);
    }
  else
    {
    Mat<eT> tmp;
    
    glue_times::apply
      <
      eT,
      partial_unwrap<T1>::do_trans,
      partial_unwrap<T2>::do_trans,
      (partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times)
      >
      (tmp, A, B, alpha);
    
    out.steal_mem(tmp);
    }
  }



template<typename T1, typename T2>
inline
void
glue_times_redirect<2>::apply(Mat<typename T1::elem_type>& out, const Glue<T1,T2,glue_times>& X)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  const partial_unwrap<T1> tmp1(X.A);
  const partial_unwrap<T2> tmp2(X.B);
  
  const typename partial_unwrap<T1>::stored_type& A = tmp1.M;
  const typename partial_unwrap<T2>::stored_type& B = tmp2.M;
  
  const bool use_alpha = partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times;
  const eT       alpha = use_alpha ? (tmp1.get_val() * tmp2.get_val()) : eT(0);
  
  const bool alias = tmp1.is_alias(out) || tmp2.is_alias(out);
  
  if(alias == false)
    {
    glue_times::apply
      <
      eT,
      partial_unwrap<T1>::do_trans,
      partial_unwrap<T2>::do_trans,
      (partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times)
      >
      (out, A, B, alpha);
    }
  else
    {
    Mat<eT> tmp;
    
    glue_times::apply
      <
      eT,
      partial_unwrap<T1>::do_trans,
      partial_unwrap<T2>::do_trans,
      (partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times)
      >
      (tmp, A, B, alpha);
    
    out.steal_mem(tmp);
    }
  }



template<typename T1, typename T2, typename T3>
inline
void
glue_times_redirect<3>::apply(Mat<typename T1::elem_type>& out, const Glue< Glue<T1,T2,glue_times>, T3, glue_times>& X)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  // we have exactly 3 objects
  // hence we can safely expand X as X.A.A, X.A.B and X.B
  
  const partial_unwrap<T1> tmp1(X.A.A);
  const partial_unwrap<T2> tmp2(X.A.B);
  const partial_unwrap<T3> tmp3(X.B  );
  
  const typename partial_unwrap<T1>::stored_type& A = tmp1.M;
  const typename partial_unwrap<T2>::stored_type& B = tmp2.M;
  const typename partial_unwrap<T3>::stored_type& C = tmp3.M;
  
  const bool use_alpha = partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times || partial_unwrap<T3>::do_times;
  const eT       alpha = use_alpha ? (tmp1.get_val() * tmp2.get_val() * tmp3.get_val()) : eT(0);
  
  const bool alias = tmp1.is_alias(out) || tmp2.is_alias(out) || tmp3.is_alias(out);
  
  if(alias == false)
    {
    glue_times::apply
      <
      eT,
      partial_unwrap<T1>::do_trans,
      partial_unwrap<T2>::do_trans,
      partial_unwrap<T3>::do_trans,
      (partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times || partial_unwrap<T3>::do_times)
      >
      (out, A, B, C, alpha);
    }
  else
    {
    Mat<eT> tmp;
    
    glue_times::apply
      <
      eT,
      partial_unwrap<T1>::do_trans,
      partial_unwrap<T2>::do_trans,
      partial_unwrap<T3>::do_trans,
      (partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times || partial_unwrap<T3>::do_times)
      >
      (tmp, A, B, C, alpha);
    
    out.steal_mem(tmp);
    }
  }



template<typename T1, typename T2, typename T3, typename T4>
inline
void
glue_times_redirect<4>::apply(Mat<typename T1::elem_type>& out, const Glue< Glue< Glue<T1,T2,glue_times>, T3, glue_times>, T4, glue_times>& X)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  // there is exactly 4 objects
  // hence we can safely expand X as X.A.A.A, X.A.A.B, X.A.B and X.B
  
  const partial_unwrap<T1> tmp1(X.A.A.A);
  const partial_unwrap<T2> tmp2(X.A.A.B);
  const partial_unwrap<T3> tmp3(X.A.B  );
  const partial_unwrap<T4> tmp4(X.B    );
  
  const typename partial_unwrap<T1>::stored_type& A = tmp1.M;
  const typename partial_unwrap<T2>::stored_type& B = tmp2.M;
  const typename partial_unwrap<T3>::stored_type& C = tmp3.M;
  const typename partial_unwrap<T4>::stored_type& D = tmp4.M;
  
  const bool use_alpha = partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times || partial_unwrap<T3>::do_times || partial_unwrap<T4>::do_times;
  const eT       alpha = use_alpha ? (tmp1.get_val() * tmp2.get_val() * tmp3.get_val() * tmp4.get_val()) : eT(0);
  
  const bool alias = tmp1.is_alias(out) || tmp2.is_alias(out) || tmp3.is_alias(out) || tmp4.is_alias(out);
  
  if(alias == false)
    {
    glue_times::apply
      <
      eT,
      partial_unwrap<T1>::do_trans,
      partial_unwrap<T2>::do_trans,
      partial_unwrap<T3>::do_trans,
      partial_unwrap<T4>::do_trans,
      (partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times || partial_unwrap<T3>::do_times || partial_unwrap<T4>::do_times)
      >
      (out, A, B, C, D, alpha);
    }
  else
    {
    Mat<eT> tmp;
    
    glue_times::apply
      <
      eT,
      partial_unwrap<T1>::do_trans,
      partial_unwrap<T2>::do_trans,
      partial_unwrap<T3>::do_trans,
      partial_unwrap<T4>::do_trans,
      (partial_unwrap<T1>::do_times || partial_unwrap<T2>::do_times || partial_unwrap<T3>::do_times || partial_unwrap<T4>::do_times)
      >
      (tmp, A, B, C, D, alpha);
    
    out.steal_mem(tmp);
    }
  }



template<typename T1, typename T2>
inline
void
glue_times::apply(Mat<typename T1::elem_type>& out, const Glue<T1,T2,glue_times>& X)
  {
  coot_extra_debug_sigprint();
  
  const sword N_mat = 1 + depth_lhs< glue_times, Glue<T1,T2,glue_times> >::num;
  
  coot_extra_debug_print(coot_str::format("N_mat = %d") % N_mat);
  
  glue_times_redirect<N_mat>::apply(out, X);
  }



template<typename eT, const bool do_trans_A, const bool do_trans_B>
inline
uword
glue_times::mul_storage_cost(const Mat<eT>& A, const Mat<eT>& B)
  {
  const uword final_A_n_rows = (do_trans_A == false) ? A.n_rows : A.n_cols;
  const uword final_B_n_cols = (do_trans_B == false) ? B.n_cols : B.n_rows;
  
  return final_A_n_rows * final_B_n_cols;
  }



template
  <
  typename   eT,
  const bool do_trans_A,
  const bool do_trans_B,
  const bool use_alpha
  >
inline
void
glue_times::apply
  (
        Mat<eT>& out,
  const Mat<eT>& A,
  const Mat<eT>& B,
  const eT       alpha
  )
  {
  coot_extra_debug_sigprint();
  
  coot_debug_assert_trans_mul_size<do_trans_A, do_trans_B>(A.n_rows, A.n_cols, B.n_rows, B.n_cols, "matrix multiplication");
  
  const uword final_n_rows = (do_trans_A == false) ? A.n_rows : A.n_cols;
  const uword final_n_cols = (do_trans_B == false) ? B.n_cols : B.n_rows;
  
  out.set_size(final_n_rows, final_n_cols);
  
  if( (A.n_elem == 0) || (B.n_elem == 0) )
    {
    out.zeros();
    return;
    }
  
  
  if( (do_trans_A == false) && (do_trans_B == false) && (use_alpha == false) )
    {
         if( (A.n_rows == 1) && (is_cx<eT>::no) )  { gemv<true,         false, false>::apply(out, B, A); }
    else if( (B.n_cols == 1)                    )  { gemv<false,        false, false>::apply(out, A, B); }
    else                                           { gemm<false, false, false, false>::apply(out, A, B); }
    }
  else
  if( (do_trans_A == false) && (do_trans_B == false) && (use_alpha == true) )
    {
         if( (A.n_rows == 1) && (is_cx<eT>::no) )  { gemv<true,         true, false>::apply(out, B, A, alpha); }
    else if( (B.n_cols == 1)                    )  { gemv<false,        true, false>::apply(out, A, B, alpha); }
    else                                           { gemm<false, false, true, false>::apply(out, A, B, alpha); }
    }
  else
  if( (do_trans_A == true) && (do_trans_B == false) && (use_alpha == false) )
    {
         if( (A.n_cols == 1) && (is_cx<eT>::no) )  { gemv<true,        false, false>::apply(out, B, A); }
    else if( (B.n_cols == 1)                    )  { gemv<true,        false, false>::apply(out, A, B); }
    else                                           { gemm<true, false, false, false>::apply(out, A, B); }
    }
  else
  if( (do_trans_A == true) && (do_trans_B == false) && (use_alpha == true) )
    {
         if( (A.n_cols == 1) && (is_cx<eT>::no) )  { gemv<true,        true, false>::apply(out, B, A, alpha); }
    else if( (B.n_cols == 1)                    )  { gemv<true,        true, false>::apply(out, A, B, alpha); }
    else                                           { gemm<true, false, true, false>::apply(out, A, B, alpha); }
    }
  else
  if( (do_trans_A == false) && (do_trans_B == true) && (use_alpha == false) )
    {
         if( (A.n_rows == 1) && (is_cx<eT>::no)  )  { gemv<false,       false, false>::apply(out, B, A); }
    else if( (B.n_rows == 1) && (is_cx<eT>::no)  )  { gemv<false,       false, false>::apply(out, A, B); }
    else                                            { gemm<false, true, false, false>::apply(out, A, B); }
    }
  else
  if( (do_trans_A == false) && (do_trans_B == true) && (use_alpha == true) )
    {
         if( (A.n_rows == 1) && (is_cx<eT>::no) ) { gemv<false,       true, false>::apply(out, B, A, alpha); }
    else if( (B.n_rows == 1) && (is_cx<eT>::no) ) { gemv<false,       true, false>::apply(out, A, B, alpha); }
    else                                          { gemm<false, true, true, false>::apply(out, A, B, alpha); }
    }
  else
  if( (do_trans_A == true) && (do_trans_B == true) && (use_alpha == false) )
    {
         if( (A.n_cols == 1) && (is_cx<eT>::no) )  { gemv<false,      false, false>::apply(out, B, A); }
    else if( (B.n_rows == 1) && (is_cx<eT>::no) )  { gemv<true,       false, false>::apply(out, A, B); }
    else                                           { gemm<true, true, false, false>::apply(out, A, B); }
    }
  else
  if( (do_trans_A == true) && (do_trans_B == true) && (use_alpha == true) )
    {
         if( (A.n_cols == 1) && (is_cx<eT>::no) )  { gemv<false,      true, false>::apply(out, B, A, alpha); }
    else if( (B.n_rows == 1) && (is_cx<eT>::no) )  { gemv<true,       true, false>::apply(out, A, B, alpha); }
    else                                           { gemm<true, true, true, false>::apply(out, A, B, alpha); }
    }
  }



template
  <
  typename   eT,
  const bool do_trans_A,
  const bool do_trans_B,
  const bool do_trans_C,
  const bool use_alpha
  >
inline
void
glue_times::apply
  (
        Mat<eT>& out,
  const Mat<eT>& A,
  const Mat<eT>& B,
  const Mat<eT>& C,
  const eT       alpha
  )
  {
  coot_extra_debug_sigprint();
  
  Mat<eT> tmp;
  
  const uword storage_cost_AB = glue_times::mul_storage_cost<eT, do_trans_A, do_trans_B>(A, B);
  const uword storage_cost_BC = glue_times::mul_storage_cost<eT, do_trans_B, do_trans_C>(B, C);
  
  if(storage_cost_AB <= storage_cost_BC)
    {
    // out = (A*B)*C
    
    glue_times::apply<eT, do_trans_A, do_trans_B, use_alpha>(tmp, A,   B, alpha);
    glue_times::apply<eT, false,      do_trans_C, false    >(out, tmp, C, eT(0));
    }
  else
    {
    // out = A*(B*C)
    
    glue_times::apply<eT, do_trans_B, do_trans_C, use_alpha>(tmp, B, C,   alpha);
    glue_times::apply<eT, do_trans_A, false,      false    >(out, A, tmp, eT(0));
    }
  }



template
  <
  typename   eT,
  const bool do_trans_A,
  const bool do_trans_B,
  const bool do_trans_C,
  const bool do_trans_D,
  const bool use_alpha
  >
inline
void
glue_times::apply
  (
        Mat<eT>& out,
  const Mat<eT>& A,
  const Mat<eT>& B,
  const Mat<eT>& C,
  const Mat<eT>& D,
  const eT       alpha
  )
  {
  coot_extra_debug_sigprint();
  
  Mat<eT> tmp;
  
  const uword storage_cost_AC = glue_times::mul_storage_cost<eT, do_trans_A, do_trans_C>(A, C);
  const uword storage_cost_BD = glue_times::mul_storage_cost<eT, do_trans_B, do_trans_D>(B, D);
  
  if(storage_cost_AC <= storage_cost_BD)
    {
    // out = (A*B*C)*D
    
    glue_times::apply<eT, do_trans_A, do_trans_B, do_trans_C, use_alpha>(tmp, A, B, C, alpha);
    
    glue_times::apply<eT, false, do_trans_D, false>(out, tmp, D, eT(0));
    }
  else
    {
    // out = A*(B*C*D)
    
    glue_times::apply<eT, do_trans_B, do_trans_C, do_trans_D, use_alpha>(tmp, B, C, D, alpha);
    
    glue_times::apply<eT, do_trans_A, false, false>(out, A, tmp, eT(0));
    }
  }



//! @}
