//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_ENGINE_ENV_HPP_INCLUDED
#define TTS_ENGINE_ENV_HPP_INCLUDED

#include <tts/api.hpp>
#include <cstddef>
#include <iostream>
#include <string>

namespace tts
{
  /*!
    @brief Test case environment
    @ingroup group-unit

    This data structure holds information required to run and report tests status across
    test cases.
  **/
  struct env
  {
    TTS_API env(int argc, char **argv, std::ostream &s = std::cout);

    std::string const &name() const { return test_name_; }

    TTS_API void verbose(bool m);
    TTS_API bool verbose() const;

    /// Report a test to be successful
    TTS_API void as_success();
    TTS_API void as_invalid();
    TTS_API void as_failure();
    TTS_API bool passed() const;

    TTS_API std::ptrdiff_t tests() const;
    TTS_API std::ptrdiff_t successes() const;
    TTS_API std::ptrdiff_t invalids() const;
    TTS_API std::ptrdiff_t failures() const;

    TTS_API std::ostream &stream() const;
    TTS_API std::ostream &output() const;

    TTS_API void scenario_header(std::string const& name);

    TTS_API std::ostream &pass();
    TTS_API std::ostream &fail();
    TTS_API std::ostream &invalid();

  private:
    std::ptrdiff_t test_count;
    std::ptrdiff_t success_count;
    std::ptrdiff_t invalid_count;
    std::string    current_scenario_;
    std::string    test_name_;
    std::ostream & os;
    bool           verbose_;
  };

  /*!
    @ingroup group-unit

    Display the results aggregated across all tests cases and return a value indicating if the
    whole test is successful or not.
  **/
  TTS_API bool report(env const &e, std::ptrdiff_t fails, std::ptrdiff_t invalids);

  /*!
    @ingroup group-unit

    Handle the case of invalid scenario, i.e test case with no test inside

    @param env    Environment to use as data source
    @param count  Expected number of test to have run
  **/
  TTS_API void process_invalid(env &env, std::ptrdiff_t count);
}

#endif
