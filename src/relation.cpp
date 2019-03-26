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
#include <tts/detail/relations.hpp>
#include <string>

namespace tts::detail
{
  void check_relation ( ::tts::env& runtime, bool result
                      , std::string_view a, std::string_view b, std::string_view op
                      , std::string const& va, std::string const& vb
                      )
  {
    if(result)
    {
      TTS_PASS("Expecting: "  << ::tts::detail::white_(a)   << " "
                              << ::tts::detail::white_(op)  << " "
                              << ::tts::detail::white_(b)
              );
    }
    else
    {
      TTS_FAIL("Expecting: "  << ::tts::detail::red_(va)  << " "
                              << ::tts::detail::red_(op) << " "
                              << ::tts::detail::red_(vb)
              );
    }
  }
}
