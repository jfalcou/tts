//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <iomanip>
#include <sstream>
#include <type_traits>
#include <tts/tools/typename.hpp>

namespace tts
{
  //================================================================================================
  // as_string concepts
  //================================================================================================
  template<typename T>
  concept support_std_to_string = requires(T e) { std::to_string(e); };

  template<typename T>
  concept support_to_string = requires(T e) { to_string(e); };

  template<typename T>
  concept has_to_string = requires(T e) { e.to_string(); };

  template<typename T>
  concept sequence = requires(T e) {std::begin(e); std::end(e); };

  template<typename T>
  concept streamable = requires(T e, std::ostream& o) { o << e; };

  //================================================================================================
  // Display a result
  //================================================================================================
  template<typename T> std::string as_string(T const& e)
  {
    if constexpr( std::is_pointer_v<T> )
    {
      std::ostringstream os;
      os << std::string(typename_<T>) << "(" << e << ")";
      return os.str();
    }
    else if constexpr( std::floating_point<T> )
    {
      auto precision = ::tts::arguments.value({"--precision"}, 2);
      bool hexmode   = ::tts::arguments[{"-x","--hex"}];
      bool scimode   = ::tts::arguments[{"-s","--scientific"}];

      std::ostringstream os;

      os << std::setprecision(precision);
            if(hexmode) os << std::hexfloat << e << std::defaultfloat;
      else  if(scimode) os << std::scientific << e << std::defaultfloat;
      else              os << e;

      return os.str();
    }
    else if constexpr( support_std_to_string<T> )
    {
      return std::to_string(e);
    }
    else if constexpr( streamable<T> )
    {
      std::ostringstream os;
      os << e;
      return os.str();
    }
    else if constexpr( support_to_string<T> )
    {
      return to_string(e);
    }
    else if constexpr( sequence<T> )
    {
      std::string that = "{ ";
      for(auto const& v : e) that += as_string(v) + " ";
      that += "}";
      return that;
    }
    else
    {
      std::ostringstream os;
      os << "[" << std::string(typename_<T>) << "]@(" << &e << ")";
      return os.str();
    }
  }

  inline std::string as_string(bool b) { return b ? std::string("true") : std::string("false"); }
  inline std::string as_string(std::string const& e)      { return  e;                          }
  inline std::string as_string(std::string_view const& e) { return  std::string(e);             }
  inline std::string as_string(std::nullptr_t)            { return  std::string("nullptr");     }

  inline std::string as_string(const char* e)
  {
    std::ostringstream os;
    os << "char*(" << (void*)e << ")";
    return os.str();
  }

  inline std::string as_string(char* e )
  {
    std::ostringstream os;
    os << "char*(" << (void*)e << ")";
    return os.str();
  }
}
