//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/engine/env.hpp>
#include <tts/detail/test.hpp>
#include <tts/api.hpp>

namespace tts::detail
{
  test::test(std::string const &n, behavior_t const &b)
      : name(n)
      , behaviour(b)
  {
  }

  void test::operator()(tts::env &e)
  {
    e.scenario(name);
    behaviour(e);
  }
}
