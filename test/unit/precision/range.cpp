//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <array>

int ok_x(float x)
{
  return static_cast<int>(x);
}

int ajar_x(float x)
{
  return static_cast<int>((int(x) % 2 == 1) ? x : x + x);
}

std::array<int, 4> ajar_ax(std::array<float, 4> const& x)
{
  std::array<int, 4> that {ajar_x(x[ 0 ]), ajar_x(x[ 1 ]), ajar_x(x[ 2 ]), ajar_x(x[ 3 ])};
  return that;
}

namespace tts
{
  template<typename X> struct adapter<std::array<X, 4>>
  {
    using element_type                = X;
    static constexpr std::size_t size = 4;

    static std::array<X, 4>      load(X const* src) noexcept
    {
      return {src[ 0 ], src[ 1 ], src[ 2 ], src[ 3 ]};
    }

    static void store(std::array<X, 4> const& v, X* dst) noexcept
    {
      dst[ 0 ] = v[ 0 ];
      dst[ 1 ] = v[ 1 ];
      dst[ 2 ] = v[ 2 ];
      dst[ 3 ] = v[ 3 ];
    }
  };
}

TTS_CASE("Test stateless range check")
{
  TTS_ULP_RANGE_CHECK(tts::between(0.f, 100.f), float, (std::array<float, 4>), ok_x, ajar_ax, 64);
};

TTS_CASE("Test stateful range check")
{
  TTS_ULP_RANGE_CHECK(
  tts::realistic_generator<float>(-10, 10), float, (std::array<float, 4>), ok_x, ajar_ax, 128);
};

TTS_CASE("Range check next to a local named like a macro local")
{
  auto generator = tts::between(0.f, 100.f);
  TTS_ULP_RANGE_CHECK(generator, float, (std::array<float, 4>), ok_x, ajar_ax, 64);
};

//==================================================================================================
// Functions of several arguments, each with its own scalar type
//==================================================================================================
int scale(float x, int k)
{
  return static_cast<int>(x) * k;
}

std::array<int, 4> ajar_scale(std::array<float, 4> const& x, std::array<int, 4> const& k)
{
  std::array<int, 4> that {scale(x[ 0 ], k[ 0 ]) + (k[ 0 ] % 2),
                           scale(x[ 1 ], k[ 1 ]) + (k[ 1 ] % 2),
                           scale(x[ 2 ], k[ 2 ]) + (k[ 2 ] % 2),
                           scale(x[ 3 ], k[ 3 ]) + (k[ 3 ] % 2)};
  return that;
}

// The cases below need odd values of k, which a ramp has at any block size.
auto ramp = [](auto, auto i, auto) { return static_cast<int>(i); };

TTS_CASE("Range check on a function of two arguments of different types")
{
  TTS_ULP_RANGE_CHECK((tts::between(0.f, 100.f), ramp),
                      (tts::types<float, int>),
                      (tts::types<std::array<float, 4>, std::array<int, 4>>),
                      scale,
                      ajar_scale,
                      0.5);
};

// Passes only with k stuck at zero on the challenger side, which the tag refuses.
TTS_CASE(TTS_XFAIL("Range check on a function of two arguments reads each leaf"))
{
  TTS_ULP_RANGE_CHECK((tts::between(0.f, 100.f), ramp),
                      (tts::types<float, int>),
                      (tts::types<std::array<float, 4>, std::array<int, 4>>),
                      scale,
                      ajar_scale,
                      0.4);
};

TTS_CASE("Range check with one producer shared by every argument")
{
  auto plus  = [](int a, int b) { return a + b; };
  auto aplus = [](std::array<int, 4> const& a, std::array<int, 4> const& b)
  {
    return std::array<int, 4> {a[ 0 ] + b[ 0 ], a[ 1 ] + b[ 1 ], a[ 2 ] + b[ 2 ], a[ 3 ] + b[ 3 ]};
  };

  TTS_ULP_RANGE_CHECK(tts::between(-50, 50),
                      (tts::types<int, int>),
                      (tts::types<std::array<int, 4>, std::array<int, 4>>),
                      plus,
                      aplus,
                      0.);
};

// A block of 4096 pairs is the product of 64 values on each side.
TTS_CASE("Range check on two arguments covers the product of the producers")
{
  static std::array<std::array<bool, 64>, 64> seen = {};

  auto                                        note = [](int a, int b)
  {
    seen[ static_cast<std::size_t>(a) ][ static_cast<std::size_t>(b) ] = true;
    return a - b;
  };
  auto anote = [](std::array<int, 4> const& a, std::array<int, 4> const& b)
  {
    return std::array<int, 4> {a[ 0 ] - b[ 0 ], a[ 1 ] - b[ 1 ], a[ 2 ] - b[ 2 ], a[ 3 ] - b[ 3 ]};
  };

  TTS_ULP_RANGE_CHECK(ramp,
                      (tts::types<int, int>),
                      (tts::types<std::array<int, 4>, std::array<int, 4>>),
                      note,
                      anote,
                      0.);

  int pairs = 0;
  for(auto const& row: seen)
    for(bool b: row)
      pairs += b;

  TTS_EQUAL(pairs, 4096);
};

// The first odd k sits at index 1 of the block: its element alone is the sample of the 0.5 bucket.
TTS_CASE("Range check reports the element of each bucket, under the call form of each function")
{
  ::tts::_::scoped_verbosity verbosity;
  ::tts::_::set_verbose(true);
  ::tts::_::set_quiet(false);

  tts::gathering_sink gs;
  {
    tts::scoped_sink scope(gs);
    TTS_ULP_RANGE_CHECK((tts::between(0.f, 100.f), ramp),
                        (tts::types<float, int>),
                        (tts::types<std::array<float, 4>, std::array<int, 4>>),
                        scale,
                        ajar_scale,
                        0.5);
  }

  auto has = [ & ](char const* s)
  {
    return std::strstr(gs.content().data(), s) != nullptr; // NOSONAR - contains() is C++23
  };

  TTS_EXPECT(has("Comparing: scale(float, int) with "
                 "ajar_scale(std::array<float, 4>, std::array<int, 4>)"));
  TTS_EXPECT(has("Input:      0, 1\n"));
  TTS_EXPECT(has("Found:      1\n"));
  TTS_EXPECT(has("instead of: 0\n"));
};

TTS_CASE("Range check that passes prints nothing without --verbose")
{
  ::tts::_::scoped_verbosity verbosity;
  ::tts::_::set_verbose(false);
  ::tts::_::set_quiet(false);

  tts::gathering_sink gs;
  {
    tts::scoped_sink scope(gs);
    TTS_ULP_RANGE_CHECK(tts::between(0.f, 100.f), float, (std::array<float, 4>), ok_x, ajar_ax, 64);
  }

  TTS_EXPECT(gs.content().is_empty());
};
