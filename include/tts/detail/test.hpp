//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_TEST_HPP_INCLUDED
#define TTS_DETAIL_TEST_HPP_INCLUDED

#include <functional>
#include <vector>

namespace tts
{
  struct env;
}

namespace tts::detail
{
  // Test case wrapper class
  struct test
  {
    using behavior_t = std::function<void(tts::env &)>;
    test(std::string const &n, behavior_t const &b);
    void operator()(tts::env &e);

    std::string name;
    behavior_t  behaviour;
  };

  using test_suite = std::vector<test>;
  inline test_suite suite;

  template<typename Test> bool registration(Test &&t)
  {
    suite.push_back(std::forward<Test>(t));
    return true;
  }
}

#endif
