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


//! \addtogroup coot_config
//! @{



struct coot_config
  {
  #if defined(COOT_USE_LAPACK)
    static const bool lapack = true;
  #else
    static const bool lapack = false;
  #endif
  
  
  #if defined(COOT_USE_BLAS)
    static const bool blas = true;
  #else
    static const bool blas = false;
  #endif
  
  
  #if defined(COOT_NO_DEBUG)
    static const bool debug = false;
  #else
    static const bool debug = true;
  #endif
  
  
  #if defined(COOT_EXTRA_DEBUG)
    static const bool extra_debug = true;
  #else
    static const bool extra_debug = false;
  #endif
  
  
  #if defined(COOT_GOOD_COMPILER)
    static const bool good_comp = true;
  #else
    static const bool good_comp = false;
  #endif
  
  
  #if (  \
         defined(COOT_EXTRA_MAT_BONES)   || defined(COOT_EXTRA_MAT_MEAT)   \
      || defined(COOT_EXTRA_COL_BONES)   || defined(COOT_EXTRA_COL_MEAT)   \
      || defined(COOT_EXTRA_ROW_BONES)   || defined(COOT_EXTRA_ROW_MEAT)   \
      )
    static const bool extra_code = true;
  #else
    static const bool extra_code = false;
  #endif
  
  
  #if defined(COOT_USE_CXX11)
    static const bool use_cxx11 = true;
  #else
    static const bool use_cxx11 = false;
  #endif
  
  
  // TODO: may need to link with -lbandicoot anyway, to provide the runtime library
  #if defined(COOT_USE_WRAPPER)
    static const bool use_wrapper = true;
  #else
    static const bool use_wrapper = false;
  #endif
  };



//! @}
