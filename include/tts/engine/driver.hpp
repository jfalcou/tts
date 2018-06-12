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

#include <tts/detail/args.hpp>
#include <algorithm>
#include <random>

namespace tts
{
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
  inline bool run(Environment& environment, Suite& tests, Setup const&... setup)
  {
    // retrieve status
    auto is_compact = args("compact",false);
    auto is_fail_only = args("fail-only",false);

    environment.compact(is_compact);
    environment.fail_only(is_fail_only);

    // randomize test on non-null random seed option
    if(auto seed = args("random",0u))
    {
      std::shuffle( tests.begin(), tests.end(), std::mt19937{seed} );
    }

    for(auto& t : tests )
    {
      scenario_header(environment,t);
      auto count = environment.tests();

      t(environment);

      process_invalid(environment, count);

      environment.stream() << std::endl;
    }

    return ::tts::report(environment,setup...);
  }
}

#endif
