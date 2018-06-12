//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
//! [custom_driver_macro]
#define TTS_USE_CUSTOM_DRIVER
#include <tts/unit.hpp>
//! [custom_driver_macro]

#include <list>
#include <vector>
#include <iostream>

//! [case]
TTS_CASE( "A passing test" )
{
  TTS_PASS("Forced pass 1.");
}
//! [case]

//! [fail_case]
TTS_CASE( "An empty test will fail" )
{

}
//! [fail_case]

//! [case_tpl]
TTS_CASE_TPL( "A template test", (float)(int) )
{
  TTS_PASS("Forced pass of " << sizeof(T) << " bytes.");
}
//! [case_tpl]

//! [driver]
int main(int argc, char** argv)
{
  ::tts::env runtime(std::cout);

  // Test specific setup goes here

  auto result = ::tts::run( runtime, ::tts::suite(), {0,0} );

  // Test specific clean-up goes here

  return result;
}
//! [driver]
