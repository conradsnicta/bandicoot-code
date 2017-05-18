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



template<typename glue_type, typename T1>
struct depth_lhs
  {
  static const uword num = 0;
  };

template<typename glue_type, typename T1, typename T2>
struct depth_lhs< glue_type, Glue<T1,T2,glue_type> >
  {
  static const uword num = 1 + depth_lhs<glue_type, T1>::num;
  };



template<uword N>
struct glue_times_redirect
  {
  template<typename T1, typename T2>
  inline static void apply(Mat<typename T1::elem_type>& out, const Glue<T1,T2,glue_times>& X);
  };


template<>
struct glue_times_redirect<2>
  {
  template<typename T1, typename T2>
  inline static void apply(Mat<typename T1::elem_type>& out, const Glue<T1,T2,glue_times>& X);
  };


template<>
struct glue_times_redirect<3>
  {
  template<typename T1, typename T2, typename T3>
  inline static void apply(Mat<typename T1::elem_type>& out, const Glue< Glue<T1,T2,glue_times>,T3,glue_times>& X);
  };


template<>
struct glue_times_redirect<4>
  {
  template<typename T1, typename T2, typename T3, typename T4>
  inline static void apply(Mat<typename T1::elem_type>& out, const Glue< Glue< Glue<T1,T2,glue_times>, T3, glue_times>, T4, glue_times>& X);
  };



class glue_times
  {
  public:
  
  
  template<typename T1, typename T2>
  inline static void apply(Mat<typename T1::elem_type>& out, const Glue<T1,T2,glue_times>& X);
  
  //
  
  template<typename eT, const bool do_trans_A, const bool do_trans_B>
  inline static uword mul_storage_cost(const Mat<eT>& A, const Mat<eT>& B);
  
  template<typename eT, const bool do_trans_A, const bool do_trans_B, const bool do_scalar_times>
  inline static void apply(Mat<eT>& out, const Mat<eT>& A, const Mat<eT>& B, const eT val);
  
  template<typename eT, const bool do_trans_A, const bool do_trans_B, const bool do_trans_C, const bool do_scalar_times>
  inline static void apply(Mat<eT>& out, const Mat<eT>& A, const Mat<eT>& B, const Mat<eT>& C, const eT val);
  
  template<typename eT, const bool do_trans_A, const bool do_trans_B, const bool do_trans_C, const bool do_trans_D, const bool do_scalar_times>
  inline static void apply(Mat<eT>& out, const Mat<eT>& A, const Mat<eT>& B, const Mat<eT>& C, const Mat<eT>& D, const eT val);
  };



//! @}

