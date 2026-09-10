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

  TTS_BIT_EQUAL(1.f, 0x3F800000, REQUIRED);
};

TTS_CASE("test bit_not_equal for basic types")
{
  TTS_BIT_NOT_EQUAL(1.f, 0x3F800001);
  TTS_BIT_NOT_EQUAL(true, std::uint8_t {0});

  TTS_BIT_NOT_EQUAL(1.f, 0x3F800001, REQUIRED);
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

//==================================================================================================
// A type whose storage is wider than the value it carries, as a SIMD register holding fewer lanes
// than it has bytes, compares bytes that no operation writes. The bit_equal member of
// tts::comparison is what such a type overrides.
//==================================================================================================
namespace app
{
  struct padded
  {
    std::uint8_t  used;
    std::uint64_t slack;
  };
}

namespace tts
{
  template<>
  struct comparison<app::padded, app::padded> : _::builtin_comparison<app::padded, app::padded>
  {
    static bool bit_equal(app::padded const& l, app::padded const& r)
    {
      return l.used == r.used;
    }
  };
}

TTS_CASE("test that tts::comparison drives the bitwise macros")
{
  app::padded const a {42, 0};
  app::padded const b {42, 0xDEADBEEF};
  app::padded const c {43, 0};

  TTS_BIT_EQUAL(a, b);
  TTS_BIT_NOT_EQUAL(a, c);
};
