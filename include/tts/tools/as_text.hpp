//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/text.hpp>
#include <tts/tools/options.hpp>
#include <tts/tools/types.hpp>

namespace tts
{
  //====================================================================================================================
  /**
    @defgroup tools-text Text Utilities
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /*!
    @ingroup tools-text
    @public
    @brief Value-to-string conversion

    When displaying the data required to understand a test's failures, TTS may need to print out
  value of various types. @ref as_text provides a centralized way to perform such a task by handling
  most common types including:
      + pointers
      + floating points
      + integers
      + sequences of such types

    @ref as_text takes care of applying any command-line options related to formatting to the value
  printed. For user defined types, an overload of a `to_text` function must be found via ADL.

    @groupheader{Examples}
    @snippet doc/as_text.cpp snippet

    @param e Value to convert to @ref tts::text
    @return An instance of @ref tts::text containing a representation of the value of e

    @see tts::text
  **/
  //====================================================================================================================
  template<typename T> text as_text(T const& e)
  {
    if constexpr(requires { to_text(e); }) { return to_text(e); }
    else if constexpr(std::floating_point<T>)
    {
      auto precision = ::tts::arguments().value(16, "--precision");
      bool hexmode   = ::tts::arguments()("-x", "--hex");
      bool scimode   = ::tts::arguments()("-s", "--scientific");

      if(scimode) return text("%.*E", e, precision);
      else if(hexmode) return text("%#.*A", e, precision);
      else return text("%.*g", e, precision);
    }
    else if constexpr(std::integral<T>)
    {
      if constexpr(sizeof(T) > 4)
      {
        auto fmt = ::tts::arguments()("-x", "--hex") ? "%lX" : "%ld";
        return text(fmt, e);
      }
      else
      {
        auto fmt = ::tts::arguments()("-x", "--hex") ? "%X" : "%d";
        return text(fmt, e);
      }
    }
    else if constexpr(_::string<T>) { return text("'%.*s'", e.size(), e.data()); }
    else if constexpr(_::optional<T>)
    {
      text base {"optional<%s>", as_text(typename_<typename T::value_type>).data()};
      if(e.has_value()) return base + text("{%s}", as_text(e.value()).data());
      else return base + "{}";
    }
    else if constexpr(std::is_pointer_v<T>)
    {
      return text("%p (%s)", (void*)(e), as_text(typename_<T>).data());
    }
    else if constexpr(_::sequence<T>)
    {
      text that("{ ");
      for(auto const& v: e) that += as_text(v) + " ";
      that += "}";
      return that;
    }
    else
    {
      // Display accessible bytes
      unsigned char bytes[ sizeof(e) ];
      std::memcpy(bytes, &e, sizeof(e));
      text txt_bytes("[ ");
      for(auto const& b: bytes) txt_bytes += text("%2.2X", b) + " ";
      txt_bytes += "]";

      return text("%s: %s", as_text(typename_<T>).data(), txt_bytes.data());
    }
  }

  //====================================================================================================================
  //! @}
  //====================================================================================================================

  template<std::size_t N> auto as_text(char const (&t)[ N ]) { return text(t); }
  inline auto                  as_text(std::nullptr_t) { return text("nullptr"); }
  inline auto                  as_text(bool b) { return text(b ? "true" : "false"); }
}
