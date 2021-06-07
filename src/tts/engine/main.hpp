//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/engine/help.hpp>
#include <tts/engine/suite.hpp>
#include <tts/tools/color.hpp>
#include <tts/tools/option.hpp>

//==================================================================================================
// Test application entry-point customization
//==================================================================================================
#if !defined(TTS_CUSTOM_DRIVER_FUNCTION)
#  define TTS_CUSTOM_DRIVER_FUNCTION main
namespace tts::detail
{
  constexpr bool use_main = true;
}
#else
namespace tts::detail
{
  constexpr bool use_main = false;
}
#endif


#if defined(TTS_MAIN)
int TTS_CUSTOM_DRIVER_FUNCTION([[maybe_unused]] int argc,[[maybe_unused]] char const** argv)
{
  ::tts::arguments = ::tts::options{argc,argv};
  ::tts::detail::color_status = !::tts::arguments[{"-n","--no-color"}];

  if( ::tts::arguments[{"-h","--help"}] )
    return ::tts::usage(argv[0]);

  ::tts::verbose_status       =  ::tts::arguments[{"-p","--pass"}];
  std::size_t repetitions     =  ::tts::arguments.value( "--repeat", 1            );
  std::string filter          =  ::tts::arguments.value( "--filter", std::string{});

  for(auto &t: ::tts::detail::suite)
  {
    if(filter.empty() || (t.name.find(filter) != std::string::npos) )
    {
      auto count = ::tts::global_runtime.test_count;

      std::cout << ::tts::yellow << ::tts::bold
                << "[SCENARIO]" << " - " << t.name
                << ::tts::reset << std::endl;

      for(std::size_t i = 0; i < repetitions; ++i) t();

      if(count == ::tts::global_runtime.test_count)
        ::tts::global_runtime.invalid();

      if(::tts::global_runtime.fatal_count > 0)
      {
        std::cout << "\n" << ::tts::red
                  << ::tts::bold << "** ABORTING AFTER FIRST FAILURE **"
                  << "\n";
        break;
      }
    }
  }

  if constexpr( ::tts::detail::use_main ) return ::tts::report(0,0);
  else                                    return 0;
}
#endif
