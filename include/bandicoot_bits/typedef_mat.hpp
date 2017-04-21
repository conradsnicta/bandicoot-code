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


//! \addtogroup typedef_mat
//! @{


#if defined(COOT_64BIT_INT)
  
  typedef Mat <u64> umat;
  typedef Col <u64> uvec;
  typedef Col <u64> ucolvec;
  typedef Row <u64> urowvec;
  typedef Cube<u64> ucube;
  
  typedef Mat <s64> imat;
  typedef Col <s64> ivec;
  typedef Col <s64> icolvec;
  typedef Row <s64> irowvec;
  typedef Cube<s64> icube;
  
#else
  
  typedef Mat <u32> umat;
  typedef Col <u32> uvec;
  typedef Col <u32> ucolvec;
  typedef Row <u32> urowvec;
  typedef Cube<u32> ucube;
  
  typedef Mat <s32> imat;
  typedef Col <s32> ivec;
  typedef Col <s32> icolvec;
  typedef Row <s32> irowvec;
  typedef Cube<s32> icube;
  
#endif


typedef Mat <float> fmat;
typedef Col <float> fvec;
typedef Col <float> fcolvec;
typedef Row <float> frowvec;
typedef Cube<float> fcube;

typedef Mat <double> dmat;
typedef Col <double> dvec;
typedef Col <double> dcolvec;
typedef Row <double> drowvec;
typedef Cube<double> dcube;

typedef Mat <double> mat;
typedef Col <double> vec;
typedef Col <double> colvec;
typedef Row <double> rowvec;
typedef Cube<double> cube;

typedef Mat <cx_float> cx_fmat;
typedef Col <cx_float> cx_fvec;
typedef Col <cx_float> cx_fcolvec;
typedef Row <cx_float> cx_frowvec;
typedef Cube<cx_float> cx_fcube;

typedef Mat <cx_double> cx_dmat;
typedef Col <cx_double> cx_dvec;
typedef Col <cx_double> cx_dcolvec;
typedef Row <cx_double> cx_drowvec;
typedef Cube<cx_double> cx_dcube;

typedef Mat <cx_double> cx_mat;
typedef Col <cx_double> cx_vec;
typedef Col <cx_double> cx_colvec;
typedef Row <cx_double> cx_rowvec;
typedef Cube<cx_double> cx_cube;


//! @}
