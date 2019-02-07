//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_TYPE_ID_HPP_INCLUDED
#define TTS_DETAIL_TYPE_ID_HPP_INCLUDED

#include <cstddef>
#include <cstdlib>
#include <string>
#include <type_traits>
#include <typeinfo>

#if !defined(_MSC_VER)
#  include <cxxabi.h>
#endif

namespace tts
{
  namespace detail
  {
    inline std::string demangle(char const *name) noexcept
    {
#if defined(_MSC_VER)
      return std::string(name);
#else
      int         status = 0;
      std::size_t size   = 0;
      auto        ptr    = abi::__cxa_demangle(name, nullptr, &size, &status);

      std::string that(ptr);

      free(ptr);

      return that;
#endif
    }
  }

  // Type name demangling function
  template<typename T> inline std::string type_id()
  {
    std::string s = detail::demangle(typeid(T).name());
    s += std::is_const_v<std::remove_reference_t<T>> ? " const" : "";
    s += std::is_lvalue_reference_v<T> ? "&" : "";
    s += std::is_rvalue_reference_v<T> ? "&&" : "";

    return s;
  }

  template<typename T> inline std::string type_id(const T &) { return type_id<T>(); }
}

#endif
