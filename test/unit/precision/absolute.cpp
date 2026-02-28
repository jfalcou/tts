//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE("Absolute distance")
{
  TTS_ABSOLUTE_EQUAL(-2., 2.f, 4.);
  TTS_ABSOLUTE_EQUAL('A', 80LL, 15);
};

TTS_CASE_TPL("Absolute distance between floating point", double, float)
<typename T>(::tts::type<T>)
{
  T a {1};

  TTS_ABSOLUTE_EQUAL(a, a, 0.);
  TTS_ABSOLUTE_EQUAL(a, a + 0.049f, .05);
  TTS_ABSOLUTE_EQUAL(a, a - 0.049f, .05);
  TTS_ABSOLUTE_EQUAL(a + 0.049f, a, .05);
  TTS_ABSOLUTE_EQUAL(a - 0.049f, a, .05);
  TTS_ABSOLUTE_EQUAL(a, a + 0.499f, .5);
  TTS_ABSOLUTE_EQUAL(a, a - 0.499f, .5);
  TTS_ABSOLUTE_EQUAL(a + 0.499f, a, .5);
  TTS_ABSOLUTE_EQUAL(a - 0.499f, a, .5);

#if !defined(__FAST_MATH__)
  T qnan = std::numeric_limits<T>::quiet_NaN();
  T inf  = std::numeric_limits<T>::infinity();
  T minf = -inf;

  TTS_ABSOLUTE_EQUAL(a, qnan, inf);
  TTS_ABSOLUTE_EQUAL(qnan, a, inf);
  TTS_ABSOLUTE_EQUAL(qnan, qnan, 0.);

  TTS_ABSOLUTE_EQUAL(a, inf, inf);
  TTS_ABSOLUTE_EQUAL(inf, a, inf);
  TTS_ABSOLUTE_EQUAL(inf, inf, inf);

  TTS_ABSOLUTE_EQUAL(a, minf, inf);
  TTS_ABSOLUTE_EQUAL(minf, a, inf);
  TTS_ABSOLUTE_EQUAL(minf, minf, inf);
#endif
};

TTS_CASE("Absolute distance between boolean")
{
  TTS_ABSOLUTE_EQUAL(true, true, 0.);
  TTS_ABSOLUTE_EQUAL(true, false, 1.);
  TTS_ABSOLUTE_EQUAL(false, false, 0.);
  TTS_ABSOLUTE_EQUAL(false, true, 1.);
};

TTS_CASE_TPL("Absolute distance between integers",
             std::uint8_t,
             std::uint16_t,
             std::uint32_t,
             std::uint64_t,
             std::int8_t,
             std::int16_t,
             std::int32_t,
             std::int64_t)
<typename T>(::tts::type<T>)
{
  T a = 65, b = a + 5;

  TTS_ABSOLUTE_EQUAL(a, a, 0.);
  TTS_ABSOLUTE_EQUAL(a, b, 5.1);
  TTS_ABSOLUTE_EQUAL(b, a, 5.1);
};

#include "my_real.hpp"

TTS_CASE("Absolute distance of type with custom absolute distance")
{
  TTS_ABSOLUTE_EQUAL(n1::my_real {1.f}, n1::my_real {8.f}, 7.);
  TTS_ABSOLUTE_EQUAL(n1::my_real {8.f}, n1::my_real {1.f}, 7.);
};
