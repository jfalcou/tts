//======================================================================================================================
//! @file
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/engine/suite.hpp>
#include <tts/tools/source_location.hpp>

//======================================================================================================================
/**
  @def TTS_PASS
  @brief Force a passing test and display a message.

  @param Message A literal string to display as additional informations

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check that forced pass passes" )
  {
    TTS_PASS("Forced success!!");
  };
  @endcode
**/
//======================================================================================================================
#define TTS_PASS(Message)                                                                           \
  do                                                                                                \
  {                                                                                                 \
    ::tts::global_runtime.pass();                                                                   \
  } while(0)
/**/

//======================================================================================================================
/**
  @def TTS_FAIL
  @brief Force a failing test and display a message.

  @param Message A literal string to display as additional informations

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check that forced failure fails" )
  {
    TTS_FAIL("Forced failure!!");
    TTS_PASS("This will pass though");
  };
  @endcode
**/
//======================================================================================================================
#define TTS_FAIL(Message)                                                                           \
  do                                                                                                \
  {                                                                                                 \
    ::tts::global_runtime.fail();                                                                   \
    if(!::tts::global_runtime.fail_status)                                                          \
    {                                                                                               \
      ::tts::global_runtime.fail_status = true;                                                     \
      std::cout << "[X] - " << ::tts::detail::current_test << "\n";                                 \
    }                                                                                               \
    if( !::tts::detail::current_type.empty())                                                       \
    {                                                                                               \
      std::cout << "  > " << ::tts::detail::current_type << "\n";                                   \
    }                                                                                               \
    std::cout << "    " << ::tts::source_location::current() << " - ** FAILURE **"                  \
              << " : " << Message << std::endl;                                                     \
  } while(0)
/**/

//======================================================================================================================
/**
  @def TTS_FATAL
  @brief Force a failing test, display a message and halt the test suite.

  @param Message A literal string to display as additional informations

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check that forced major failure fails" )
  {
    TTS_FATAL("Forced major failure!!");
    TTS_PASS("This won't pass");
  };
  @endcode
**/
//======================================================================================================================
#define TTS_FATAL(Message)                                                                          \
  do                                                                                                \
  {                                                                                                 \
    ::tts::global_runtime.fatal();                                                                  \
    if(!::tts::global_runtime.fail_status)                                                          \
    {                                                                                               \
      ::tts::global_runtime.fail_status = true;                                                     \
      std::cout << "[@] - " << ::tts::detail::current_test<< "\n";                                  \
    }                                                                                               \
    if( !::tts::detail::current_type.empty())                                                       \
    {                                                                                               \
      std::cout << "  > " << ::tts::detail::current_type << "\n";                                   \
    }                                                                                               \
    std::cout << "    " << ::tts::source_location::current() << " - @@ FATAL @@"                    \
              << " : " << Message << std::endl;                                                     \
    ::tts::fatal_error_status = true;                                                               \
  } while(0)
/**/
