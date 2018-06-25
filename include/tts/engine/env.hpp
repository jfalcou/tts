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

#include <tts/detail/args.hpp>
#include <tts/detail/rt_helpers.hpp>
#include <iostream>
#include <cstddef>
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
    env ( int argc, const char** argv, std::ostream& s = std::cout )
        : test_count{0}, success_count{0}, invalid_count{0}, os(s)
    {
      args.update(argc,argv);
    }

    void verbose(bool m) { verbose_ = m; }
    bool verbose() const { return verbose_; }

    /// Report a test to be successful
    void as_success()   { test_count++; success_count++;  }
    void as_invalid()   { test_count++; invalid_count++;  }
    void as_failure()   { test_count++;                   }
    bool passed() const { return tests() != successes();  }

    std::ptrdiff_t tests()      const { return test_count;    }
    std::ptrdiff_t successes()  const { return success_count; }
    std::ptrdiff_t invalids()   const { return invalid_count; }
    std::ptrdiff_t failures()   const { return tests() - successes() - invalids(); }

    std::ostream& stream() const { return os; }
    std::ostream& output() const { return !verbose_ ? tts::detail::null_stream : stream(); }

    void                scenario(std::string const& title)  { current_scenario_ = title;  }
    std::string const&  scenario() const                    { return current_scenario_;   }

    std::ostream& pass()
    {
      as_success();
      return output();
    }

    std::ostream& fail()
    {
      as_failure();
      return stream();
    }

    std::ostream& invalid()
    {
      as_invalid();
      return stream();
    }

    private:
    std::ptrdiff_t  test_count;
    std::ptrdiff_t  success_count;
    std::ptrdiff_t  invalid_count;
    std::string     current_scenario_;
    std::ostream&   os;
    bool            verbose_;
  };

  /*!
    @ingroup group-unit

    Display the results aggregated across all tests cases and return a value indicating if the
    whole test is successful or not.
  **/
  inline bool report(env const& e, std::ptrdiff_t fails, std::ptrdiff_t invalids)
  {
    auto test_txt = e.tests()     > 1 ? "tests"     : "test";
    auto pass_txt = e.successes() > 1 ? "successes" : "success";
    auto fail_txt = e.failures()  > 1 ? "failures"  : "failure";
    auto inv_txt  = e.invalids()  > 1 ? "invalids"  : "invalid";

    e.output()  << std::string(80,'-') << "\n";
    e.stream()  << "Results: "
                << e.tests()  << " "    << test_txt << " - "
                << e.successes() << " " << pass_txt << " - "
                << e.failures() << "/"  << fails     << " " << fail_txt << " - "
                << e.invalids() << "/"  << invalids  << " " << inv_txt
                << std::endl;

    if(!fails && !invalids)
      return e.passed();
    else
      return e.failures() != fails || e.invalids() != invalids;
  }

  /*!
    @ingroup group-unit

    Handle the case of invalid scenario, i.e test case with no test inside

    @param env    Environment to use as data source
    @param count  Expected number of test to have run
  **/
  inline void process_invalid( env& env, std::ptrdiff_t count)
  {
    if(count == env.tests())
      env.invalid();
  }
}

#endif
