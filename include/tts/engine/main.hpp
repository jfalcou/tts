//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_ENGINE_MAIN_HPP_INCLUDED
#define TTS_ENGINE_MAIN_HPP_INCLUDED

#include <tts/detail/test.hpp>
#include <tts/engine/driver.hpp>
#include <tts/engine/env.hpp>

#if !defined(TTS_CUSTOM_DRIVER_FUNCTION)
#  define TTS_CUSTOM_DRIVER_FUNCTION main
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
int TTS_CUSTOM_DRIVER_FUNCTION(int argc, const char **argv)
{
  ::tts::env runtime(argc, argv, std::cout);
  return ::tts::run(runtime, ::tts::detail::suite, 0, 0);
}
#endif

#endif
