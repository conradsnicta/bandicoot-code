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


//! \addtogroup Mat
//! @{



template<typename eT>
class Mat : public Base< eT, Mat<eT> >
  {
  public:
  
  typedef eT                                elem_type;  //!< the type of elements stored in the matrix
  typedef typename get_pod_type<eT>::result  pod_type;  //!< if eT is std::complex<T>, pod_type is T; otherwise pod_type is eT
  
  coot_aligned const uword n_rows;    //!< number of rows     (read-only)
  coot_aligned const uword n_cols;    //!< number of columns  (read-only)
  coot_aligned const uword n_elem;    //!< number of elements (read-only)
  coot_aligned const uword vec_state; //!< 0: matrix layout; 1: column vector layout; 2: row vector layout
  coot_aligned const uword mem_state; //!< 0: normal; 1: external;  TODO: should this be expanded to allow re-allocation if size of aux mem is smaller than requested size?
  
  static const bool is_col = false;
  static const bool is_row = false;
  
  
  private:
  
  coot_aligned cl_mem device_mem;
  
  
  public:
  
  inline ~Mat();
  inline  Mat();
  
  inline explicit Mat(const uword in_rows, const uword in_cols);
  
  inline Mat(cl_mem aux_device_mem, const uword in_rows, const uword in_cols);
  
  inline cl_mem get_device_mem(const bool sync = true) const;
  
  inline void  read_device_mem(      eT* dest_memptr, const uword N) const;
  inline void write_device_mem(const eT*  src_memptr, const uword N);
  
  inline const Mat& operator= (const eT val);
  inline const Mat& operator+=(const eT val);
  inline const Mat& operator-=(const eT val);
  inline const Mat& operator*=(const eT val);
  inline const Mat& operator/=(const eT val);
  
  inline                   Mat(const Mat& X);
  inline const Mat& operator= (const Mat& X);
  inline const Mat& operator+=(const Mat& X);
  inline const Mat& operator-=(const Mat& X);
  inline const Mat& operator*=(const Mat& X);
  inline const Mat& operator%=(const Mat& X);
  inline const Mat& operator/=(const Mat& X);
  
  #if defined(COOT_USE_CXX11)
  inline                  Mat(Mat&& X);
  inline const Mat& operator=(Mat&& X);
  #endif
  
  inline void steal_mem(Mat& X);  //!< only for writing code internal to bandicoot
  
  inline                   Mat(const subview<eT>& X);
  inline const Mat& operator= (const subview<eT>& X);
  inline const Mat& operator+=(const subview<eT>& X);
  inline const Mat& operator-=(const subview<eT>& X);
  inline const Mat& operator*=(const subview<eT>& X);
  inline const Mat& operator%=(const subview<eT>& X);
  inline const Mat& operator/=(const subview<eT>& X);
  
  template<typename T1, typename eop_type> inline                   Mat(const eOp<T1, eop_type>& X);
  template<typename T1, typename eop_type> inline const Mat& operator= (const eOp<T1, eop_type>& X);
  template<typename T1, typename eop_type> inline const Mat& operator+=(const eOp<T1, eop_type>& X);
  template<typename T1, typename eop_type> inline const Mat& operator-=(const eOp<T1, eop_type>& X);
  template<typename T1, typename eop_type> inline const Mat& operator*=(const eOp<T1, eop_type>& X);
  template<typename T1, typename eop_type> inline const Mat& operator%=(const eOp<T1, eop_type>& X);
  template<typename T1, typename eop_type> inline const Mat& operator/=(const eOp<T1, eop_type>& X);
  
  template<typename T1, typename T2, typename eglue_type> inline                   Mat(const eGlue<T1, T2, eglue_type>& X);
  template<typename T1, typename T2, typename eglue_type> inline const Mat& operator= (const eGlue<T1, T2, eglue_type>& X);
  template<typename T1, typename T2, typename eglue_type> inline const Mat& operator+=(const eGlue<T1, T2, eglue_type>& X);
  template<typename T1, typename T2, typename eglue_type> inline const Mat& operator-=(const eGlue<T1, T2, eglue_type>& X);
  template<typename T1, typename T2, typename eglue_type> inline const Mat& operator*=(const eGlue<T1, T2, eglue_type>& X);
  template<typename T1, typename T2, typename eglue_type> inline const Mat& operator%=(const eGlue<T1, T2, eglue_type>& X);
  template<typename T1, typename T2, typename eglue_type> inline const Mat& operator/=(const eGlue<T1, T2, eglue_type>& X);
  
  inline const Mat& fill(const eT val);
  
  inline const Mat& zeros();
  inline const Mat& zeros(const uword new_n_elem);
  inline const Mat& zeros(const uword new_n_rows, const uword new_n_cols);
  
  inline const Mat& ones();
  inline const Mat& ones(const uword new_n_elem);
  inline const Mat& ones(const uword new_n_rows, const uword new_n_cols);
  
  inline const Mat& eye();
  inline const Mat& eye(const uword new_n_rows, const uword new_n_cols);
  
  inline void reset();
  inline void set_size(const uword new_n_elem);
  inline void set_size(const uword new_n_rows, const uword new_n_cols);
  
  inline void impl_print(const std::string extra_text) const;
  
  coot_warn_unused inline bool is_vec()    const;
  coot_warn_unused inline bool is_colvec() const;
  coot_warn_unused inline bool is_rowvec() const;
  coot_warn_unused inline bool is_square() const;
  coot_warn_unused inline bool is_empty()  const;
  
  coot_inline uword get_n_rows() const;
  coot_inline uword get_n_cols() const;
  coot_inline uword get_n_elem() const;
  
  inline coot_warn_unused MatValProxy<eT> operator[] (const uword ii);
  inline coot_warn_unused eT              operator[] (const uword ii) const;
  inline coot_warn_unused MatValProxy<eT> at         (const uword ii);
  inline coot_warn_unused eT              at         (const uword ii) const;
  inline coot_warn_unused MatValProxy<eT> operator() (const uword ii);
  inline coot_warn_unused eT              operator() (const uword ii) const;
  
  inline coot_warn_unused MatValProxy<eT> at         (const uword in_row, const uword in_col);
  inline coot_warn_unused eT              at         (const uword in_row, const uword in_col) const;
  inline coot_warn_unused MatValProxy<eT> operator() (const uword in_row, const uword in_col);
  inline coot_warn_unused eT              operator() (const uword in_row, const uword in_col) const;
  
  coot_inline       subview_row<eT> row(const uword row_num);
  coot_inline const subview_row<eT> row(const uword row_num) const;
  
  inline            subview_row<eT> operator()(const uword row_num, const span& col_span);
  inline      const subview_row<eT> operator()(const uword row_num, const span& col_span) const;
  
  
  coot_inline       subview_col<eT> col(const uword col_num);
  coot_inline const subview_col<eT> col(const uword col_num) const;
  
  inline            subview_col<eT> operator()(const span& row_span, const uword col_num);
  inline      const subview_col<eT> operator()(const span& row_span, const uword col_num) const;
  
  
  coot_inline       subview<eT> rows(const uword in_row1, const uword in_row2);
  coot_inline const subview<eT> rows(const uword in_row1, const uword in_row2) const;
  
  coot_inline       subview<eT> cols(const uword in_col1, const uword in_col2);
  coot_inline const subview<eT> cols(const uword in_col1, const uword in_col2) const;
  
  inline            subview<eT> rows(const span& row_span);
  inline      const subview<eT> rows(const span& row_span) const;
  
  coot_inline       subview<eT> cols(const span& col_span);
  coot_inline const subview<eT> cols(const span& col_span) const;
  
  
  coot_inline       subview<eT> submat(const uword in_row1, const uword in_col1, const uword in_row2, const uword in_col2);
  coot_inline const subview<eT> submat(const uword in_row1, const uword in_col1, const uword in_row2, const uword in_col2) const;
  
  coot_inline       subview<eT> submat(const uword in_row1, const uword in_col1, const SizeMat& s);
  coot_inline const subview<eT> submat(const uword in_row1, const uword in_col1, const SizeMat& s) const;
  
  inline            subview<eT> submat    (const span& row_span, const span& col_span);
  inline      const subview<eT> submat    (const span& row_span, const span& col_span) const;
  
  inline            subview<eT> operator()(const span& row_span, const span& col_span);
  inline      const subview<eT> operator()(const span& row_span, const span& col_span) const;
  
  inline            subview<eT> operator()(const uword in_row1, const uword in_col1, const SizeMat& s);
  inline      const subview<eT> operator()(const uword in_row1, const uword in_col1, const SizeMat& s) const;
  
  inline       subview<eT> head_rows(const uword N);
  inline const subview<eT> head_rows(const uword N) const;
  
  inline       subview<eT> tail_rows(const uword N);
  inline const subview<eT> tail_rows(const uword N) const;
  
  inline       subview<eT> head_cols(const uword N);
  inline const subview<eT> head_cols(const uword N) const;
  
  inline       subview<eT> tail_cols(const uword N);
  inline const subview<eT> tail_cols(const uword N) const;
  
  
  private:
  
  inline void cleanup();
  inline void init(const uword new_n_rows, const uword new_n_cols);
  
  friend class subview<eT>;
  friend class MatValProxy<eT>;

  
  public:
  
  #ifdef COOT_EXTRA_MAT_BONES
    #include COOT_INCFILE_WRAP(COOT_EXTRA_MAT_BONES)
  #endif
  };



class Mat_aux
  {
  public:

  template<typename eT> inline static void prefix_pp(Mat<eT>& x);
  template<typename T>  inline static void prefix_pp(Mat< std::complex<T> >& x);
  
  template<typename eT> inline static void postfix_pp(Mat<eT>& x);
  template<typename T>  inline static void postfix_pp(Mat< std::complex<T> >& x);
  
  template<typename eT> inline static void prefix_mm(Mat<eT>& x);
  template<typename T>  inline static void prefix_mm(Mat< std::complex<T> >& x);
  
  template<typename eT> inline static void postfix_mm(Mat<eT>& x);
  template<typename T>  inline static void postfix_mm(Mat< std::complex<T> >& x);
  
  template<typename eT, typename T1> inline static void set_real(Mat<eT>&                out, const Base<eT,T1>& X);
  template<typename T,  typename T1> inline static void set_real(Mat< std::complex<T> >& out, const Base< T,T1>& X);
  
  template<typename eT, typename T1> inline static void set_imag(Mat<eT>&                out, const Base<eT,T1>& X);
  template<typename T,  typename T1> inline static void set_imag(Mat< std::complex<T> >& out, const Base< T,T1>& X);
  };



//! @}
