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


//! \addtogroup cpu_memory
//! @{


class cpu_memory
  {
  public:
  
  template<typename eT> inline coot_malloc static eT* acquire(const uword n_elem);
  
  template<typename eT> coot_inline static void release(eT* mem);
  };



template<typename eT>
inline
coot_malloc
eT*
cpu_memory::acquire(const uword n_elem)
  {
  coot_debug_check
    (
    ( size_t(n_elem) > (std::numeric_limits<size_t>::max() / sizeof(eT)) ),
    "cpu_memory::acquire(): requested size is too large"
    );
  
  eT* out_memptr;
  
  #if defined(COOT_HAVE_POSIX_MEMALIGN)
    {
    eT* memptr;
    
    const size_t alignment = 16;  // change the 16 to 64 if you wish to align to the cache line
    
    int status = posix_memalign((void **)&memptr, ( (alignment >= sizeof(void*)) ? alignment : sizeof(void*) ), sizeof(eT)*n_elem);
    
    out_memptr = (status == 0) ? memptr : NULL;
    }
  #elif defined(_MSC_VER)
    {
    //out_memptr = (eT *) malloc(sizeof(eT)*n_elem);
    out_memptr = (eT *) _aligned_malloc( sizeof(eT)*n_elem, 16 );  // lives in malloc.h
    }
  #else
    {
    //return ( new(std::nothrow) eT[n_elem] );
    out_memptr = (eT *) malloc(sizeof(eT)*n_elem);
    }
  #endif
  
  if(n_elem > 0)
    {
    coot_check_bad_alloc( (out_memptr == NULL), "cpu_memory::acquire(): out of memory" );
    }
  
  return out_memptr;
  }



template<typename eT>
coot_inline
void
cpu_memory::release(eT* mem)
  {
  #if defined(COOT_HAVE_POSIX_MEMALIGN)
    {
    free( (void *)(mem) );
    }
  #elif defined(_MSC_VER)
    {
    //free( (void *)(mem) );
    _aligned_free( (void *)(mem) );
    }
  #else
    {
    //delete [] mem;
    free( (void *)(mem) );
    }
  #endif
  }



//! @}
