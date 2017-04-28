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


//! \addtogroup wall_clock
//! @{


class wall_clock
  {
  public:
  
  inline  wall_clock();
  inline ~wall_clock();
  
  inline void   tic();  //!< start the timer
  inline double toc();  //!< return the number of seconds since the last call to tic()
  
  
  private:
  
  bool valid;
  
  #if defined(COOT_USE_CXX11)
    std::chrono::steady_clock::time_point chrono_time1;
  #elif defined(COOT_HAVE_GETTIMEOFDAY)
    struct timeval posix_time1;
    struct timeval posix_time2;
  #else
    std::clock_t time1;
  #endif
  };


//! @}
