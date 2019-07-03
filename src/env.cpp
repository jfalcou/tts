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

namespace tts
{
  env::env(int argc, char **argv, std::ostream &s)
      : test_count {0}
      , success_count {0}
      , invalid_count {0}
      , test_name_(argv[0])
      , os(s)
  {
    args.update(argc, argv);
  }

  std::string const& env::name() const { return test_name_; }

  void env::verbose(bool m) { verbose_ = m; }
  bool env::verbose() const { return verbose_; }

  void env::as_success()
  {
    test_count++;
    success_count++;
  }

  void env::as_invalid()
  {
    test_count++;
    invalid_count++;
  }

  void env::as_failure() { test_count++; }

  bool env::passed() const { return tests() != successes(); }

  std::ptrdiff_t env::tests() const { return test_count; }
  std::ptrdiff_t env::successes() const { return success_count; }
  std::ptrdiff_t env::invalids() const { return invalid_count; }
  std::ptrdiff_t env::failures() const { return tests() - successes() - invalids(); }

  std::ostream & env::stream() const { return os; }
  std::ostream & env::output() const { return !verbose() ? tts::detail::null_stream : stream(); }

  void                env::scenario(std::string const &title) { current_scenario_ = title; }
  std::string const & env::scenario() const { return current_scenario_; }

  std::ostream & env::pass()
  {
    as_success();
    return output();
  }

  std::ostream & env::fail()
  {
    as_failure();
    return stream();
  }

  std::ostream & env::invalid()
  {
    as_invalid();
    return stream();
  }

  bool report(env const &e, std::ptrdiff_t fails, std::ptrdiff_t invalids)
  {
    auto test_txt = e.tests() > 1 ? "tests" : "test";
    auto pass_txt = e.successes() > 1 ? "successes" : "success";
    auto fail_txt = e.failures() > 1 ? "failures" : "failure";
    auto inv_txt  = e.invalids() > 1 ? "invalids" : "invalid";

    e.output()  << detail::default_ << std::string(80, '-') << "\n";
    e.stream()  << detail::white_("Results: ")
                << detail::white_(e.tests())  << " "    << detail::white_(test_txt) << " - "
                << detail::white_(e.successes()) << " " << detail::green_(pass_txt) << " - "
                << detail::white_(e.failures()) << "/"  << fails     << " "
                << detail::red_(fail_txt) << " - "
                << detail::white_(e.invalids()) << "/"  << invalids  << " "
                << detail::yellow_(inv_txt)
                << std::endl;

    if(!fails && !invalids)
      return e.passed();
    else
      return e.failures() != fails || e.invalids() != invalids;
  }

  void process_invalid(env &env, std::ptrdiff_t count)
  {
    if(count == env.tests()) env.invalid();
  }
}
