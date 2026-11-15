//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>
#include <cmath>
#include <utility>

float my_ldexp(float x, int n)
{
  return x * std::exp2(static_cast<float>(n));
}

TTS_CASE("Check my_ldexp against std::ldexp")
{
  auto std_ldexp = [](float x, int n) { return std::ldexp(x, n); };

  // One generator per argument: every x is checked with every n from -32 to 31
  TTS_ULP_RANGE_CHECK((tts::between(-1.f, 1.f), tts::ramp(-32)),
                      (tts::types<float, int>),
                      (tts::types<float, int>),
                      std_ldexp,
                      my_ldexp,
                      0.);
};

std::pair<float, int> my_frexp(float x)
{
  int e = std::ilogb(x) + 1;
  return {std::scalbn(x, -e), e};
}

TTS_CASE("Check my_frexp against std::frexp")
{
  auto std_frexp = [](float x)
  {
    int   e = 0;
    float m = std::frexp(x, &e);
    return std::pair {m, e};
  };

  // A tuple result is compared member by member, with one bound per member
  TTS_ULP_RANGE_CHECK(tts::between(1.f, 1000.f), float, float, std_frexp, my_frexp, 0., 0.);
};
//! [snippet]
