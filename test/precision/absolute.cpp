//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

//! [abs-equal]
#include <tts/tts.hpp>
#include <tts/tests/precision.hpp>

TTS_CASE("Absolute distance")
{
  TTS_ABSOLUTE_EQUAL(-2.  , 2.f  , 4.);
  TTS_ABSOLUTE_EQUAL('A'  , 80LL , 15);
}
//! [abs-equal]

TTS_CASE_TPL( "Absolute distance between floating point", TTS_IEEE_TYPES)
{
  T a{1};
  T qnan = std::numeric_limits<T>::quiet_NaN();
  T inf  = std::numeric_limits<T>::infinity();
  T minf = -inf;

  TTS_ABSOLUTE_EQUAL(a    , a     , 0.  );
  TTS_ABSOLUTE_EQUAL(a    , qnan  , inf );
  TTS_ABSOLUTE_EQUAL(qnan , a     , inf );
  TTS_ABSOLUTE_EQUAL(qnan , qnan  , 0.  );

  TTS_ABSOLUTE_EQUAL(a    , inf   , inf );
  TTS_ABSOLUTE_EQUAL(inf  , a     , inf );
  TTS_ABSOLUTE_EQUAL(inf  , inf   , inf );

  TTS_ABSOLUTE_EQUAL(a    , minf  , inf );
  TTS_ABSOLUTE_EQUAL(minf , a     , inf );
  TTS_ABSOLUTE_EQUAL(minf , minf  , inf );

  TTS_ABSOLUTE_EQUAL(a, a + 0.049f, .05 );
  TTS_ABSOLUTE_EQUAL(a, a - 0.049f, .05 );
  TTS_ABSOLUTE_EQUAL(a + 0.049f, a, .05 );
  TTS_ABSOLUTE_EQUAL(a - 0.049f, a, .05 );
  TTS_ABSOLUTE_EQUAL(a, a + 0.499f, .5 );
  TTS_ABSOLUTE_EQUAL(a, a - 0.499f, .5 );
  TTS_ABSOLUTE_EQUAL(a + 0.499f, a, .5 );
  TTS_ABSOLUTE_EQUAL(a - 0.499f, a, .5 );
}

TTS_CASE( "Absolute distance between boolean" )
{
  TTS_ABSOLUTE_EQUAL(true , true  , 0.);
  TTS_ABSOLUTE_EQUAL(true , false , 100.);
  TTS_ABSOLUTE_EQUAL(false, false , 0.);
  TTS_ABSOLUTE_EQUAL(false, true  , 100.);
}

TTS_CASE_TPL( "Absolute distance between integers", TTS_INTEGRAL_TYPES)
{
  T a = 65, b = a+5;

  TTS_ABSOLUTE_EQUAL(a, a, 0.);
  TTS_ABSOLUTE_EQUAL(a, b, 5.1);
  TTS_ABSOLUTE_EQUAL(b, a, 5.1);
}

#include "my_real.hpp"

TTS_CASE("Absolute distance of type with custom reldist")
{
  TTS_ABSOLUTE_EQUAL(n1::my_real{1.f}, n1::my_real{8.f}, 7.);
  TTS_ABSOLUTE_EQUAL(n1::my_real{8.f}, n1::my_real{1.f}, 7.);
}
