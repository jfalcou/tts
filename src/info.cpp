//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/detail/rt_helpers.hpp>
#include <tts/detail/color.hpp>
#include <tts/engine/env.hpp>
#include <tts/tests/infos.hpp>
#include <typeinfo>
#include <cstdlib>
#include <string>

namespace tts::detail
{
  // Simple wrapper for __FILE__/__LINE__
  std::ostream &operator<<(std::ostream &os, location const &l)
  {
    return os << darkgray_(l.file) << ":" << darkgray_(l.line);
  }

  // Basic messaging
  std::ostream & pass(tts::env& runtime, location const& l)
  {
    return runtime.pass() << l
                   << ": " << ::tts::detail::green_("passed")
                   << ": " << ::tts::detail::white_(runtime.scenario()) << " - ";
  }

  std::ostream & fail(tts::env& runtime, location const& l)
  {
    return runtime.fail() << l
                   << ": " << ::tts::detail::red_("**FAILED**")
                   << ": " << ::tts::detail::white_(runtime.scenario()) << " - ";
  }

  std::ostream & invalid(tts::env& runtime, location const& l)
  {
    return runtime.invalid()  << l
                              << ": " << ::tts::detail::yellow_("**INVALID**")
                              << ": " << ::tts::detail::white_(runtime.scenario()) << " - ";
  }
}
