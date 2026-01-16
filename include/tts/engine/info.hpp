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
#define TTS_PASS(...)                                                                         \
  do                                                                                          \
  {                                                                                           \
    ::tts::global_runtime.pass();                                                             \
    if(::tts::_::is_verbose && !::tts::_::is_quiet)                                           \
    {                                                                                         \
      auto contents = ::tts::text{__VA_ARGS__};                                               \
      printf( "  [+] %s : %.*s\n"                                                             \
            , ::tts::_::source_location::current().data(), contents.size(), contents.data()   \
            );                                                                                \
    }                                                                                         \
  } while(0)                                                                                  \
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
#define TTS_FAIL(...)                                                                                       \
  do                                                                                                        \
  {                                                                                                         \
    ::tts::global_runtime.fail();                                                                           \
    if(!::tts::global_runtime.fail_status)  ::tts::global_runtime.fail_status = true;                       \
    if(!::tts::_::is_verbose)                                                                               \
    {                                                                                                       \
      if( !::tts::_::current_type.is_empty() ) printf(">  With <T = %s>\n", ::tts::_::current_type.data()); \
    }                                                                                                       \
    if(!::tts::_::is_quiet)                                                                                 \
    {                                                                                                       \
      auto contents = ::tts::text{__VA_ARGS__};                                                             \
      printf( "  [X] %s : ** FAILURE ** : %.*s\n"                                                           \
            , ::tts::_::source_location::current().data(), contents.size(), contents.data()                 \
            );                                                                                              \
    }                                                                                                       \
  } while(0)                                                                                                \
/**/
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
#define TTS_FATAL(...)                                                                                      \
  do                                                                                                        \
  {                                                                                                         \
    ::tts::global_runtime.fail();                                                                           \
    if(!::tts::global_runtime.fail_status) ::tts::global_runtime.fail_status = true;                        \
    if(!::tts::_::is_verbose)                                                                               \
    {                                                                                                       \
      if( !::tts::_::current_type.is_empty() ) printf(">  With <T = %s>\n", ::tts::_::current_type.data()); \
    }                                                                                                       \
    auto contents = ::tts::text{__VA_ARGS__};                                                               \
    printf( "  [@] %s : @@ FATAL @@ : %.*s\n"                                                               \
          , ::tts::_::source_location::current().data(), contents.size(), contents.data()                   \
          );                                                                                                \
    ::tts::fatal_error_status = true;                                                                       \
    [[maybe_unused ]] ::tts::_::logger _local_tts_fail_hard{};                                              \
  } while(0)                                                                                                \
/**/
#endif

//======================================================================================================================
/**
  @}
**/
//======================================================================================================================
