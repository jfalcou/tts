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
                        , has_ulp_(false)
                        , has_valmax_(false)
                        , has_valmin_(false)
                        , repetition_{1}
                        , count_{10000}
                        , order_{""}
                        , seed_{1}
                        , ulpmax_{2.0}
                        , valmax_{0.}
                        , valmin_{1.}
                        , hex_{false}

  {}

  TTS_API void args_map::update(int argc, char **argv)
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
                  ( "c,count"    , "Set size of experiments (default is 10000)"
                  , cxxopts::value<unsigned int>()
                  )
                  ( "u,ulpmax"   , "Set global failure ulp threshold for range tests (default is 2.0)"
                  , cxxopts::value<double>()
                  )
                  ( "valmin"   , "Set minimal value for range tests (default is code)"
                  , cxxopts::value<double>()
                  )
                  ( "valmax"   , "Set maximal value for range tests (default is code)"
                  , cxxopts::value<double>()
                  )
                  ( "x,hex"      , "print the floating results in hexfloat mode"
                  , cxxopts::value<bool>()
                  )
                  ( "h,help"     , "Display this help message");

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
      std::cout << options.help({""}) << std::endl;
      exit(0);
    }

    disable_colors_ = result.count("no-color") != 0;
    report_pass_    = result.count("pass") != 0;

    has_valmin_ = result.count("valmin") != 0;
    has_valmax_ = result.count("valmax") != 0;
    has_ulp_    = result.count("ulpmax") != 0;

    if( result.count("count") ) count_      = result["count"].as<unsigned int>();
    if( result.count("repeat")) repetition_ = result["repeat"].as<unsigned int>();
    if( result.count("order") ) order_      = result["order" ].as<std::string>();
    if( result.count("hex")   ) hex_        = result["hex"].as<bool>();
    if( has_valmin_           ) valmin_     = result["valmin"].as<double>();
    if( has_valmax_           ) valmax_     = result["valmax"].as<double>();
    if( has_ulp_              ) ulpmax_     = result["ulpmax"].as<double>();

    if( result.count("seed")  )
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
  detail::args_map args;
}
