//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

#include <vector>
#include <tts/tts.hpp>
#include <tts/tests/basic.hpp>

// Adapted from CATCH and LEST
TTS_CASE( "Default-constructed pointer behavior" )
{
  std::vector<int> v( 5 );

  TTS_EXPECT( v.size() == 5u );
  TTS_EXPECT( v.capacity() >= 5u );

  TTS_SUBCASE( "resizing bigger changes size and capacity" )
  {
    v.resize( 10 );

    TTS_EXPECT( v.size() == 10u );
    TTS_EXPECT( v.capacity() >= 10u );
  }

  TTS_SUBCASE( "resizing smaller changes size but not capacity" )
  {
    v.resize( 0 );
    TTS_EXPECT( v.size() == 0u );
    TTS_EXPECT( v.capacity() >= 5u );
  }

  TTS_SUBCASE( "reserving bigger changes capacity but not size" )
  {
    v.reserve( 10 );
    TTS_EXPECT( v.size() == 5u );
    TTS_EXPECT( v.capacity() >= 10u );

    TTS_SUBCASE( "reserving smaller again does not change capacity" )
    {
      v.reserve( 7 );
      TTS_EXPECT( v.capacity() >= 10u );
    }
  }

  TTS_SUBCASE( "reserving smaller does not change size or capacity" )
  {
    v.reserve( 0 );
    TTS_EXPECT( v.size() == 5u );
    TTS_EXPECT( v.capacity() >= 5u );
  }
}
