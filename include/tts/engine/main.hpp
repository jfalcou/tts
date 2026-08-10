//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/usage.hpp>
#include <tts/engine/logger.hpp>
#include <tts/engine/test.hpp>
#include <tts/engine/environment.hpp>
#include <tts/engine/shard.hpp>
#include <tts/engine/sink_selection.hpp>
#include <tts/tools/clock.hpp>
#include <tts/tools/file.hpp>
#include <tts/tools/options.hpp>
#include <tts/tools/random.hpp>

#if defined(TTS_DOXYGEN_INVOKED)
//======================================================================================================================
/**
  @defgroup customization-points Customization Points
  @{
**/
//======================================================================================================================

//======================================================================================================================
/*!
  @def TTS_CUSTOM_DRIVER_FUNCTION
  @brief Test application entry-point customization

  In case where the `main` function is to be provided externally, one can redefine
  @ref TTS_CUSTOM_DRIVER_FUNCTION in order to specify the name of the test executable entry point.

  @see TTS_MAIN
  @see tts::initialize
  @see tts::report

  @groupheader{Example}
  @snippet doc/custom_driver.cpp snippet
**/
//======================================================================================================================
#define TTS_CUSTOM_DRIVER_FUNCTION

//======================================================================================================================
/*!
  @def TTS_MAIN
  @brief Identify the file containing tests entry point

  Users can dispatch tests in multiple Translation Units to be linked afterward. In this scenario,
one single Translation Unit must define @ref TTS_MAIN to generate the test entry point function.
**/
//======================================================================================================================
#define TTS_MAIN

//======================================================================================================================
/// @}
//======================================================================================================================
#endif

#if !defined(TTS_CUSTOM_DRIVER_FUNCTION)
#define TTS_CUSTOM_DRIVER_FUNCTION main
namespace tts::_
{
  inline constexpr bool use_main = true;
}
#else
namespace tts::_
{
  inline constexpr bool use_main = false;
}
#endif

#if defined(TTS_MAIN)
//======================================================================================================================
// Outlined reporting functions implementations
//======================================================================================================================
namespace tts::_
{
  // Shared by report_fail/report_fatal - the only place the failing type shows up outside verbose.
  void report_type_hint(::tts::text const& type)
  {
    if(!::tts::is_verbose() && !type.is_empty())
      ::tts::output().writeln(">  With <T = %s>", type.data());
  }

  void report_pass(char const* location, char const* message)
  {
    if(::tts::is_detailed())
    {
      ::tts::output().writeln("  [+] %s : %s", location, message);
    }
  }

  void report_fail(char const* location, char const* message, ::tts::text const& type)
  {
    report_type_hint(type);

    ::tts::output().assertion_failed(::tts::text {location}, ::tts::text {message}, false);

    if(!::tts::is_quiet())
    {
      ::tts::output().writeln("  [X] %s : ** FAILURE ** : %s", location, message);
    }
  }

  void report_fatal(char const* location, char const* message, ::tts::text const& type)
  {
    report_type_hint(type);

    ::tts::output().assertion_failed(::tts::text {location}, ::tts::text {message}, true);

    ::tts::output().writeln("  [@] %s : @@ FATAL @@ : %s", location, message);
  }
}

TTS_DISABLE_WARNING_PUSH
TTS_DISABLE_WARNING_CRT_SECURE
int TTS_CUSTOM_DRIVER_FUNCTION([[maybe_unused]] int argc, [[maybe_unused]] char const** argv)
{
  ::tts::initialize(argc, argv);
  if(::tts::arguments()("-h", "--help")) return ::tts::_::usage(argv[ 0 ]);

  bool shard_ok = true;
  auto shard    = ::tts::_::parse_shard(shard_ok);
  if(!shard_ok)
  {
    ::tts::output().writeln("Invalid --shard value, expected i/n with 0 <= i < n");
    return 1;
  }

  // No-op for a custom driver: tts::report(fails, invalids) expects the whole suite's count.
  if constexpr(!::tts::_::use_main) shard.active = false;

  if(::tts::arguments()("--dry"))
  {
    std::size_t position = 0;
    for(auto const& t: ::tts::_::suite())
    {
      if(!shard.selects(position++)) continue;

      if(t.types.is_empty()) ::tts::output().writeln(t.name);
      else ::tts::output().writeln("%s <%s>", t.name, t.types.data());
    }
    return 0;
  }

  ::tts::_::set_verbose(::tts::arguments()("-v", "--verbose"));
  ::tts::_::set_quiet(::tts::arguments()("-q", "--quiet"));

  ::tts::text          capture_path = ::tts::arguments().value<::tts::text>("--capture");
  ::tts::_::file_guard capture_file;

  if(!capture_path.is_empty())
  {
    capture_file = ::tts::_::file_guard {fopen(capture_path.data(), "w")}; // NOSONAR
    if(!capture_file)
    {
      ::tts::output().writeln("Unable to open '%s' for writing (--capture)", capture_path.data());
      return 1;
    }
  }

  // --sink and --capture are orthogonal - every candidate sink below targets capture_target.
  ::tts::gathering_sink capture_sink;
  ::tts::output_sink& capture_target = capture_file ? static_cast<::tts::output_sink&>(capture_sink)
                                                    : ::tts::output_handler::default_sink();

  // Like --shard, no-op for a custom driver: it already manages its own sink lifecycle.
  ::tts::text sink_name = ::tts::arguments().value<::tts::text>("--sink");
  if constexpr(!::tts::_::use_main) sink_name = ::tts::text {};

  bool        sink_ok    = true;
  ::tts::text sink_error = ::tts::_::validate_sink_name(sink_name, sink_ok);
  if(!sink_ok)
  {
    ::tts::output().writeln(sink_error);
    return 1;
  }

  ::tts::colorized_sink   colorized_candidate {capture_target};
  ::tts::tap_sink         tap_candidate {capture_target};
  ::tts::diagnostics_sink diagnostics_candidate {capture_target};
  ::tts::json_sink        json_candidate {capture_target};
  ::tts::junit_sink       junit_candidate {capture_target};

  // Neither flag given: leave whatever sink is already installed alone.
  if(sink_name.is_empty() && capture_file) ::tts::output().sink(capture_sink);
  else if(sink_name == "colored") ::tts::output().sink(colorized_candidate);
  else if(sink_name == "tap") ::tts::output().sink(tap_candidate);
  else if(sink_name == "diagnostics") ::tts::output().sink(diagnostics_candidate);
  else if(sink_name == "json") ::tts::output().sink(json_candidate);
  else if(sink_name == "junit") ::tts::output().sink(junit_candidate);

  auto        nb_tests   = shard.count(::tts::_::suite().size());
  std::size_t done_tests = 0;
  auto        seed       = ::tts::random_seed();
  ::tts::set_random_seed(static_cast<std::uint64_t>(seed));
  ::tts::output().writeln(
  "Random seed: %d (rerun with --seed=%d to reproduce this run)", seed, seed);
  if(shard.active)
    ::tts::output().writeln("Shard: %u/%u (%zu test%s selected)",
                            shard.index,
                            shard.total,
                            nb_tests,
                            nb_tests > 1 ? "s" : "");

  try
  {
    std::size_t position = 0;
    for(auto& t: ::tts::_::suite())
    {
      if(!shard.selects(position++)) continue;

      auto test_count                   = ::tts::global_runtime.test_count;
      auto failure_count                = ::tts::global_runtime.failure_count;
      ::tts::global_runtime.fail_status = false;

      ::tts::output().test_started(::tts::text {t.name});

      if(!::tts::is_quiet()) ::tts::output().writeln("TEST: '%s'", t.name);
      ::tts::output().flush();

      // Always measured regardless of --verbose - sinks and the Total Time: line need it either way.
      auto start_ns = ::tts::_::now_ns();
      t();
      auto duration_ns = ::tts::_::now_ns() - start_ns;
      done_tests++;

      ::tts::global_runtime.total_duration_ns += duration_ns;

      bool invalid                             = (test_count == ::tts::global_runtime.test_count);
      bool passed = !invalid && (failure_count == ::tts::global_runtime.failure_count);

      // invalid/passed reflect what happened, not the tag - only the comparison below does.
      bool matches_expectation = false;
      using enum ::tts::expected_outcome;
      switch(t.tag)
      {
      case pass: matches_expectation = passed; break;
      case xfail: matches_expectation = !invalid && !passed; break;
      case may_fail: matches_expectation = !invalid; break;
      case xinvalid: matches_expectation = invalid; break;
      }

      if(invalid) ::tts::global_runtime.invalid();
      if(!matches_expectation) ::tts::global_runtime.unexpected();

      ::tts::output().test_finished(::tts::text {t.name}, passed, invalid, duration_ns);

      ::tts::text duration_txt = ::tts::_::format_duration(static_cast<double>(duration_ns));

      if(t.tag != pass && !matches_expectation)
      {
        if(!::tts::is_quiet())
          ::tts::output().writeln("TEST: '%s' - ** UNEXPECTED ** (tagged %s) (%s)",
                                  t.name,
                                  ::tts::_::tag_name(t.tag),
                                  duration_txt.data());
        ::tts::output().flush();
      }
      else if(invalid)
      {
        if(!::tts::is_quiet())
        {
          ::tts::text line = ::tts::is_verbose()
                             ? ::tts::text {"  [!!]: EMPTY TEST CASE (%s)", duration_txt.data()}
                             : ::tts::text {"  [!!]: EMPTY TEST CASE"};
          ::tts::output().writeln(line);
        }
        ::tts::output().flush();
      }
      else if(passed)
      {
        if(!::tts::is_quiet())
        {
          ::tts::text line =
          ::tts::is_verbose()
          ? ::tts::text {"TEST: '%s' - [PASSED] (%s)", t.name, duration_txt.data()}
          : ::tts::text {"TEST: '%s' - [PASSED]", t.name};
          ::tts::output().writeln(line);
        }
        ::tts::output().flush();
      }
      else if(::tts::is_verbose() && !::tts::is_quiet())
      {
        ::tts::output().writeln("TEST: '%s' - (%s)", t.name, duration_txt.data());
        ::tts::output().flush();
      }
    }
  }
  catch(::tts::_::fatal_signal&)
  {
    // t() never returned, so per-case bookkeeping above never ran - count the abort here instead.
    ::tts::global_runtime.unexpected();

    ::tts::output().suite_aborted();

    if(!::tts::is_quiet())
      ::tts::output().writeln("@@ ABORTING DUE TO EARLY FAILURE @@ - %d Tests not run",
                              static_cast<int>(nb_tests - done_tests - 1));
  }

  // finish() is TTS_MAIN-only, like --sink - a custom driver decides for itself when to dump its own sink.
  int exit_code = 0;
  if constexpr(::tts::_::use_main)
  {
    exit_code = ::tts::report(0, 0);
    ::tts::output().finish();
  }

  if(capture_file)
  {
    ::tts::output().sink(::tts::output_handler::default_sink());
    fputs(capture_sink.content().data(), capture_file.get()); // NOSONAR
  }

  return exit_code;
}
TTS_DISABLE_WARNING_POP

#endif
