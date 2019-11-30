//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//====================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/precision.hpp>

TTS_CASE("ULP distance")
{
  auto pi   = 3.14159265358979323846;
  auto eps  = std::numeric_limits<float>::epsilon();

  TTS_ULP_EQUAL('A'  , 80LL                   , 15.         );
  TTS_ULP_EQUAL( 1.f , 1.f+eps                , 0.5         );
  TTS_ULP_EQUAL( pi  , static_cast<float>(pi) , 9.84293e+07 );
  TTS_ULP_EQUAL( 1.  , short{1}               , 0.          );
}

TTS_CASE( "ULP distance between boolean" )
{
  TTS_ULP_EQUAL(true , true  , 0.);
  TTS_ULP_EQUAL(true , false , 1.);
  TTS_ULP_EQUAL(false, false , 0.);
  TTS_ULP_EQUAL(false, true  , 1.);
}

TTS_CASE_TPL( "ULP distance between integers", TTS_INTEGRAL_TYPES)
{
  T a = 65, b = a+5;

  TTS_ULP_EQUAL(a, a, 0.);
  TTS_ULP_EQUAL(a, b, 5.);
  TTS_ULP_EQUAL(b, a, 5.);
}

TTS_CASE_TPL( "ULP distance between floating point", TTS_IEEE_TYPES)
{
  T a{1};
  T eps  = std::numeric_limits<T>::epsilon();
  T qnan = std::numeric_limits<T>::quiet_NaN();
  T inf  = std::numeric_limits<T>::infinity();
  T minf = -inf;

  TTS_ULP_EQUAL(a    , a     , 0.  );
  TTS_ULP_EQUAL(a    , qnan  , inf );
  TTS_ULP_EQUAL(qnan , a     , inf );
  TTS_ULP_EQUAL(qnan , qnan  , 0.  );

  TTS_ULP_EQUAL(a    , inf   , inf );
  TTS_ULP_EQUAL(inf  , a     , inf );
  TTS_ULP_EQUAL(inf  , inf   , inf );

  TTS_ULP_EQUAL(a    , minf  , inf );
  TTS_ULP_EQUAL(minf , a     , inf );
  TTS_ULP_EQUAL(minf , minf  , inf );

  TTS_ULP_EQUAL(a, a-eps   , 0.5 );
  TTS_ULP_EQUAL(a, a+eps   , 0.5 );
  TTS_ULP_EQUAL(a, a+3*eps , 1.5 );

  TTS_ULP_EQUAL(a-eps  , a , 0.5 );
  TTS_ULP_EQUAL(a+eps  , a , 0.5 );
  TTS_ULP_EQUAL(a+3*eps, a , 1.5 );
}

#include "my_real.hpp"

TTS_CASE("ULP distance of type with custom ulpdist")
{
  TTS_ULP_EQUAL(n1::my_real{1.f}, n1::my_real{8.f}, 0.125);
  TTS_ULP_EQUAL(n1::my_real{8.f}, n1::my_real{1.f}, 8.);
}