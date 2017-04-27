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



//! \addtogroup span
//! @{


struct span_extra {};


template<typename junk = int>
struct span_base
  {
  static const span_extra all;
  };


template<typename junk>
const span_extra span_base<junk>::all = span_extra();


struct span : public span_base<>
  {
  coot_aligned uword a;
  coot_aligned uword b;
  coot_aligned bool  whole;
  
  inline span()                  : whole(true) {}
  inline span(const span_extra&) : whole(true) {}
  
  inline explicit span(const uword in_a)                   : a(in_a), b(in_a), whole(false) {}
  inline explicit span(const uword in_a, const uword in_b) : a(in_a), b(in_b), whole(false) {}
  };



//! @}
