//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE("Relative distance")
{
  TTS_RELATIVE_EQUAL(42.f , 42.f  , 0     );
  TTS_RELATIVE_EQUAL('A'  , 80LL  , 18.75 );
  TTS_RELATIVE_EQUAL(1.   , 2.f   , 100.  );
  TTS_RELATIVE_EQUAL(1    , 10    , 900.  );
};

TTS_CASE( "Relative distance between boolean" )
{
  TTS_RELATIVE_EQUAL(true , true  , 0.);
  TTS_RELATIVE_EQUAL(true , false , 100.);
  TTS_RELATIVE_EQUAL(false, false , 0.);
  TTS_RELATIVE_EQUAL(false, true  , 100.);
};

TTS_CASE_TPL(  "Relative distance between integers"
            , std::uint8_t,std::uint16_t,std::uint32_t,std::uint64_t
            , std::int8_t ,std::int16_t ,std::int32_t ,std::int64_t
            )
<typename T>(::tts::type<T>)
{
  T a = 65, b = a+5;

  TTS_RELATIVE_EQUAL(a, a, 0.);
  TTS_RELATIVE_EQUAL(a, b, 7.15);
  TTS_RELATIVE_EQUAL(b, a, 7.15);
};

TTS_CASE_TPL( "ULP distance between floating point", float, double )
<typename T>(::tts::type<T>)
{
  T a{1};
  T qnan = std::numeric_limits<T>::quiet_NaN();
  T inf  = std::numeric_limits<T>::infinity();
  T minf = -inf;

  TTS_RELATIVE_EQUAL(a    , a     , 0.  );
  TTS_RELATIVE_EQUAL(a    , qnan  , inf );
  TTS_RELATIVE_EQUAL(qnan , a     , inf );
  TTS_RELATIVE_EQUAL(qnan , qnan  , 0.  );

  TTS_RELATIVE_EQUAL(a    , inf   , inf );
  TTS_RELATIVE_EQUAL(inf  , a     , inf );
  TTS_RELATIVE_EQUAL(inf  , inf   , inf );

  TTS_RELATIVE_EQUAL(a    , minf  , inf );
  TTS_RELATIVE_EQUAL(minf , a     , inf );
  TTS_RELATIVE_EQUAL(minf , minf  , inf );

  TTS_RELATIVE_EQUAL(a, a + 0.049f,  5 );
  TTS_RELATIVE_EQUAL(a, a - 0.049f,  5 );
  TTS_RELATIVE_EQUAL(a + 0.049f, a,  5 );
  TTS_RELATIVE_EQUAL(a - 0.049f, a,  5 );
  TTS_RELATIVE_EQUAL(a, a + 0.499f, 50 );
  TTS_RELATIVE_EQUAL(a, a - 0.499f, 50 );
  TTS_RELATIVE_EQUAL(a + 0.499f, a, 50 );
  TTS_RELATIVE_EQUAL(a - 0.499f, a, 50 );
};

#include "my_real.hpp"

TTS_CASE("Relative distance of type with custom reldist")
{
  TTS_RELATIVE_EQUAL(n1::my_real{1.f}, n1::my_real{8.f}, 12.5);
  TTS_RELATIVE_EQUAL(n1::my_real{8.f}, 1.f, 800.);
};
