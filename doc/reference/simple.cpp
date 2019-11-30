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
  // This test case is empty and thus will be reported as invalid
}

//! [tts_case]
#include <tts/tts.hpp>
#include <tts/tests/infos.hpp>
#include <tts/tests/basic.hpp>

TTS_CASE( "Make a simple test suite" )
{
  TTS_PASS("This one's a success");
  TTS_EXPECT( (2+2) == 4);
}
//! [tts_case]

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

//! [tts_case_tpl]
#include <tts/tts.hpp>
#include <tts/tests/basic.hpp>

TTS_CASE_TPL( "Make a simple test suite for some types", int, float, double )
{
  TTS_EXPECT(sizeof(T) >= 4);
}
//! [tts_case_tpl]
