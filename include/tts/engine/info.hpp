//======================================================================================================================
//! @file
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/engine/environment.hpp>
#include <tts/tools/source_location.hpp>

//======================================================================================================================
/**
  @def TTS_PASS
  @brief Force a passing test and display a message.

  @param ... A format string and potential arguments to display as additional informations

  @groupheader{Example}

  @code
  #define TTS_MAIN  // No need for main()
  #include <tts/tts.hpp>

  TTS_CASE( "Check that forced pass passes" )
  {
    TTS_PASS("Forced success!!");
  };
  @endcode
**/
//======================================================================================================================
#define TTS_PASS(...)                                                                         \
  do                                                                                          \
  {                                                                                           \
    ::tts::global_runtime.pass();                                                             \
    if(::tts::_::is_verbose)                                                                  \
    {                                                                                         \
      auto contents = ::tts::text{__VA_ARGS__};                                               \
      printf( "  [+] %s : %.*s\n"                                                             \
            , ::tts::_::source_location::current().data(), contents.size(), contents.data()   \
            );                                                                                \
    }                                                                                         \
  } while(0)                                                                                  \
/**/

//======================================================================================================================
/**
  @def TTS_FAIL
  @brief Force a failing test and display a message.

  @param ... A format string and potential arguments to display as additional informations

  @groupheader{Example}

  @code
  #define TTS_MAIN  // No need for main()
  #include <tts/tts.hpp>

  TTS_CASE( "Check that forced failure fails" )
  {
    TTS_FAIL("Forced failure!!");
    TTS_PASS("This will pass though");
  };
  @endcode
**/
//======================================================================================================================
#define TTS_FAIL(...)                                                                                       \
  do                                                                                                        \
  {                                                                                                         \
    ::tts::global_runtime.fail();                                                                           \
    if(!::tts::global_runtime.fail_status)  ::tts::global_runtime.fail_status = true;                       \
    if(!::tts::_::is_verbose)                                                                               \
    {                                                                                                       \
      if( !::tts::_::current_type.is_empty() ) printf(">  With <T = %s>\n", ::tts::_::current_type.data()); \
    }                                                                                                       \
    auto contents = ::tts::text{__VA_ARGS__};                                                               \
    printf( "  [X] %s : ** FAILURE ** : %.*s\n"                                                             \
          , ::tts::_::source_location::current().data(), contents.size(), contents.data()                   \
          );                                                                                                \
  } while(0)                                                                                                \
/**/

//======================================================================================================================
/**
  @def TTS_FATAL
  @brief Force a failing test, display a message and halt the test suite.

  @param ... A format string and potential arguments to display as additional informations

  @groupheader{Example}

  @code
  #define TTS_MAIN  // No need for main()
  #include <tts/tts.hpp>

  TTS_CASE( "Check that forced major failure fails" )
  {
    TTS_FATAL("Forced major failure!!");
    TTS_PASS("This won't pass");
  };
  @endcode
**/
//======================================================================================================================
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
  } while(0)                                                                                                \
/**/
