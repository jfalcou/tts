//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_RELATIONS_HPP_INCLUDED
#define TTS_DETAIL_RELATIONS_HPP_INCLUDED

#include <tts/api.hpp>
#include <string>

namespace tts
{
  struct env;

  namespace detail
  {
    struct location;
    TTS_API void check_relation(::tts::env &       runtime,
                                location const&    l,
                                bool               result,
                                std::string_view   a,
                                std::string_view   b,
                                std::string_view   op,
                                std::string_view   va,
                                std::string_view   vb);
  }
}

#endif
