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
#include <array>
#include <cstdint>

TTS_CASE("Ranks of the values of an integer")
{
  using ranks = tts::enumeration<std::int8_t>;

  // The 256 values of std::int8_t, from -128 to 127
  TTS_EQUAL(ranks::last, 255ULL);
  TTS_EQUAL(ranks::index(-128), 0ULL);
  TTS_EQUAL(ranks::value(255), std::int8_t {127});
};

TTS_CASE("Ranks of the values of a float")
{
  using ranks = tts::enumeration<float>;

  // -0 and +0 take two neighboring ranks
  TTS_EQUAL(ranks::index(0.f), ranks::index(-0.f) + 1);
  TTS_LESS(ranks::index(-1.f), ranks::index(1.f));
  TTS_EQUAL(ranks::value(ranks::index(1.5f)), 1.5f);
};

// A type of three values
enum class rounding
{
  down,
  nearest,
  up
};

double shift(rounding r)
{
  return static_cast<double>(r) - 1.;
}

namespace tts
{
  // The name of a rounding in the report of a check
  template<> struct display<rounding>
  {
    static text render(rounding r)
    {
      std::array names {"down", "nearest", "up"};
      return text {names[ static_cast<std::size_t>(r) ]};
    }
  };

  // The three values of rounding, in the order of their declaration
  template<> struct enumeration<rounding>
  {
    static constexpr std::uint64_t last = 2;

    static constexpr std::uint64_t index(rounding r) noexcept
    {
      return static_cast<std::uint64_t>(r);
    }

    static constexpr rounding value(std::uint64_t i) noexcept
    {
      return static_cast<rounding>(i);
    }
  };
}

TTS_CASE("Sweep over the values of a user-defined type")
{
  // Calls shift on down, nearest, and up
  TTS_ULP_RANGE_CHECK(tts::sweep, rounding, rounding, shift, shift, 0.);
};
//! [snippet]
