//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/buffer.hpp>
#include <tts/tools/callable.hpp>

namespace tts::_
{
  inline const char *current_test = "";

  struct test
  {
    void operator()()
    {
      current_test = name;
      behaviour();
    }
    static inline bool acknowledge(test &&f);

    const char      *name;
    tts::_::callable behaviour;
  };

  // Global tests suite
  inline buffer<test> &suite()
  {
    static buffer<test> that = {};
    return that;
  }

  bool inline test::acknowledge(test &&f)
  {
    suite().emplace_back(TTS_MOVE(f));
    return true;
  }
}
