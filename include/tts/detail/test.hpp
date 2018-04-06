//==================================================================================================
/*
  Copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/
//==================================================================================================
#ifndef TTS_DETAIL_TEST_HPP_INCLUDED
#define TTS_DETAIL_TEST_HPP_INCLUDED

#include <vector>
#include <functional>
#include <tts/engine/env.hpp>

namespace tts { namespace detail
{
  // Test case wrapper class
  struct test
  {
    using behavior_t = std::function<void( tts::env& )>;

    test( std::string const& n, behavior_t const& b ) : name( n ), behaviour( b ) {}
    void operator()(tts::env& e) { behaviour(e); }

    std::string name;
    behavior_t  behaviour;
  };

  using   test_suite = std::vector<test>;
  inline  test_suite suite = {};

  template<typename Test> bool registration(Test&& t)
  {
    suite.push_back(std::forward<Test>(t));
    return true;
  }
} }

#endif
