//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================

//! [entry-point]
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION custom_entry_point
#include <tts/tts.hpp>

TTS_CASE( "Tautological test" )
{
  TTS_EXPECT_NOT(false == true);
};

int main(int argc, char const** argv)
{
  custom_entry_point(argc, argv);
  return tts::report(0,0);
}
//! [entry-point]