//======================================================================================================================
//! @file
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/engine/help.hpp>
#include <tts/engine/test.hpp>
#include <tts/engine/suite.hpp>
#include <tts/tools/option.hpp>

#if defined(TTS_DOXYGEN_INVOKED)
//======================================================================================================================
/*!
  @def TTS_CUSTOM_DRIVER_FUNCTION
  @brief Test application entry-point customization

  In case where the `main` function is to br provided externally, one can redefine
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
namespace tts::detail { constexpr bool use_main = true; }
#else
namespace tts::detail { constexpr bool use_main = false; }
#endif

namespace tts::detail { struct fatal_signal {}; }

#if defined(TTS_MAIN)
int TTS_CUSTOM_DRIVER_FUNCTION([[maybe_unused]] int argc,[[maybe_unused]] char const** argv)
{
  ::tts::initialize(argc,argv);
  if( ::tts::arguments()[{"-h","--help"}] )
    return ::tts::detail::usage(argv[0]);

  auto nb_tests = ::tts::detail::suite().size();
  std::size_t done_tests = 0;

  try
  {
    for(auto &t: ::tts::detail::suite())
    {
      auto test_count     = ::tts::global_runtime.test_count;
      auto failure_count  = ::tts::global_runtime.failure_count;
      ::tts::global_runtime.fail_status = false;

      t();
      done_tests++;

      if(test_count     == ::tts::global_runtime.test_count)
      {
        ::tts::global_runtime.invalid();
        std::cout << "[!] - " << ::tts::detail::current_test << " : EMPTY TEST CASE\n";
      }
      else if(failure_count  == ::tts::global_runtime.failure_count)
      {
        std::cout << "[V] - " << ::tts::detail::current_test << "\n";
      }
    }
  }
  catch( ::tts::detail::fatal_signal& )
  {
    std::cout << "@@ ABORTING DUE TO EARLY FAILURE @@ - "
              << (nb_tests - done_tests - 1) << " Tests not run\n";
  }

  if constexpr( ::tts::detail::use_main )   return ::tts::report(0,0);
  else                                      return 0;
}
#endif
