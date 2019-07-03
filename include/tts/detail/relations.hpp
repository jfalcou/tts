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

#include <string>

namespace tts
{
  struct env;

  namespace detail
  {
    void check_relation(::tts::env &       runtime,
                        bool               result,
                        std::string_view   a,
                        std::string_view   b,
                        std::string_view   op,
                        std::string const &va,
                        std::string const &vb);
  }
}

#endif
