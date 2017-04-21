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



//! \addtogroup coot_cmath
//! @{



//
// wrappers for isfinite


template<typename eT>
coot_inline
bool
coot_isfinite(eT val)
  {
  coot_ignore(val);
  
  return true;
  }



template<>
coot_inline
bool
coot_isfinite(float x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::isfinite(x);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::isfinite(x);
    }
  #elif defined(COOT_HAVE_ISFINITE)
    {
    return (std::isfinite(x) != 0);
    }
  #else
    {
    const float y = (std::numeric_limits<float>::max)();
    
    const volatile float xx = x;
    
    return (xx == xx) && (x >= -y) && (x <= y);
    }
  #endif
  }



template<>
coot_inline
bool
coot_isfinite(double x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::isfinite(x);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::isfinite(x);
    }
  #elif defined(COOT_HAVE_ISFINITE)
    {
    return (std::isfinite(x) != 0);
    }
  #else
    {
    const double y = (std::numeric_limits<double>::max)();
    
    const volatile double xx = x;
    
    return (xx == xx) && (x >= -y) && (x <= y);
    }
  #endif
  }



template<typename T>
coot_inline
bool
coot_isfinite(const std::complex<T>& x)
  {
  if( (coot_isfinite(x.real()) == false) || (coot_isfinite(x.imag()) == false) )
    {
    return false;
    }
  else
    {
    return true;
    }
  }



//
// wrappers for isinf


template<typename eT>
coot_inline
bool
coot_isinf(eT val)
  {
  coot_ignore(val);
    
  return false;
  }



template<>
coot_inline
bool
coot_isinf(float x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::isinf(x);
    }
  #elif defined(COOT_HAVE_ISINF)
    {
    return (std::isinf(x) != 0);
    }
  #else
    {
    const float y = (std::numeric_limits<float>::max)();
    
    const volatile float xx = x;
    
    return (xx == xx) && ((x < -y) || (x > y));
    }
  #endif
  }



template<>
coot_inline
bool
coot_isinf(double x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::isinf(x);
    }
  #elif defined(COOT_HAVE_ISINF)
    {
    return (std::isinf(x) != 0);
    }
  #else
    {
    const double y = (std::numeric_limits<double>::max)();
    
    const volatile double xx = x;
    
    return (xx == xx) && ((x < -y) || (x > y));
    }
  #endif
  }



template<typename T>
coot_inline
bool
coot_isinf(const std::complex<T>& x)
  {
  return ( coot_isinf(x.real()) || coot_isinf(x.imag()) );
  }



//
// wrappers for isnan


template<typename eT>
coot_inline
bool
coot_isnan(eT val)
  {
  coot_ignore(val);
    
  return false;
  }



template<>
coot_inline
bool
coot_isnan(float x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::isnan(x);
    }
  #elif defined(COOT_HAVE_ISNAN)
    {
    return (std::isnan(x) != 0);
    }
  #else
    {
    const volatile float xx = x;
    
    return (xx != xx);
    }
  #endif
  }



template<>
coot_inline
bool
coot_isnan(double x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::isnan(x);
    }
  #elif defined(COOT_HAVE_ISNAN)
    {
    return (std::isnan(x) != 0);
    }
  #else
    {
    const volatile double xx = x;
    
    return (xx != xx);
    }
  #endif
  }



template<typename T>
coot_inline
bool
coot_isnan(const std::complex<T>& x)
  {
  return ( coot_isnan(x.real()) || coot_isnan(x.imag()) );
  }



// TODO: the functions below are probably not necessary


// rudimentary wrappers for log1p()

coot_inline
float
coot_log1p(const float x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::log1p(x);
    }
  #else
    {
    if((x >= float(0)) && (x < std::numeric_limits<float>::epsilon()))
      {
      return x;
      }
    else
    if((x < float(0)) && (-x < std::numeric_limits<float>::epsilon()))
      {
      return x;
      }
    else
      {
      return std::log(float(1) + x);
      }
    }
  #endif
  }



coot_inline
double
coot_log1p(const double x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::log1p(x);
    }
  #elif defined(COOT_HAVE_LOG1P)
    {
    return log1p(x);
    }
  #else
    {
    if((x >= double(0)) && (x < std::numeric_limits<double>::epsilon()))
      {
      return x;
      }
    else
    if((x < double(0)) && (-x < std::numeric_limits<double>::epsilon()))
      {
      return x;
      }
    else
      {
      return std::log(double(1) + x);
      }
    }
  #endif
  }





//
// wrappers for trigonometric functions
// 
// wherever possible, try to use C++11 or TR1 versions of the following functions:
// 
// complex acos
// complex asin
// complex atan
//
// real    acosh
// real    asinh
// real    atanh
//
// complex acosh
// complex asinh
// complex atanh
// 
// 
// if C++11 or TR1 are not available, we have rudimentary versions of:
// 
// real    acosh
// real    asinh
// real    atanh



template<typename T>
coot_inline
std::complex<T>
coot_acos(const std::complex<T>& x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::acos(x);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::acos(x);
    }
  #else
    {
    coot_ignore(x);
    coot_stop_logic_error("acos(): need C++11 compiler");
    
    return std::complex<T>(0);
    }
  #endif
  }



template<typename T>
coot_inline
std::complex<T>
coot_asin(const std::complex<T>& x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::asin(x);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::asin(x);
    }
  #else
    {
    coot_ignore(x);
    coot_stop_logic_error("asin(): need C++11 compiler");
    
    return std::complex<T>(0);
    }
  #endif
  }



template<typename T>
coot_inline
std::complex<T>
coot_atan(const std::complex<T>& x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::atan(x);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::atan(x);
    }
  #else
    {
    coot_ignore(x);
    coot_stop_logic_error("atan(): need C++11 compiler");
    
    return std::complex<T>(0);
    }
  #endif
  }



template<typename eT>
coot_inline
eT
coot_acosh(const eT x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::acosh(x);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::acosh(x);
    }
  #else
    {
    if(x >= eT(1))
      {
      // http://functions.wolfram.com/ElementaryFunctions/ArcCosh/02/
      return std::log( x + std::sqrt(x*x - eT(1)) );
      }
    else
      {
      if(std::numeric_limits<eT>::has_quiet_NaN)
        {
        return -(std::numeric_limits<eT>::quiet_NaN());
        }
      else
        {
        return eT(0);
        }
      }
    }
  #endif
  }



template<typename eT>
coot_inline
eT
coot_asinh(const eT x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::asinh(x);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::asinh(x);
    }
  #else
    {
    // http://functions.wolfram.com/ElementaryFunctions/ArcSinh/02/
    return std::log( x + std::sqrt(x*x + eT(1)) );
    }
  #endif
  }



template<typename eT>
coot_inline
eT
coot_atanh(const eT x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::atanh(x);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::atanh(x);
    }
  #else
    {
    if( (x >= eT(-1)) && (x <= eT(+1)) )
      {
      // http://functions.wolfram.com/ElementaryFunctions/ArcTanh/02/
      return std::log( ( eT(1)+x ) / ( eT(1)-x ) ) / eT(2);
      }
    else
      {
      if(std::numeric_limits<eT>::has_quiet_NaN)
        {
        return -(std::numeric_limits<eT>::quiet_NaN());
        }
      else
        {
        return eT(0);
        }
      }
    }
  #endif
  }



template<typename T>
coot_inline
std::complex<T>
coot_acosh(const std::complex<T>& x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::acosh(x);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::acosh(x);
    }
  #else
    {
    coot_ignore(x);
    coot_stop_logic_error("acosh(): need C++11 compiler");
    
    return std::complex<T>(0);
    }
  #endif
  }



template<typename T>
coot_inline
std::complex<T>
coot_asinh(const std::complex<T>& x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::asinh(x);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::asinh(x);
    }
  #else
    {
    coot_ignore(x);
    coot_stop_logic_error("asinh(): need C++11 compiler");
    
    return std::complex<T>(0);
    }
  #endif
  }



template<typename T>
coot_inline
std::complex<T>
coot_atanh(const std::complex<T>& x)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::atanh(x);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::atanh(x);
    }
  #else
    {
    coot_ignore(x);
    coot_stop_logic_error("atanh(): need C++11 compiler");
    
    return std::complex<T>(0);
    }
  #endif
  }



//
// wrappers for hypot(x, y) = sqrt(x^2 + y^2)


template<typename eT>
inline
eT
coot_hypot_generic(const eT x, const eT y)
  {
  #if defined(COOT_USE_CXX11)
    {
    return std::hypot(x, y);
    }
  #elif defined(COOT_HAVE_TR1)
    {
    return std::tr1::hypot(x, y);
    }
  #else
    {
    const eT xabs = std::abs(x);
    const eT yabs = std::abs(y);
    
    eT larger;
    eT ratio;
    
    if(xabs > yabs)
      {
      larger = xabs;
      ratio  = yabs / xabs;
      }
    else
      {
      larger = yabs;
      ratio  = xabs / yabs;
      }
    
    return (larger == eT(0)) ? eT(0) : (larger * std::sqrt(eT(1) + ratio * ratio));
    }
  #endif
  }



template<typename eT>
inline
eT
coot_hypot(const eT x, const eT y)
  {
  coot_ignore(x);
  coot_ignore(y);
  
  coot_stop_runtime_error("coot_hypot(): not implemented for integer or complex element types");
  
  return eT(0);
  }



template<>
coot_inline
float
coot_hypot(const float x, const float y)
  {
  return coot_hypot_generic(x,y);
  }



template<>
coot_inline
double
coot_hypot(const double x, const double y)
  {
  return coot_hypot_generic(x,y);
  }



//
// wrappers for arg()


template<typename eT>
struct coot_arg
  {
  static
  inline
  eT
  eval(const eT x)
    {
    #if defined(COOT_USE_CXX11)
      {
      return eT( std::arg(x) );
      }
    #else
      {
      coot_ignore(x);
      coot_stop_logic_error("arg(): need C++11 compiler");
      
      return eT(0);
      }
    #endif
    }
  };



template<>
struct coot_arg<float>
  {
  static
  coot_inline
  float
  eval(const float x)
    {
    #if defined(COOT_USE_CXX11)
      {
      return std::arg(x);
      }
    #else
      {
      return std::arg( std::complex<float>( x, float(0) ) );
      }
    #endif
    }
  };



template<>
struct coot_arg<double>
  {
  static
  coot_inline
  double
  eval(const double x)
    {
    #if defined(COOT_USE_CXX11)
      {
      return std::arg(x);
      }
    #else
      {
      return std::arg( std::complex<double>( x, double(0) ) );
      }
    #endif
    }
  };



template<>
struct coot_arg< std::complex<float> >
  {
  static
  coot_inline
  float
  eval(const std::complex<float>& x)
    {
    return std::arg(x);
    }
  };



template<>
struct coot_arg< std::complex<double> >
  {
  static
  coot_inline
  double
  eval(const std::complex<double>& x)
    {
    return std::arg(x);
    }
  };



//! @}
