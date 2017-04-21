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


//! \addtogroup coot_static_check
//! @{



template<bool ERROR___INCORRECT_OR_UNSUPPORTED_TYPE>
struct coot_type_check_cxx1998
  {
  coot_inline
  static
  void
  apply()
    {
    static const char
    junk[ ERROR___INCORRECT_OR_UNSUPPORTED_TYPE ? -1 : +1 ];
    }
  };



template<>
struct coot_type_check_cxx1998<false>
  {
  coot_inline
  static
  void
  apply()
    {
    }
  };



#if defined(COOT_USE_CXX11)
  
  #define coot_static_check(condition, message)  static_assert( !(condition), #message )
  
  #define coot_type_check(condition)  static_assert( !(condition), "error: incorrect or unsupported type" )
  
#else

  #define coot_static_check(condition, message)  static const char message[ (condition) ? -1 : +1 ]
  
  #define coot_type_check(condition)  coot_type_check_cxx1998<condition>::apply()

#endif



//! @}
