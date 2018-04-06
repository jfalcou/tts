//==================================================================================================
/*
  Copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/
//==================================================================================================
#ifndef TTS_DETAIL_TYPE_ID_HPP_INCLUDED
#define TTS_DETAIL_TYPE_ID_HPP_INCLUDED

#include <type_traits>
#include <typeinfo>
#include <cstddef>
#include <cstdlib>
#include <string>

#if !defined(_MSC_VER)
#include <cxxabi.h>
#endif

namespace tts
{
  namespace detail
  {
    inline std::string demangle( char const * name ) noexcept
    {
      #if defined(_MSC_VER)
      return std::string(name);
      #else
      int status = 0;
      std::size_t size = 0;
      auto ptr = abi::__cxa_demangle( name, nullptr, &size, &status );

      return std::string(ptr);
      #endif
    }
  }

  // Type name demangling function
  template<typename T> inline std::string type_id()
  {
    std::string s = detail::demangle(typeid(T).name());
    s += std::is_const_v<std::remove_reference_t<T>>  ? " const"  : "";
    s += std::is_lvalue_reference_v<T>                ? "&"       : "";
    s += std::is_rvalue_reference_v<T>                ? "&&"      : "";

    return s;
  }

  template<typename T> inline std::string type_id( const T& )
  {
    return type_id<T>();
  }
}

#endif
