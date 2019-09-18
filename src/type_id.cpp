//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/api.hpp>
#include <cstdlib>
#include <string>

#if !defined(_MSC_VER)
#  include <cxxabi.h>
#endif

namespace tts::detail
{
  TTS_API std::string demangle(char const *name)
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
