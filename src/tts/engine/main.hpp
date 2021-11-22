//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/engine/help.hpp>
#include <tts/engine/test.hpp>
#include <tts/engine/suite.hpp>
#include <tts/tools/option.hpp>

//==================================================================================================
// Test application entry-point customization
//==================================================================================================
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
  ::tts::arguments = ::tts::options{argc,argv};

  if( ::tts::arguments[{"-h","--help"}] )
    return ::tts::usage(argv[0]);

  ::tts::verbose_status =  ::tts::arguments[{"-p","--pass"}];

  auto nb_tests = ::tts::detail::suite.size();
  std::size_t done_tests = 0;

  try
  {
    for(auto &t: ::tts::detail::suite)
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
