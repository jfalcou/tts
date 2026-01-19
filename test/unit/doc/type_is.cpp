//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>
#include <type_traits>

TTS_CASE("Check that types can be tested for equivalence")
{
  TTS_TYPE_IS(std::add_pointer<float const>::type, float const*);
};
//! [snippet]
