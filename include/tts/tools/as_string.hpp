//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <iomanip>
#include <sstream>
#include <optional>
#include <type_traits>
#include <tts/tools/concepts.hpp>
#include <tts/tools/typename.hpp>

namespace tts
{
  //====================================================================================================================
  /*!
    @brief Value-to-string conversion

    When displaying the data required to understand a test's failures, TTS may need to print out value of various
    types. @ref as_string provides a centralized way to perform such a task by handling most common types including:
      + types supporting std::to_string
      + types supporting stream insertion
      + types supporting unqualified call to to_string
      + sequences of such types

    @ref as_string takes care of applying any command-line options related to formatting to the value printed.

    @groupheader{Examples}
    @code
    #define TTS_MAIN
    #include <tts/tts.hpp>

    namespace space
    {
      struct some_type { int i; };
      struct some_other_type { int i; };

      std::ostream& operator<<(std::ostream& os, some_other_type const& s)
      {
        return os << "[[" << s.i << "]]";
      }

      std::string to_string( some_type const& s ) { return "some_type[" + tts::as_string(s.i) + "]"; }
    }

    TTS_CASE( "Check display of type with specific to_string" )
    {
      TTS_EQUAL(tts::as_string( space::some_type{42} )      , "some_type[42]" );
      TTS_EQUAL(tts::as_string( space::some_other_type{63} ), "[[63]]"        );
    };
    @endcode

    @param e Value to convert to a string
    @return the formatted string containing a representation of the value of e
  **/
  //====================================================================================================================
  template<typename T> std::string as_string(T const& e)
  {
    if constexpr( std::is_pointer_v<T> )
    {
      std::ostringstream os;
      os << typename_<T> << "(" << (void*)(e) << ")";
      return os.str();
    }
    else if constexpr( tts::floating_point<T> )
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
    else if constexpr( detail::support_std_to_string<T> )
    {
      return std::to_string(e);
    }
    else if constexpr( detail::streamable<T> )
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
    else if constexpr( detail::support_to_string<T> )
    {
      return to_string(e);
    }
    else if constexpr( detail::sequence<T> )
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

  template<typename T>
  std::string as_string(std::optional<T> const& o)
  {
    if(o) return  std::string("optional<") + typename_<T> +">{" + as_string(*o) + "}";
    else  return  std::string("optional<") + typename_<T> + ">{}";
  }
}
