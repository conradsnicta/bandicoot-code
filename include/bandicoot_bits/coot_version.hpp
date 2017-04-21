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


//! \addtogroup coot_version
//! @{



#define COOT_VERSION_MAJOR 0
#define COOT_VERSION_MINOR 100
#define COOT_VERSION_PATCH 0
#define COOT_VERSION_NOTE  "unstable development version"



struct coot_version
  {
  static const unsigned int major = COOT_VERSION_MAJOR;
  static const unsigned int minor = COOT_VERSION_MINOR;
  static const unsigned int patch = COOT_VERSION_PATCH;
  
  static
  inline
  std::string
  as_string()
    {
    std::stringstream ss;
    ss << coot_version::major << '.' << coot_version::minor << '.' << coot_version::patch;
    
    const std::string note = COOT_VERSION_NOTE;
    if(note.length() > 0)  { ss << " (" << note << ')'; }
    
    return ss.str();
    }
  };



//! @}
