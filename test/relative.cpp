//==================================================================================================
/*
  Copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/precision.hpp>

TTS_CASE( "Relative distance between boolean" )
{
  TTS_RELATIVE_EQUAL(true , true  , 0.);
  TTS_RELATIVE_EQUAL(true , false , 100.);
  TTS_RELATIVE_EQUAL(false, false , 0.);
  TTS_RELATIVE_EQUAL(false, true  , 100.);
}

TTS_CASE_TPL( "Relative distance between integers", TTS_INTEGRAL_TYPES )
{
  T a = 65, b = a+5;

  TTS_RELATIVE_EQUAL(a, a, 0.);
  TTS_RELATIVE_EQUAL(a, b, 7.15);
  TTS_RELATIVE_EQUAL(b, a, 7.15);
}

TTS_CASE_TPL( "Relative distance between floating point", TTS_IEEE_TYPES )
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
}

//! [relative]
TTS_CASE("Relative distance between different types")
{
  TTS_RELATIVE_EQUAL('A'  , 80LL  , 18.75);
  TTS_RELATIVE_EQUAL(true , 1LL   , 0.);
  TTS_RELATIVE_EQUAL(1. , 2.f     , 100.);
}
//! [relative]

namespace n1
{
  struct my_real { float x; operator float() const { return x; } };
}

namespace tts { namespace ext
{
  template<typename EnableIf>
  struct equal<n1::my_real,n1::my_real,EnableIf>
  {
    inline bool operator()(n1::my_real const& l, n1::my_real const& r) const
    {
      return l.x == r.x;
    }
  };

  template<typename EnableIf>
  struct reldist<n1::my_real,n1::my_real,EnableIf>
  {
    inline double operator()(n1::my_real const& a, n1::my_real const& b) const
    {
      return a.x/b.x;
    }
  };
} }

TTS_CASE("Relative distance of type with custom reldist")
{
  TTS_RELATIVE_EQUAL(n1::my_real{1.f}, n1::my_real{8.f}, 12.5);
  TTS_RELATIVE_EQUAL(n1::my_real{8.f}, 1.f, 800.);
}
