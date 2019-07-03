//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_EXCEPTIONS_HPP_INCLUDED
#define TTS_DETAIL_EXCEPTIONS_HPP_INCLUDED

#include <cstddef>
#include <string>

namespace tts
{
  struct env;

  namespace detail
  {
    struct location;
    void throw_test(tts::env &       runtime,
                    location const & l,
                    std::string_view s,
                    std::string_view t,
                    void (*statement)(),
                    std::size_t);

    void nothrow_test(env &runtime, location const &l, std::string_view s, void (*statement)());
  }
}

#endif
