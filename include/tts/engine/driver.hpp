//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_ENGINE_DRIVER_HPP_INCLUDED
#define TTS_ENGINE_DRIVER_HPP_INCLUDED

#include <algorithm>
#include <random>
#include <chrono>
#include <tts/detail/args.hpp>

namespace tts
{
  namespace detail { int usage(std::string const&); }

  /*!
    @ingroup group-common

    @brief Starts the execution of a group of function object.

    Starts the execution of every functions registered as a test suite of any kind and report
    success or failure with respect to the expectation of said test semantic.

    Usually, the test suite being run is given by tts::test_suite() but it can
    differs in some use case, hence the need to pass it as a parameter.

    @par Example:
    @snippet example/unit/case.cpp driver

    @param environment  Test environment
    @param tests        Test suite to execute
    @param setup        Additional, test kind specific, parameters

    @return A boolean value indicating if the whole test is considered a success.
  **/
  template<typename Environment, typename Suite, typename... Setup>
  inline bool run(Environment &environment, Suite &tests, Setup const &... setup)
  {
    using namespace std::literals;

    if(args("help"sv, false))
      return ::tts::detail::usage(environment.name());

    // retrieve status
    auto fo = args("pass"sv, false);
    environment.verbose(fo);

    // check if tests needs to be repeated
    auto repetition = args("repeat"sv, 1);

    // randomize test on non-null random seed option
    auto seed = args("seed"sv, 1);

    // no seed ? See if e didn't asked for fully randomized
    if(args("seed"sv, ""s) == "time"sv)
      seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    auto order = args("order","declared"s);

    if(order == "random"s)
    {
      std::shuffle(tests.begin(), tests.end(), std::mt19937 {seed});
    }
    else if(order == "sorted"s)
    {
      std::sort ( tests.begin(), tests.end()
                , [](auto const& a, auto const& b) { return a.name < b.name; }
                );
    }

    for(auto &t: tests)
    {
      auto count = environment.tests();
      for(int i=0;i<repetition;++i) t(environment);
      process_invalid(environment, count);
    }

    return ::tts::report(environment, setup...);
  }
}

#endif
