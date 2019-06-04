//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_PRECISION_HPP_INCLUDED
#define TTS_DETAIL_PRECISION_HPP_INCLUDED

#include <string>

namespace tts
{
  struct env;

  namespace detail
  {
    void check_ulp(env&, double, double, std::string_view, std::string_view);
  }
}

#endif
