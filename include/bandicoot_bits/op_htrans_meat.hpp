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


//! \addtogroup op_htrans
//! @{



template<typename T1>
inline 
void
op_htrans::apply(Mat<typename T1::elem_type>& out, const Op<T1,op_htrans>& in)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  const unwrap<T1> U(in.m);
  
  if(U.is_alias(out))
    {
    Mat<eT> tmp;
    
    op_htrans::apply_noalias(tmp, U.M);
    
    out.steal_mem(tmp);
    }
  else
    {
    op_htrans::apply_noalias(out, U.M);
    }
  }



template<typename eT>
inline
void
op_htrans::apply_noalias(Mat<eT>& out, const Mat<eT>& A, const typename coot_not_cx<eT>::result* junk)
  {
  coot_extra_debug_sigprint();
  
  coot_stop_runtime_error("op_htrans: not implemented");
  }



template<typename eT>
inline
void
op_htrans::apply_noalias(Mat<eT>& out, const Mat<eT>& A, const typename coot_cx_only<eT>::result* junk)
  {
  coot_extra_debug_sigprint();
  
  coot_stop_runtime_error("op_htrans: not implemented");
  }



//



template<typename T1>
inline 
void
op_htrans2::apply(Mat<typename T1::elem_type>& out, const Op<T1,op_htrans2>& in)
  {
  coot_extra_debug_sigprint();
  
  typedef typename T1::elem_type eT;
  
  const unwrap<T1> U(in.m);
  
  if(U.is_alias(out))
    {
    Mat<eT> tmp;
    
    op_htrans2::apply_noalias(tmp, U.M);
    
    out.steal_mem(tmp);
    }
  else
    {
    op_htrans2::apply_noalias(out, U.M);
    }
  }



template<typename eT>
inline
void
op_htrans2::apply_noalias(Mat<eT>& out, const Mat<eT>& A, const typename coot_not_cx<eT>::result* junk)
  {
  coot_extra_debug_sigprint();
  
  coot_stop_runtime_error("op_htrans2: not implemented");
  }



template<typename eT>
inline
void
op_htrans2::apply_noalias(Mat<eT>& out, const Mat<eT>& A, const typename coot_cx_only<eT>::result* junk)
  {
  coot_extra_debug_sigprint();
  
  coot_stop_runtime_error("op_htrans2: not implemented");
  }



//! @}
