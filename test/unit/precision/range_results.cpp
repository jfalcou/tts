//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "range.hpp"
#include <array>
#include <utility>

std::pair<int, int> halve(int x)
{
  return {x / 2, x % 2};
}

// The remainder of an odd value is 3 instead of 1: one ULP away
std::pair<std::array<int, 4>, std::array<int, 4>> ajar_halve(std::array<int, 4> const& x)
{
  std::pair<std::array<int, 4>, std::array<int, 4>> that {};
  for(std::size_t k = 0; k < 4; ++k)
  {
    that.first[ k ]  = x[ k ] / 2;
    that.second[ k ] = 3 * (x[ k ] % 2);
  }
  return that;
}

TTS_CASE("Range check on a tuple result holds each member to its own bound")
{
  TTS_ULP_RANGE_CHECK(ramp, int, (std::array<int, 4>), halve, ajar_halve, 0., 1.);
};

TTS_CASE(TTS_XFAIL("Range check on a tuple result applies the bounds in the order of the members"))
{
  TTS_ULP_RANGE_CHECK(ramp, int, (std::array<int, 4>), halve, ajar_halve, 1., 0.);
};

TTS_CASE(TTS_XFAIL("Range check on a tuple result applies a single bound to every member"))
{
  TTS_ULP_RANGE_CHECK(ramp, int, (std::array<int, 4>), halve, ajar_halve, 0.5);
};

// The first odd value sits at index 1: the 1 ULP bucket of the remainder marks the second lane.
TTS_CASE("Range check prints one histogram per member of a tuple result")
{
  ::tts::_::scoped_verbosity verbosity({.verbose = true, .quiet = false});

  tts::gathering_sink        gs;
  {
    tts::scoped_sink scope(gs);
    TTS_ULP_RANGE_CHECK(ramp, int, (std::array<int, 4>), halve, ajar_halve, 0., 1.);
  }

  auto has = [ & ](char const* s)
  {
    return std::strstr(gs.content().data(), s) != nullptr; // NOSONAR - contains() is C++23
  };

  TTS_EXPECT(has("Member 0 of the result\n"));
  TTS_EXPECT(has("Member 1 of the result\n"));
  TTS_EXPECT(has("Input:      { 0 *1* 2 3 }\n"));
  TTS_EXPECT(has("Found:      { 0 *3* 0 3 }\n"));
  TTS_EXPECT(has("instead of: { 0 *1* 0 1 }\n"));
};

// A pair of shorts measured as a whole, the sum of the distances of its members
using duo = std::pair<short, short>;

namespace tts
{
  template<> struct precision<duo>
  {
    static double ulp(duo const& a, duo const& b)
    {
      return ::tts::ulp_check(a.first, b.first) + ::tts::ulp_check(a.second, b.second);
    }
  };

  template<> struct adapter<duo>
  {
    using element_type                = duo;
    static constexpr std::size_t size = 1;

    static duo                   load(duo const* src) noexcept
    {
      return *src;
    }

    static void store(duo const& v, duo* dst) noexcept
    {
      *dst = v;
    }
  };
}

duo twin(int x)
{
  return {static_cast<short>(x), static_cast<short>(x)};
}

// Each member is one ULP away, the pair two
duo twin_off(int x)
{
  return {static_cast<short>(x + 2), static_cast<short>(x + 2)};
}

// Split into its members, the result would stay within 1.5 ULP.
TTS_CASE(TTS_XFAIL("Range check measures a result with an adapter of its own as a whole"))
{
  TTS_ULP_RANGE_CHECK(ramp, int, int, twin, twin_off, 1.5);
};
