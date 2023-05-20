//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <tts/tools/callable.hpp>
#include <vector>
#include <string>

namespace tts::detail
{
  inline std::string current_test = "";

  struct test
  {
    void operator()()
    {
      current_test = name;
      behaviour();
    }
    static inline bool acknowledge(test&& f);

    std::string     name;
    tts::callable   behaviour;
  };

  // Global tests suite
  inline std::vector<test>& suite()
  {
    static std::vector<test> that = {};
    return that;
  }

  bool inline test::acknowledge(test&& f)
  {
    suite().emplace_back( std::forward<test>(f));
    return true;
  }
}
