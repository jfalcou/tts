//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/engine/env.hpp>
#include <tts/tests/infos.hpp>
#include <tts/detail/color.hpp>
#include <tts/tests/basic.hpp>
#include <string>

namespace tts::detail
{
  void check_ulp(tts::env& runtime, double r, double x, std::string_view a, std::string_view b)
  {
    if(r <= x)
    TTS_PASS("Expecting: " << ::tts::detail::white_(a) << " == "
                           << ::tts::detail::white_(b) << " within "
                           << ::tts::detail::green_(x) << " ULPs");
    else
    TTS_FAIL("Expecting: " << ::tts::detail::white_(a) << " == "
                           << ::tts::detail::white_(b) << " within "
                           << ::tts::detail::red_(x) << " ULPs but found: "
                           << ::tts::detail::red_(r)
                           << " ULPs instead.");
  }
}
