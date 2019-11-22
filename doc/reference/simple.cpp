//==================================================================================================
/*
  Copyright 2019 Joel FALCOU
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE( "Make a simple test suite" )
{

}

#include <tts/tts.hpp>
#include <tts/tests/infos.hpp>
#include <tts/tests/basic.hpp>

TTS_CASE( "Make a simple test suite" )
{
  TTS_PASS("This one's a success");
  TTS_EXPECT( (2+2) == 4);
}

#include <tts/tts.hpp>
#include <tts/tests/relation.hpp>
#include <vector>

TTS_CASE( "Verify basic relations" )
{
  std::vector<float> a{1,2,3}, b{1,2,3};

  TTS_EQUAL(a, b);
  TTS_NOT_EQUAL( true, false );
  TTS_LESS( -1.f, 0.5 );
}
