//==================================================================================================
/*
  Copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/precision.hpp>

// ![all_absolute]
TTS_CASE("Absolute distance between containers")
{
  std::vector<float> a{1.f,1.1f,0.9f,1.3f};
  std::vector<float> b{1.f,1.f,1.f,1.f};
  std::vector<float> s{1.1f};

  TTS_ALL_ABSOLUTE_EQUAL(a  , b   , .4);
  TTS_ALL_ABSOLUTE_EQUAL(s  , 1.f , .15);
  TTS_ALL_ABSOLUTE_EQUAL(1.f, s   , .15);
}
// ![all_absolute]
