//==================================================================================================
/*
  Copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/
//==================================================================================================
#ifndef TTS_ENGINE_MAIN_HPP_INCLUDED
#define TTS_ENGINE_MAIN_HPP_INCLUDED

#include <tts/engine/driver.hpp>
#include <tts/engine/env.hpp>
#include <tts/detail/test.hpp>

#if !defined(TTS_CUSTOM_DRIVER_FUNCTION)
#define TTS_CUSTOM_DRIVER_FUNCTION main
#endif

//==================================================================================================
/*
  Test application entry-point customization

  In some scenario, one may need to run tests from a function not called main.
  Redefining TTS_CUSTOM_DRIVER_FUNCTION to any valid symbol name will generate a
  custom function callable from an already existing @c main function
*/
//==================================================================================================
#if !defined(TTS_USE_CUSTOM_DRIVER)
int TTS_CUSTOM_DRIVER_FUNCTION(int argc, const char** argv)
{
  ::tts::env runtime(argc,argv,std::cout);
  return ::tts::run( runtime, ::tts::detail::suite, 0, 0 );
}
#endif

#endif
