//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

#include <iostream>
#include <cstddef>
#include <string>
#include <chrono>
#include <tts/detail/args.hpp>
#include <tts/detail/cxxopts.hpp>
#include <tts/api.hpp>

namespace tts::detail
{
  args_map::args_map()  : disable_colors_{false}
                        , report_pass_{false}
                        , repetition_{1}
                        , order_{""}
                        , seed_{1}
  {

  }

  TTS_API void args_map::update(int argc, char **argv) const
  {
    cxxopts::Options options(argv[0], "TTS Unit Tests Options");

    options
    .allow_unrecognised_options()
    .add_options()( "n,no-color", "Disable colored output"  )
                  ( "p,pass"    , "Report passing tests"    )
                  ( "r,repeat"  , "Repeat each tests arg times"
                  , cxxopts::value<std::size_t>()
                  )
                  ( "o,order"   , "Execute tests at 'random' or 'sorted'"
                  , cxxopts::value<std::string>()
                  )
                  ( "s,seed"    , "Set random seed to args (default is time)"
                  , cxxopts::value<unsigned int>()
                  )
                  ( "h,help"      , "Display this help message");

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
      std::cout << options.help({""}) << std::endl;
      exit(0);
    }

    disable_colors_ = result.count("no-color") != 0;
    report_pass_    = result.count("pass") != 0;

    if( result.count("repeat")  ) repetition_ = result["repeat"].as<std::size_t>();
    if( result.count("order")   ) order_      = result["order" ].as<std::string>();
    if( result.count("seed")    )
    {
      seed_ = result["seed"].as<unsigned int>();
    }
    else
    {
      auto now = std::chrono::high_resolution_clock::now();
      seed_ = static_cast<unsigned int>(now.time_since_epoch().count());
    }
  }
}

namespace tts
{
  const detail::args_map args;
}
