//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/detail/args.hpp>
#include <tts/engine/env.hpp>
#include <tts/detail/rt_helpers.hpp>
#include <tts/detail/color.hpp>
#include <tts/api.hpp>

namespace tts
{
  TTS_API env::env(int argc, char **argv, std::ostream &s)
                  : test_count {0}
                  , success_count {0}
                  , invalid_count {0}
                  , test_name_(argv[0])
                  , os(s)
  {
    args.update(argc, argv);
  }

  TTS_API void env::verbose(bool m) { verbose_ = m; }
  TTS_API bool env::verbose() const { return verbose_; }

  TTS_API void env::as_success()
  {
    test_count++;
    success_count++;
  }

  TTS_API void env::as_invalid()
  {
    test_count++;
    invalid_count++;
  }

  TTS_API void env::as_failure() { test_count++; }

  TTS_API bool env::passed() const { return tests() != successes(); }

  TTS_API std::ptrdiff_t env::tests() const { return test_count; }
  TTS_API std::ptrdiff_t env::successes() const { return success_count; }
  TTS_API std::ptrdiff_t env::invalids() const { return invalid_count; }
  TTS_API std::ptrdiff_t env::failures() const { return tests() - successes() - invalids(); }

  TTS_API std::ostream & env::stream() const { return os; }
  TTS_API std::ostream & env::output() const { return !verbose() ? tts::detail::null_stream : stream(); }

  TTS_API std::ostream & env::pass()
  {
    as_success();
    return output();
  }

  TTS_API std::ostream & env::fail()
  {
    as_failure();
    return stream();
  }

  TTS_API std::ostream & env::invalid()
  {
    as_invalid();
    return stream();
  }

  TTS_API void env::scenario_header(std::string const& name)
  {
    if(verbose())
    {
      auto hbar = std::string(80,'-');
      stream()  << hbar << std::endl
                << "[SCENARIO] - " << ::tts::detail::yellow << name << detail::reset << std::endl
                << hbar << std::endl;
    }
    else
    {
      output() << "[SCENARIO] - " << ::tts::detail::yellow << name << detail::reset << std::endl;
    }
  }

  TTS_API bool report(env const &e, std::ptrdiff_t fails, std::ptrdiff_t invalids)
  {
    auto test_txt = e.tests() > 1 ? "tests" : "test";
    auto pass_txt = e.successes() > 1 ? "successes" : "success";
    auto fail_txt = e.failures() > 1 ? "failures" : "failure";
    auto inv_txt  = e.invalids() > 1 ? "invalids" : "invalid";

    e.output()  << detail::reset << std::string(80, '-') << "\n";
    e.stream()  << detail::white << "Results: " << e.tests()  << " " << test_txt << " - "
                << e.successes() << detail::reset
                << " " << detail::green << pass_txt << detail::reset << " - "
                << detail::white << e.failures() << "/"  << fails     << " "<< detail::reset
                << detail::red << fail_txt << detail::reset << " - "
                << detail::white << e.invalids() << "/"  << invalids  << " " << detail::reset
                << detail::yellow << inv_txt
                << std::endl;

    if(!fails && !invalids)
      return e.passed();
    else
      return e.failures() != fails || e.invalids() != invalids;
  }

  TTS_API void process_invalid(env &env, std::ptrdiff_t count)
  {
    if(count == env.tests()) env.invalid();
  }
}
