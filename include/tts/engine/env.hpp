//==================================================================================================
/*!
  @file

  Defines the tts::env data structure and related functions

  @copyright 2018 Joel FALCOU


  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

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
    public:

    env(int argc, const char** argv, std::ostream& s = std::cout)
      : test_count{0}, success_count{0}, invalid_count{0}, os(s)
    {
      args.update(argc,argv);
    }

    /// Trigger environment compact mode
    void compact(bool m) { compact_mode = m; }

    /// Check the environment mode
    bool is_compact() const { return compact_mode; }

    /// Trigger environment compact mode
    void fail_only(bool m) { fail_only_mode = m; }

    /// Check the environment mode
    bool is_fail_only() const { return fail_only_mode; }

    /// Report a test to be successful
    void as_success() { test_count++; success_count++; }

    /*!
      Report a test to be invalid

      A test case is to be reported invalid if it performs no test
    **/
    void as_invalid() { test_count++; invalid_count++; }

    /// Report a test to fail
    void as_failure() { test_count++; }

    /// Notify the test driver of the success of all test cases
    bool passed() const { return tests() != successes(); }

    /// Return the count of tests
    std::ptrdiff_t tests() const { return test_count; }

    /// Return the count of successes
    std::ptrdiff_t successes() const { return success_count; }

    /// Return the count of invalid tests
    std::ptrdiff_t invalids() const { return invalid_count; }

    /// Return the count of failures
    std::ptrdiff_t failures() const { return tests() - successes() - invalids(); }

    /// Return the output stream of the current driver
    std::ostream& stream() const { return os; }

    /// Insert a "[PASS]" message into the stream
    std::ostream& pass()
    {
      as_success();
      if(fail_only_mode)
      {
        return tts::detail::null_stream;
      }
      else
      {
        if(!compact_mode)
        {
          return os << "[PASS]" << " - ";
        }
        else
        {
          os << "+";
          return tts::detail::null_stream;
        }
      }
    }

    /// Insert a "[FAIL]" message into the stream
    std::ostream& fail()
    {
      as_failure();
      if(!compact_mode)
      {
        return os << "[FAIL]" << " - ";
      }
      else
      {
        os << "-";
        return tts::detail::null_stream;
      }
    }

    /// Insert a "[IVLD]" message into the stream
    std::ostream& invalid()
    {
      as_invalid();
      if(compact_mode)
        return os << "!";
      else
        return os << "[IVLD]" << " - Empty test case" << std::endl;
    }

    // Can't be generated due ton reference to ostream member
    env(env const&)             = delete;
    env& operator=(env const&)  = delete;

    private:
    std::ptrdiff_t  test_count;
    std::ptrdiff_t  success_count;
    std::ptrdiff_t  invalid_count;
    bool            compact_mode;
    bool            fail_only_mode;
    std::ostream&   os;
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

    e.stream()  << std::string(80,'-') << "\n"
      << "Results: "
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

    Display a header for each scenarios in a test suite

    @param env  Environment to use as data source
    @param t    Test to process
  **/
  template<typename Test>
  inline void scenario_header( env& env, Test const& t)
  {
    if(!env.is_compact())
    {
      auto hbar = std::string(80,'-');
      env.stream()  << hbar << std::endl
                    << "Scenario: " << t.name << std::endl
                    << hbar << std::endl;
    }
    else
    {
      env.stream()  << "Scenario: " << t.name << " : ";
    }
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
