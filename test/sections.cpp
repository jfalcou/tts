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

TTS_CASE( "Sections and sub-sections behavior" )
{
  TTS_WHEN("A vector is initialized with 5 elements")
  {
    std::vector<int> v( 5 );

    TTS_EXPECT( v.size() == 5u );
    TTS_EXPECT( v.capacity() >= 5u );

    TTS_AND_THEN( "is resized to a larger size" )
    {
      v.resize( 10 );

      TTS_EXPECT( v.size() == 10u );
      TTS_EXPECT( v.capacity() >= 10u );
    }

    TTS_AND_THEN( "is resized to a smaller size" )
    {
      v.resize( 0 );
      TTS_EXPECT( v.size() == 0u );
      TTS_EXPECT( v.capacity() >= 5u );
    }
  }

  TTS_WHEN("A vector is uninitialized elements")
  {
    std::vector<int> v;

    TTS_EXPECT( v.size() == 0u );

    TTS_AND_THEN( "is now reserved to a larger size" )
    {
      v.reserve( 10 );
      TTS_EXPECT( v.size() == 0u );
      TTS_EXPECT( v.capacity() >= 10u );

      TTS_AND_THEN( "    is now reserved to an empty state" )
      {
        v.reserve( 0 );
        TTS_EXPECT( v.size() == 0u );
        TTS_EXPECT( v.capacity() >= 10u );
      }
    }
  }
}
