//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/tools/callable.hpp>
#include <string>
#include <vector>

namespace tts::detail
{
  struct test
  {
    using behavior_t = tts::callable<void()>;

    void operator()()  { behaviour(); }

    static inline bool acknowledge(test&& f);

    std::string name;
    behavior_t  behaviour;
  };

  // Global tests suite
  inline std::vector<test> suite = {};

  bool inline test::acknowledge(test&& f)
  {
    suite.emplace_back( std::forward<test>(f));
    return true;
  }
}
