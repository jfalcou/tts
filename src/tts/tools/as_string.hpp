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
#include <tts/tools/concepts.hpp>
#include <tts/tools/typename.hpp>

namespace tts
{
  //================================================================================================
  // Display a result
  //================================================================================================
  template<typename T> std::string as_string(T const& e)
  {
    if constexpr( std::is_pointer_v<T> )
    {
      std::ostringstream os;
      os << typename_<T> << "(" << (void*)(e) << ")";
      return os.str();
    }
    else if constexpr( std::floating_point<T> )
    {
      auto precision = ::tts::arguments().value({"--precision"}, -1);
      bool hexmode   = ::tts::arguments()[{"-x","--hex"}];
      bool scimode   = ::tts::arguments()[{"-s","--scientific"}];

      std::ostringstream os;

      if(precision != -1 ) os << std::setprecision(precision);

            if(hexmode) os << std::hexfloat   << e << std::defaultfloat;
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
      auto precision = ::tts::arguments().value({"--precision"}, -1);
      bool hexmode   = ::tts::arguments()[{"-x","--hex"}];
      bool scimode   = ::tts::arguments()[{"-s","--scientific"}];

      if(precision != -1 ) os << std::setprecision(precision);
            if(hexmode) os << std::hexfloat;
      else  if(scimode) os << std::scientific << e << std::defaultfloat;

      os << e;

      if(hexmode || scimode) os << std::defaultfloat;

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
      os << "[" << typename_<T> << "]@(" << &e << ")";
      return os.str();
    }
  }

  inline std::string as_string(bool b) { return b ? std::string("true") : std::string("false"); }
  inline std::string as_string(std::string const& e)      { return  e;                          }
  inline std::string as_string(std::string_view const& e) { return  std::string(e);             }
  inline std::string as_string(std::nullptr_t)            { return  std::string("nullptr");     }
}
