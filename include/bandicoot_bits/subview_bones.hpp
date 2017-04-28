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


//! \addtogroup subview
//! @{


template<typename eT>
class subview : public Base<eT, subview<eT> >
  {
  public:
  
  typedef eT                                       elem_type;
  typedef typename get_pod_type<elem_type>::result pod_type;
  
  coot_aligned const Mat<eT>& m;
  
  static const bool is_row = false;
  static const bool is_col = false;
  
  const uword aux_row1;
  const uword aux_col1;
  
  const uword n_rows;
  const uword n_cols;
  const uword n_elem;
  
  coot_inline uword get_n_rows() const;
  coot_inline uword get_n_cols() const;
  coot_inline uword get_n_elem() const;
  
  
  protected:
  
  coot_inline subview(const Mat<eT>& in_m, const uword in_row1, const uword in_col1, const uword in_n_rows, const uword in_n_cols);
  
  
  public:
  
  inline ~subview();
  
  inline void operator= (const subview<eT>& x);
  
  inline void inplace_op(const eT val, cl_kernel kernel);
  
  inline void operator=  (const eT val);
  inline void operator+= (const eT val);
  inline void operator-= (const eT val);
  inline void operator*= (const eT val);
  inline void operator/= (const eT val);
  
  
  template<typename T1> inline void inplace_op(const Base<eT,T1>& x, cl_kernel kernel, const char* identifier);
  
  template<typename T1> inline void operator= (const Base<eT,T1>& x);
  template<typename T1> inline void operator+=(const Base<eT,T1>& x);
  template<typename T1> inline void operator-=(const Base<eT,T1>& x);
  template<typename T1> inline void operator%=(const Base<eT,T1>& x);
  template<typename T1> inline void operator/=(const Base<eT,T1>& x);
  
  
  inline static void       extract(Mat<eT>& out, const subview& in);
  inline static void  plus_inplace(Mat<eT>& out, const subview& in);
  inline static void minus_inplace(Mat<eT>& out, const subview& in);
  inline static void schur_inplace(Mat<eT>& out, const subview& in);
  inline static void   div_inplace(Mat<eT>& out, const subview& in);
  
  inline void fill(const eT val);
  inline void zeros();
  inline void ones();
  inline void eye(); // TODO
  
  inline bool check_overlap(const subview<eT>& x) const;
  
  coot_warn_unused inline bool is_vec()    const;
  coot_warn_unused inline bool is_colvec() const;
  coot_warn_unused inline bool is_rowvec() const;
  coot_warn_unused inline bool is_square() const;
  coot_warn_unused inline bool is_empty()  const;
  
  private:
  
  friend class Mat<eT>;
  subview();
  };



template<typename eT>
class subview_col : public subview<eT>
  {
  public:
  
  typedef eT                                       elem_type;
  typedef typename get_pod_type<elem_type>::result pod_type;
  
  static const bool is_row = false;
  static const bool is_col = true;
  
  coot_inline uword get_n_cols() const;

  inline void operator= (const subview<eT>& x);
  inline void operator= (const subview_col& x);
  inline void operator= (const eT val);
  
  template<typename T1>
  inline void operator= (const Base<eT,T1>& x);
  
  
  protected:
  
  inline subview_col(const Mat<eT>& in_m, const uword in_col);
  inline subview_col(const Mat<eT>& in_m, const uword in_col, const uword in_row1, const uword in_n_rows);
  
  
  private:
  
  friend class Mat<eT>;
  friend class Col<eT>;
  friend class subview<eT>;
  
  subview_col();
  };



template<typename eT>
class subview_row : public subview<eT>
  {
  public:
  
  typedef eT                                       elem_type;
  typedef typename get_pod_type<elem_type>::result pod_type;
  
  static const bool is_row = true;
  static const bool is_col = false;
  
  coot_inline uword get_n_rows() const;
  
  inline void operator= (const subview<eT>& x);
  inline void operator= (const subview_row& x);
  inline void operator= (const eT val);
  
  template<typename T1>
  inline void operator= (const Base<eT,T1>& x);
  
  coot_inline const Op<subview_row<eT>,op_htrans>  t() const;
  coot_inline const Op<subview_row<eT>,op_htrans> ht() const;
  coot_inline const Op<subview_row<eT>,op_strans> st() const;
  
  
  protected:
  
  inline subview_row(const Mat<eT>& in_m, const uword in_row);
  inline subview_row(const Mat<eT>& in_m, const uword in_row, const uword in_col1, const uword in_n_cols);
  
  
  private:
  
  friend class Mat<eT>;
  friend class Row<eT>;
  friend class subview<eT>;
  
  subview_row();
  };



//! @}
