//======================================================================================================================
//! @file
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/engine/usage.hpp>
#include <tts/engine/logger.hpp>
#include <tts/engine/test.hpp>
#include <tts/engine/environment.hpp>
#include <tts/tools/options.hpp>

#if defined(TTS_DOXYGEN_INVOKED)
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

  @code
  #define TTS_MAIN
  #define TTS_CUSTOM_DRIVER_FUNCTION my_test_main
  #include <tts/tts.hpp>

  TTS_CASE( "Some test" ) {};

  int main(int argc, char const** argv)
  {
    ::tts::initialize(argc,argv);
    my_test_main(argc, argv);
    return ::tts::report(0,1);
  }
  @endcode
**/
//======================================================================================================================
#define TTS_CUSTOM_DRIVER_FUNCTION

//======================================================================================================================
/*!
  @def TTS_MAIN
  @brief Identify the file containing tests entry point

  Users can dispatch tests in multiple Translation Units to be linked afterward. In this scenario, one
  single Translation Unit must define @ref TTS_MAIN to generate the test entry point function.
**/
//======================================================================================================================
#define TTS_MAIN
#endif

#if !defined(TTS_CUSTOM_DRIVER_FUNCTION)
#  define TTS_CUSTOM_DRIVER_FUNCTION main
namespace tts::_ { inline constexpr bool use_main = true; }
#else
namespace tts::_ { inline constexpr bool use_main = false; }
#endif

#if defined(TTS_MAIN)
int TTS_CUSTOM_DRIVER_FUNCTION([[maybe_unused]] int argc,[[maybe_unused]] char const** argv)
{
  ::tts::initialize(argc,argv);
  if( ::tts::arguments()("-h","--help") )
    return ::tts::_::usage(argv[0]);

  ::tts::_::is_verbose = ::tts::arguments()("-v","--verbose");

  auto nb_tests = ::tts::_::suite().size();
  std::size_t done_tests = 0;

  try
  {
    for(auto &t: ::tts::_::suite())
    {
      auto test_count                   = ::tts::global_runtime.test_count;
      auto failure_count                = ::tts::global_runtime.failure_count;
      ::tts::global_runtime.fail_status = false;

      if(::tts::_::is_verbose)
      {
        printf("TEST: '%s'\n", t.name);
      }
      t();
      done_tests++;

      if(test_count == ::tts::global_runtime.test_count)
      {
        ::tts::global_runtime.invalid();
        printf("TEST: '%s' - [!!]: EMPTY TEST CASE\n", t.name);
      }
      else if(failure_count  == ::tts::global_runtime.failure_count && !::tts::_::is_verbose)
      {
        printf("TEST: '%s' - [V]\n", t.name);
      }
    }
  }
  catch( ::tts::_::fatal_signal& )
  {
    printf("@@ ABORTING DUE TO EARLY FAILURE @@ - %d Tests not run\n", static_cast<int>(nb_tests - done_tests - 1));
  }

  if constexpr( ::tts::_::use_main ) return ::tts::report(0,0);
  else                               return 0;
}

#endif
