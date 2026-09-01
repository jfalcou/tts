//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <cmath>

TTS_CASE("Relative distance")
{
  TTS_RELATIVE_EQUAL(42.f, 42.f, 0);
  TTS_RELATIVE_EQUAL(static_cast<long long>('A'), 80LL, 0.1875);
  TTS_RELATIVE_EQUAL(1., 2., 1.);
  TTS_RELATIVE_EQUAL(1, 10, 9.);
  TTS_RELATIVE_EQUAL(42.f, 42.f, 0, REQUIRED);
};

TTS_CASE("Relative distance between boolean")
{
  TTS_RELATIVE_EQUAL(true, true, 0.);
  TTS_RELATIVE_EQUAL(true, false, 1.);
  TTS_RELATIVE_EQUAL(false, false, 0.);
  TTS_RELATIVE_EQUAL(false, true, 1.);
};

TTS_CASE_TPL("Relative distance between integers",
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
  T a = 65, b = static_cast<T>(a + 5);

  TTS_RELATIVE_EQUAL(a, a, 0.);
  TTS_RELATIVE_EQUAL(a, b, 0.0715);
  TTS_RELATIVE_EQUAL(b, a, 0.0715);
};

TTS_CASE_TPL("Relative distance between floating point", float, double)
<typename T>(::tts::type<T>)
{
  T a {1};

  TTS_RELATIVE_EQUAL(a, a, 0.);
  TTS_RELATIVE_EQUAL(a, a + 0.049f, 0.05);
  TTS_RELATIVE_EQUAL(a, a - 0.049f, 0.05);
  TTS_RELATIVE_EQUAL(a + 0.049f, a, 0.05);
  TTS_RELATIVE_EQUAL(a - 0.049f, a, 0.05);
  TTS_RELATIVE_EQUAL(a, a + 0.499f, 0.5);
  TTS_RELATIVE_EQUAL(a, a - 0.499f, 0.5);
  TTS_RELATIVE_EQUAL(a + 0.499f, a, 0.5);
  TTS_RELATIVE_EQUAL(a - 0.499f, a, 0.5);

#if !defined(__FAST_MATH__)
  T qnan = std::numeric_limits<T>::quiet_NaN();
  T inf  = std::numeric_limits<T>::infinity();
  T minf = -inf;

  TTS_RELATIVE_EQUAL(a, qnan, inf);
  TTS_RELATIVE_EQUAL(qnan, a, inf);
  TTS_RELATIVE_EQUAL(qnan, qnan, 0.);

  TTS_RELATIVE_EQUAL(a, inf, inf);
  TTS_RELATIVE_EQUAL(inf, a, inf);
  TTS_RELATIVE_EQUAL(inf, inf, inf);

  TTS_RELATIVE_EQUAL(a, minf, inf);
  TTS_RELATIVE_EQUAL(minf, a, inf);
  TTS_RELATIVE_EQUAL(minf, minf, inf);
#endif
};

TTS_CASE("relative_check treats exactly one NaN operand as infinitely far")
{
#if !defined(__FAST_MATH__)
  double qnan = std::numeric_limits<double>::quiet_NaN();

  TTS_EXPECT(!std::isfinite(tts::relative_check(qnan, 5.)));
  TTS_EXPECT(!std::isfinite(tts::relative_check(5., qnan)));
  TTS_EQUAL(tts::relative_check(qnan, qnan), 0.);
#endif
};

#include "my_real.hpp"

TTS_CASE("Relative distance of type with custom reldist")
{
  TTS_RELATIVE_EQUAL(n1::my_real {1.f}, n1::my_real {8.f}, 12.5);
  TTS_RELATIVE_EQUAL(n1::my_real {8.f}, n1::my_real {1.f}, 800.);
};
