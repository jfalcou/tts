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

    if(!::tts::is_quiet())
    {
      ::tts::output().writeln("  [X] %s : ** FAILURE ** : %s", location, message);
    }
  }

  void report_fatal(char const* location, char const* message, ::tts::text const& type)
  {
    report_type_hint(type);

    ::tts::output().writeln("  [@] %s : @@ FATAL @@ : %s", location, message);
  }
}

TTS_DISABLE_WARNING_PUSH
TTS_DISABLE_WARNING_CRT_SECURE
int TTS_CUSTOM_DRIVER_FUNCTION([[maybe_unused]] int argc, [[maybe_unused]] char const** argv)
{
  ::tts::initialize(argc, argv);
  if(::tts::arguments()("-h", "--help")) return ::tts::_::usage(argv[ 0 ]);

  if(::tts::arguments()("--dry"))
  {
    for(auto& t: ::tts::_::suite())
      ::tts::output().writeln(t.name);
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

  ::tts::gathering_sink capture_sink;
  if(capture_file) ::tts::output().sink(capture_sink);

  auto        nb_tests   = ::tts::_::suite().size();
  std::size_t done_tests = 0;
  auto        seed       = ::tts::random_seed();
  ::tts::set_random_seed(static_cast<std::uint64_t>(seed));
  ::tts::output().writeln(
  "Random seed: %d (rerun with --seed=%d to reproduce this run)", seed, seed);

  try
  {
    for(auto& t: ::tts::_::suite())
    {
      auto test_count                   = ::tts::global_runtime.test_count;
      auto failure_count                = ::tts::global_runtime.failure_count;
      ::tts::global_runtime.fail_status = false;

      if(!::tts::is_quiet()) ::tts::output().writeln("TEST: '%s'", t.name);
      ::tts::output().flush();
      t();
      done_tests++;

      if(test_count == ::tts::global_runtime.test_count)
      {
        ::tts::global_runtime.invalid();
        if(!::tts::is_quiet()) ::tts::output().writeln("  [!!]: EMPTY TEST CASE");
        ::tts::output().flush();
      }
      else if(failure_count == ::tts::global_runtime.failure_count)
      {
        if(!::tts::is_quiet()) ::tts::output().writeln("TEST: '%s' - [PASSED]", t.name);
        ::tts::output().flush();
      }
    }
  }
  catch(::tts::_::fatal_signal&)
  {
    if(!::tts::is_quiet())
      ::tts::output().writeln("@@ ABORTING DUE TO EARLY FAILURE @@ - %d Tests not run",
                              static_cast<int>(nb_tests - done_tests - 1));
  }

  if(capture_file)
  {
    ::tts::output().sink(::tts::output_handler::default_sink());
    fputs(capture_sink.content().data(), capture_file); // NOSONAR
    fclose(capture_file);                               // NOSONAR
  }

  if constexpr(::tts::_::use_main) return ::tts::report(0, 0);
  else return 0;
}
TTS_DISABLE_WARNING_POP

#endif
