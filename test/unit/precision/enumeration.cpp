//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <bit>
#include <cstdint>
#include <limits>

TTS_CASE("tts::enumeration lists every value of an 8-bit integer in order")
{
  using s_t = tts::enumeration<std::int8_t>;
  using u_t = tts::enumeration<std::uint8_t>;

  TTS_EQUAL(s_t::last, 255ULL);
  TTS_EQUAL(s_t::value(0), std::int8_t {-128});
  TTS_EQUAL(u_t::value(0), std::uint8_t {0});

  for(std::uint64_t i = 1; i <= 255; ++i)
  {
    TTS_LESS(s_t::value(i - 1), s_t::value(i));
    TTS_EQUAL(s_t::index(s_t::value(i)), i);
    TTS_LESS(u_t::value(i - 1), u_t::value(i));
  }
};

TTS_CASE("tts::enumeration orders the floats from -inf to +inf, without NaN")
{
  using e   = tts::enumeration<float>;
  using l_t = std::numeric_limits<float>;

  TTS_EQUAL(e::index(0.f), e::index(-0.f) + 1);
  TTS_LESS(e::index(-l_t::infinity()), e::index(l_t::lowest()));
  TTS_LESS(e::index(l_t::lowest()), e::index(-1.f));
  TTS_LESS(e::index(l_t::denorm_min()), e::index(l_t::min()));
  TTS_LESS(e::index(l_t::max()), e::index(l_t::infinity()));
  TTS_EQUAL(e::index(-l_t::infinity()), 0ULL);
  TTS_EQUAL(e::index(l_t::infinity()), e::last);
  TTS_EQUAL(e::last, (1ULL << 32) - (1ULL << 24) + 1);

  for(float x: {-l_t::max(), -1.5f, -l_t::denorm_min(), -0.f, 0.f, 0.1f, l_t::max()})
    TTS_EQUAL(std::bit_cast<std::uint32_t>(e::value(e::index(x))), std::bit_cast<std::uint32_t>(x));
};
