//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE("test bit_equal for basic types")
{
  TTS_BIT_EQUAL(1.f, 0x3F800000);
  TTS_BIT_EQUAL(1.0, 0x3FF0000000000000ULL);
  TTS_BIT_EQUAL(true, std::uint8_t {1});
  TTS_BIT_EQUAL(false, std::uint8_t {0});
  TTS_BIT_EQUAL('A', std::uint8_t {65});
};

struct ab
{
  int   i;
  float b;
};

struct ba
{
  float b;
  int   i;
};

TTS_CASE("test bit_equal for structures")
{
  TTS_BIT_EQUAL((ab {0x3F800000, 1.f}), (ab {0x3F800000, 1.f}));
  TTS_BIT_EQUAL((ab {0x3F800000, 1.f}), (ba {1.f, 0x3F800000}));
  TTS_BIT_EQUAL((std::pair<int, float> {0x3F800000, 1.f}),
                (std::pair<int, float> {0x3F800000, 1.f}));
  TTS_BIT_EQUAL((std::pair<int, float> {0x3F800000, 1.f}), (ab {0x3F800000, 1.f}));
};
