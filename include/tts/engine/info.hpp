//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/environment.hpp>
#include <tts/tools/source_location.hpp>

namespace tts::_
{
  //====================================================================================================================
  // Reporting functions. Inline rather than outlined under TTS_MAIN: a translation unit that writes
  // its own main and uses the checks has to link without the driver.
  //====================================================================================================================
  // Shared by report_fail/report_fatal - the only place the failing type shows up outside verbose.
  inline void report_type_hint(::tts::text const& type)
  {
    if(!::tts::is_verbose() && !type.is_empty())
      ::tts::output().writeln(">  With <T = %s>", type.data());
  }

  inline void report_pass(char const* location, char const* message)
  {
    if(::tts::is_detailed())
    {
      ::tts::output().writeln("  [+] %s : %s", location, message);
    }
  }

  inline void report_fail(char const* location, char const* message, ::tts::text const& type)
  {
    report_type_hint(type);

    ::tts::output().assertion_failed(::tts::text {location}, ::tts::text {message}, false);

    if(!::tts::is_quiet())
    {
      ::tts::output().writeln("  [X] %s : ** FAILURE ** : %s", location, message);
    }
  }

  inline void report_fatal(char const* location, char const* message, ::tts::text const& type)
  {
    report_type_hint(type);

    ::tts::output().assertion_failed(::tts::text {location}, ::tts::text {message}, true);

    ::tts::output().writeln("  [@] %s : @@ FATAL @@ : %s", location, message);
  }
}

//======================================================================================================================
/**
  @name Informations Reporting
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @def TTS_PASS
  @brief Force a passing test and display a message.
  @ingroup test-basic

  @param ... A format string and potential arguments to display as additional informations

  @groupheader{Example}
  @snippet doc/pass_fail.cpp snippet1
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_PASS(...)
#else
#define TTS_PASS(...)                                                                              \
  do                                                                                               \
  {                                                                                                \
    ::tts::global_runtime.pass();                                                                  \
    ::tts::_::report_pass(::tts::_::source_location::current().data(),                             \
                          ::tts::text {__VA_ARGS__}.data());                                       \
  } while(0) /**/
/**/
#endif

//======================================================================================================================
/**
  @def TTS_FAIL
  @brief Force a failing test and display a message.
  @ingroup test-basic

  @param ... A format string and potential arguments to display as additional informations

  @groupheader{Example}
  @snippet doc/pass_fail.cpp snippet2
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_FAIL(...)
#else
#define TTS_FAIL(...)                                                                              \
  do                                                                                               \
  {                                                                                                \
    ::tts::global_runtime.fail();                                                                  \
    if(!::tts::global_runtime.fail_status) ::tts::global_runtime.fail_status = true;               \
    ::tts::_::report_fail(::tts::_::source_location::current().data(),                             \
                          ::tts::text {__VA_ARGS__}.data(),                                        \
                          ::tts::_::current_type);                                                 \
  } while(0) /**/
#endif

//======================================================================================================================
/**
  @def TTS_FATAL
  @brief Force a failing test, display a message and halt the test suite.
  @ingroup test-basic

  @param ... A format string and potential arguments to display as additional informations

  @groupheader{Example}
  @snippet doc/pass_fail.cpp snippet3
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_FATAL(...)
#else
#define TTS_FATAL(...)                                                                             \
  do                                                                                               \
  {                                                                                                \
    ::tts::global_runtime.fail();                                                                  \
    if(!::tts::global_runtime.fail_status) ::tts::global_runtime.fail_status = true;               \
    ::tts::_::report_fatal(::tts::_::source_location::current().data(),                            \
                           ::tts::text {__VA_ARGS__}.data(),                                       \
                           ::tts::_::current_type);                                                \
    ::tts::fatal_error_status = true;                                                              \
    [[maybe_unused]] ::tts::_::logger _local_tts_fail_hard {};                                     \
  } while(0) /**/
#endif

//======================================================================================================================
/**
  @}
**/
//======================================================================================================================
