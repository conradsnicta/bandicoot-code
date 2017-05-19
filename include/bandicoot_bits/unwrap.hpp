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


//! \addtogroup unwrap
//! @{



template<typename T1>
struct unwrap
  {
  typedef typename T1::elem_type eT;
  typedef Mat<eT>                stored_type;
  
  inline
  unwrap(const T1& A)
    : M(A)
    {
    coot_extra_debug_sigprint();
    }
  
  const Mat<eT> M;
  
  template<typename eT2>
  coot_inline bool is_alias(const Mat<eT2>&) const { return false; }
  };



template<typename eT>
struct unwrap< Mat<eT> >
  {
  typedef Mat<eT> stored_type;
  
  inline
  unwrap(const Mat<eT>& A)
    : M(A)
    {
    coot_extra_debug_sigprint();
    }
  
  const Mat<eT>& M;
  
  template<typename eT2>
  coot_inline bool is_alias(const Mat<eT2>& X) const { return (void_ptr(&M) == void_ptr(&X)); }
  };



template<typename eT>
struct unwrap< Row<eT> >
  {
  typedef Row<eT> stored_type;
  
  inline
  unwrap(const Row<eT>& A)
    : M(A)
    {
    coot_extra_debug_sigprint();
    }
  
  const Row<eT>& M;
  
  template<typename eT2>
  coot_inline bool is_alias(const Mat<eT2>& X) const { return (void_ptr(&M) == void_ptr(&X)); }
  };



template<typename eT>
struct unwrap< Col<eT> >
  {
  typedef Col<eT> stored_type;

  inline
  unwrap(const Col<eT>& A)
    : M(A)
    {
    coot_extra_debug_sigprint();
    }
  
  const Col<eT>& M;
  
  template<typename eT2>
  coot_inline bool is_alias(const Mat<eT2>& X) const { return (void_ptr(&M) == void_ptr(&X)); }
  };



template<typename out_eT, typename T1, typename T2, typename glue_type>
struct unwrap< mtGlue<out_eT, T1, T2, glue_type> >
  {
  typedef Mat<out_eT> stored_type;
  
  inline
  unwrap(const mtGlue<out_eT, T1, T2, glue_type>& A)
    : M(A)
    {
    coot_extra_debug_sigprint();
    }
  
  const Mat<out_eT> M;
  
  template<typename eT2>
  coot_inline bool is_alias(const Mat<eT2>&) const { return false; }
  };



template<typename out_eT, typename T1, typename op_type>
struct unwrap< mtOp<out_eT, T1, op_type> >
  {
  typedef Mat<out_eT> stored_type;
  
  inline
  unwrap(const mtOp<out_eT, T1, op_type>& A)
    : M(A)
    {
    coot_extra_debug_sigprint();
    }
  
  const Mat<out_eT> M;
  
  template<typename eT2>
  coot_inline bool is_alias(const Mat<eT2>&) const { return false; }
  };



//
//
//



template<typename T1>
struct partial_unwrap
  {
  typedef typename T1::elem_type eT;
  typedef Mat<eT>                stored_type;
  
  inline
  partial_unwrap(const T1& A)
    : M(A)
    {
    coot_extra_debug_sigprint();
    }
  
  coot_inline eT get_val() const { return eT(1); }
  
  coot_inline bool is_alias(const Mat<eT>&) const { return false; }
  
  static const bool do_trans = false;
  static const bool do_times = false;
  
  const Mat<eT> M;
  };



template<typename eT>
struct partial_unwrap< Mat<eT> >
  {
  typedef Mat<eT> stored_type;
  
  inline
  partial_unwrap(const Mat<eT>& A)
    : M(A)
    {
    coot_extra_debug_sigprint();
    }
  
  coot_inline eT get_val() const { return eT(1); }
  
  coot_inline bool is_alias(const Mat<eT>& X) const { return ((&X) == (&M)); }
  
  static const bool do_trans = false;
  static const bool do_times = false;
  
  const Mat<eT>& M;
  };



template<typename T1>
struct partial_unwrap< Op<T1, op_htrans> >
  {
  typedef typename T1::elem_type eT;
  typedef Mat<eT>                stored_type;
  
  inline
  partial_unwrap(const Op<T1, op_htrans>& A)
    : M(A.m)
    {
    coot_extra_debug_sigprint();
    }
  
  coot_inline eT get_val() const { return eT(1); }
  
  coot_inline bool is_alias(const Mat<eT>&) const { return false; }
  
  static const bool do_trans = true;
  static const bool do_times = false;
  
  const Mat<eT> M;
  };



template<typename eT>
struct partial_unwrap< Op< Mat<eT>, op_htrans> >
  {
  typedef Mat<eT> stored_type;
  
  inline
  partial_unwrap(const Op< Mat<eT>, op_htrans>& A)
    : M(A.m)
    {
    coot_extra_debug_sigprint();
    }
  
  coot_inline eT get_val() const { return eT(1); }
  
  coot_inline bool is_alias(const Mat<eT>& X) const { return (void_ptr(&X) == void_ptr(&M)); }
  
  static const bool do_trans = true;
  static const bool do_times = false;
  
  const Mat<eT>& M;
  };



template<typename T1>
struct partial_unwrap< Op<T1, op_htrans2> >
  {
  typedef typename T1::elem_type eT;
  typedef Mat<eT>                stored_type;
  
  inline
  partial_unwrap(const Op<T1, op_htrans2>& A)
    : val(A.aux)
    , M  (A.m)
    {
    coot_extra_debug_sigprint();
    }
  
  coot_inline eT get_val() const { return val; }
  
  coot_inline bool is_alias(const Mat<eT>&) const { return false; }
  
  static const bool do_trans = true;
  static const bool do_times = true;
  
  const eT      val;
  const Mat<eT> M;
  };



template<typename eT>
struct partial_unwrap< Op< Mat<eT>, op_htrans2> >
  {
  typedef Mat<eT> stored_type;
  
  inline
  partial_unwrap(const Op< Mat<eT>, op_htrans2>& A)
    : val(A.aux)
    , M  (A.m)
    {
    coot_extra_debug_sigprint();
    }
  
  inline eT get_val() const { return val; }
  
  coot_hot coot_inline bool is_alias(const Mat<eT>& X) const { return (void_ptr(&X) == void_ptr(&M)); }
  
  static const bool do_trans = true;
  static const bool do_times = true;
  
  const eT       val;
  const Mat<eT>& M;
  };



template<typename T1>
struct partial_unwrap< eOp<T1, eop_scalar_times> >
  {
  typedef typename T1::elem_type eT;
  typedef Mat<eT>                stored_type;
  
  inline
  partial_unwrap(const eOp<T1, eop_scalar_times>& A)
    : val(A.aux)
    , M  (A.P.Q)
    {
    coot_extra_debug_sigprint();
    }
  
  coot_inline eT get_val() const { return val; }
  
  coot_inline bool is_alias(const Mat<eT>&) const { return false; }
  
  static const bool do_trans = false;
  static const bool do_times = true;
  
  const eT      val;
  const Mat<eT> M;
  };



template<typename eT>
struct partial_unwrap< eOp<Mat<eT>, eop_scalar_times> >
  {
  typedef Mat<eT> stored_type;
  
  inline
  partial_unwrap(const eOp<Mat<eT>,eop_scalar_times>& A)
    : val(A.aux)
    , M  (A.P.Q)
    {
    coot_extra_debug_sigprint();
    }
  
  inline eT get_val() const { return val; }
  
  coot_hot coot_inline bool is_alias(const Mat<eT>& X) const { return (void_ptr(&X) == void_ptr(&M)); }
  
  static const bool do_trans = false;
  static const bool do_times = true;
  
  const eT       val;
  const Mat<eT>& M;
  };



template<typename T1>
struct partial_unwrap< eOp<T1, eop_neg> >
  {
  typedef typename T1::elem_type eT;
  typedef Mat<eT>                stored_type;
  
  inline
  partial_unwrap(const eOp<T1, eop_neg>& A)
    : M(A.P.Q)
    {
    coot_extra_debug_sigprint();
    }
  
  coot_inline eT get_val() const { return eT(-1); }
  
  coot_inline bool is_alias(const Mat<eT>&) const { return false; }
  
  static const bool do_trans = false;
  static const bool do_times = true;
  
  const Mat<eT> M;
  };



template<typename eT>
struct partial_unwrap< eOp<Mat<eT>, eop_neg> >
  {
  typedef Mat<eT> stored_type;
  
  inline
  partial_unwrap(const eOp<Mat<eT>,eop_neg>& A)
    : M(A.P.Q)
    {
    coot_extra_debug_sigprint();
    }
  
  coot_inline eT get_val() const { return eT(-1); }
  
  coot_inline bool is_alias(const Mat<eT>& X) const { return (void_ptr(&X) == void_ptr(&M)); }
  
  static const bool do_trans = false;
  static const bool do_times = true;
  
  const Mat<eT>& M;
  };



//! @}
