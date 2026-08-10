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
  // Shared by report_fail/report_fatal: outside of verbose mode, the failing type isn't shown
  // anywhere else, so it gets reported here instead.
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

  // Splits the suite in `total` round-robin shards (test at registration index k belongs to
  // shard k % total), so a slow binary can be spread across parallel CI workers without
  // splitting source files. See --shard in doc/cli.hpp.
  struct shard_spec
  {
    bool         active = false;
    unsigned int index  = 0;
    unsigned int total  = 1;

    bool         selects(std::size_t position) const
    {
      return !active || (position % total == index);
    }

    std::size_t count(std::size_t suite_size) const
    {
      if(!active) return suite_size;
      if(suite_size <= index) return 0;
      return (suite_size - index - 1) / total + 1;
    }
  };

  // Parses --shard=i/n. `ok` is set to false if the flag is present but malformed or out of
  // range (n == 0 or i >= n); the returned shard_spec is then meaningless and must be ignored.
  TTS_DISABLE_WARNING_PUSH
  TTS_DISABLE_WARNING_CRT_SECURE
  inline shard_spec parse_shard(bool& ok)
  {
    ok              = true;
    ::tts::text raw = ::tts::arguments().value<::tts::text>("--shard");
    if(raw.is_empty()) return {};

    unsigned int i = 0;
    unsigned int n = 0;
    if(sscanf(raw.data(), "%u/%u", &i, &n) != 2 || n == 0 || i >= n)
    {
      ok = false;
      return {};
    }

    return {true, i, n};
  }
  TTS_DISABLE_WARNING_POP

  // Validates --sink=name (read from the current tts::arguments()). ok is set to false for an
  // unknown, non-empty name, in which case the returned text is ready to print as an error.
  inline ::tts::text validate_sink_name(::tts::text const& name, bool& ok)
  {
    ok = name.is_empty() || name == "colored" || name == "tap" || name == "diagnostics";
    if(ok) return {};

    return ::tts::text {"Unknown --sink value '%s', expected one of: colored, tap, diagnostics",
                        name.data()};
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

  // --shard only makes sense for the default "everything that ran must pass" driver: a custom
  // driver's caller typically expects a fixed, whole-suite fail/invalid count from
  // tts::report(fails, invalids), which round-robin partitioning has no way to redistribute
  // correctly, so --shard is a no-op there instead of risking a spurious pass or failure.
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

  ::tts::text capture_path = ::tts::arguments().value<::tts::text>("--capture");
  FILE*       capture_file = nullptr;

  if(!capture_path.is_empty())
  {
    capture_file = fopen(capture_path.data(), "w"); // NOSONAR
    if(!capture_file)
    {
      ::tts::output().writeln("Unable to open '%s' for writing (--capture)", capture_path.data());
      return 1;
    }
  }

  // Format (--sink) and destination (--capture) are orthogonal: every candidate sink below
  // targets capture_target, so --sink=X --capture=path writes X-formatted output to the file
  // exactly like --sink=X alone writes it to stdout.
  ::tts::gathering_sink capture_sink;
  ::tts::output_sink& capture_target = capture_file ? static_cast<::tts::output_sink&>(capture_sink)
                                                    : ::tts::output_handler::default_sink();

  // Like --shard, --sink only makes sense for the default TTS_MAIN driver: a custom driver
  // already manages its own sink lifecycle (installing it, dumping accumulate-style ones, ...),
  // so letting --sink install a different one out from under it, or finish() below fire on
  // whatever it installed, would fight that instead of helping.
  ::tts::text sink_name = ::tts::arguments().value<::tts::text>("--sink");
  if constexpr(!::tts::_::use_main) sink_name = ::tts::text {};

  bool        sink_ok    = true;
  ::tts::text sink_error = ::tts::_::validate_sink_name(sink_name, sink_ok);
  if(!sink_ok)
  {
    ::tts::output().writeln(sink_error);
    if(capture_file) fclose(capture_file); // NOSONAR
    return 1;
  }

  ::tts::colorized_sink   colorized_candidate {capture_target};
  ::tts::tap_sink         tap_candidate {capture_target};
  ::tts::diagnostics_sink diagnostics_candidate {capture_target};

  // Neither flag given: leave whatever sink the caller already installed alone, exactly as
  // before --sink existed - only touch output().sink() when there's an actual reason to.
  if(sink_name.is_empty() && capture_file) ::tts::output().sink(capture_sink);
  else if(sink_name == "colored") ::tts::output().sink(colorized_candidate);
  else if(sink_name == "tap") ::tts::output().sink(tap_candidate);
  else if(sink_name == "diagnostics") ::tts::output().sink(diagnostics_candidate);

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
      t();
      done_tests++;

      bool invalid = (test_count == ::tts::global_runtime.test_count);
      bool passed  = !invalid && (failure_count == ::tts::global_runtime.failure_count);

      if(invalid) ::tts::global_runtime.invalid();

      ::tts::output().test_finished(::tts::text {t.name}, passed, invalid);

      if(invalid)
      {
        if(!::tts::is_quiet()) ::tts::output().writeln("  [!!]: EMPTY TEST CASE");
        ::tts::output().flush();
      }
      else if(passed)
      {
        if(!::tts::is_quiet()) ::tts::output().writeln("TEST: '%s' - [PASSED]", t.name);
        ::tts::output().flush();
      }
    }
  }
  catch(::tts::_::fatal_signal&)
  {
    ::tts::output().suite_aborted();

    if(!::tts::is_quiet())
      ::tts::output().writeln("@@ ABORTING DUE TO EARLY FAILURE @@ - %d Tests not run",
                              static_cast<int>(nb_tests - done_tests - 1));
  }

  // finish() is part of the same TTS_MAIN-only convenience as --sink above: a custom driver's own
  // code decides when (or whether) to dump an accumulate-style sink it installed itself.
  int exit_code = 0;
  if constexpr(::tts::_::use_main)
  {
    exit_code = ::tts::report(0, 0);
    ::tts::output().finish();
  }

  if(capture_file)
  {
    ::tts::output().sink(::tts::output_handler::default_sink());
    fputs(capture_sink.content().data(), capture_file); // NOSONAR
    fclose(capture_file);                               // NOSONAR
  }

  return exit_code;
}
TTS_DISABLE_WARNING_POP

#endif
