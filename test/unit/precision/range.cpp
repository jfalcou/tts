//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <array>
#include <limits>
#include <utility>

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
      pairs += b ? 1 : 0;

  TTS_EQUAL(pairs, 4096);
};

// The first odd k sits at index 1 of the block: the 0.5 bucket marks the second lane of the first group.
TTS_CASE(
"Range check marks the element of each bucket in its group, under the call form of each function")
{
  ::tts::_::scoped_verbosity verbosity({.verbose = true, .quiet = false});

  tts::gathering_sink        gs;
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

  // Type names come from tts::typename_, whose spelling depends on the compiler
  TTS_EXPECT(has(tts::text("Comparing: scale(%s, %s) with ajar_scale(%s, %s)",
                           tts::as_text(tts::typename_<float>).data(),
                           tts::as_text(tts::typename_<int>).data(),
                           tts::as_text(tts::typename_<std::array<float, 4>>).data(),
                           tts::as_text(tts::typename_<std::array<int, 4>>).data())
                 .data()));
  TTS_EXPECT(has("Expression: ajar_scale similar to scale, at most 0.5 ULP apart"));
  TTS_EXPECT(has("Input:      { 0 *0* 0 0 }, { 0 *1* 2 3 }\n"));
  TTS_EXPECT(has("Found:      { 0 *1* 0 1 }\n"));
  TTS_EXPECT(has("instead of: { 0 *0* 0 0 }\n"));
};

TTS_CASE("Range check that passes prints nothing without --verbose")
{
  ::tts::_::scoped_verbosity verbosity({.verbose = false, .quiet = false});

  tts::gathering_sink        gs;
  {
    tts::scoped_sink scope(gs);
    TTS_ULP_RANGE_CHECK(tts::between(0.f, 100.f), float, (std::array<float, 4>), ok_x, ajar_ax, 64);
  }

  TTS_EXPECT(gs.content().is_empty());
};

TTS_CASE("Range check hands its generators the signed index of TTS_CASE_WITH")
{
  bool signed_index = false;
  auto probe        = [ & ]<typename I>(auto, I i, auto)
  {
    signed_index = std::is_signed_v<I>;
    return static_cast<int>(i);
  };

  auto same = [](int x) { return x; };
  TTS_ULP_RANGE_CHECK(probe, int, int, same, same, 0.);
  TTS_EXPECT(signed_index);
};

// 4096 is no fifth power: the data set grows to 6^5 values, six per argument.
TTS_CASE("Range check on five arguments covers every combination of their values")
{
  static std::array<bool, 7776> seen = {};

  auto                          note = [](int a, int b, int c, int d, int e)
  {
    seen[ static_cast<std::size_t>((((a * 6 + b) * 6 + c) * 6 + d) * 6 + e) ] = true;
    return a;
  };

  TTS_ULP_RANGE_CHECK(ramp,
                      (tts::types<int, int, int, int, int>),
                      (tts::types<int, int, int, int, int>),
                      note,
                      note,
                      0.);

  int combinations = 0;
  for(bool b: seen)
    combinations += b ? 1 : 0;

  TTS_EQUAL(combinations, 7776);
};

//==================================================================================================
// Functions whose result is a tuple
//==================================================================================================
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

// (1. + 1.) * 2. is a bound of 4, and 1. + 1. * 2. one of 3.
TTS_CASE("Range check takes a bound that opens with a parenthesis")
{
  auto same = [](int x) { return x; };
  auto off8 = [](int x) { return x + 8; };
  TTS_ULP_RANGE_CHECK(ramp, int, int, same, off8, (1. + 1.) * 2.);
};

TTS_CASE(TTS_XFAIL("Range check fails on a bound that is not a number"))
{
  auto same = [](int x) { return x; };
  TTS_ULP_RANGE_CHECK(ramp, int, int, same, same, std::numeric_limits<double>::quiet_NaN());
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
