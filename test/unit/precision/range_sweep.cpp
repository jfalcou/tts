//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <utility>
#include <vector>

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

TTS_CASE("tts::sweep(lo, hi) calls every value from lo to hi once")
{
  std::size_t      count = 0;
  std::vector<int> values;

  auto             ref = [ &count, &values ](int x)
  {
    ++count;
    values.push_back(x);
    return x;
  };
  auto same = [](int x) { return x; };

  TTS_ULP_RANGE_CHECK(tts::sweep(0, 10), int, int, ref, same, 0.);

  TTS_EQUAL(count, std::size_t {11});
  TTS_EXPECT(values == (std::vector<int> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
};

// A check runs every combination: 256 values, 256^2 pairs, and 256^3 triples.
TTS_CASE("tts::sweep calls every value, pair, and triple of 8-bit integers once")
{
  using u8_t                       = std::uint8_t;
  std::size_t                count = 0;
  std::vector<unsigned char> seen(std::size_t {1} << 24, 0);
  auto                       once = [ &seen ](std::size_t n)
  { return std::count(seen.begin(), seen.begin() + static_cast<std::ptrdiff_t>(n), 1); };

  auto one = [ &count, &seen ](u8_t a)
  {
    ++count;
    ++seen[ a ];
    return a;
  };
  auto two = [ &count, &seen ](u8_t a, u8_t b)
  {
    ++count;
    ++seen[ a * 256u + b ];
    return a;
  };
  auto three = [ &count, &seen ](u8_t a, u8_t b, u8_t c)
  {
    ++count;
    ++seen[ (a * 256u + b) * 256u + c ];
    return a;
  };
  auto first1 = [](u8_t a) { return a; };
  auto first2 = [](u8_t a, u8_t) { return a; };
  auto first3 = [](u8_t a, u8_t, u8_t) { return a; };

  TTS_ULP_RANGE_CHECK(tts::sweep, u8_t, u8_t, one, first1, 0.);
  TTS_EQUAL(count, std::size_t {256});
  TTS_EQUAL(once(256), std::ptrdiff_t {256});

  count = 0;
  std::ranges::fill(seen, 0);
  TTS_ULP_RANGE_CHECK(
  tts::sweep, (tts::types<u8_t, u8_t>), (tts::types<u8_t, u8_t>), two, first2, 0.);
  TTS_EQUAL(count, std::size_t {65536});
  TTS_EQUAL(once(65536), std::ptrdiff_t {65536});

  count = 0;
  std::ranges::fill(seen, 0);
  TTS_ULP_RANGE_CHECK(
  tts::sweep, (tts::types<u8_t, u8_t, u8_t>), (tts::types<u8_t, u8_t, u8_t>), three, first3, 0.);
  TTS_EQUAL(count, std::size_t {1} << 24);
  TTS_EQUAL(once(std::size_t {1} << 24), std::ptrdiff_t {1} << 24);
};

// The floats from 10 to 11 are 2^20 + 1 values: a sample of that many calls each once, one less
// leaves one out.
TTS_CASE("tts::sample(lo, hi, n) calls every value once when n reaches their number")
{
  using e            = tts::enumeration<float>;
  auto const  values = static_cast<std::size_t>(e::index(11.f) - e::index(10.f) + 1);
  std::size_t count  = 0;
  std::vector seen(values, 0);

  auto        ref = [ &count, &seen ](float x)
  {
    ++count;
    ++seen[ static_cast<std::size_t>(e::index(x) - e::index(10.f)) ];
    return x;
  };
  auto same = [](float x) { return x; };

  TTS_EQUAL(values, std::size_t {1048577});

  TTS_ULP_RANGE_CHECK(tts::sample(10.f, 11.f, values), float, float, ref, same, 0.);
  TTS_EQUAL(count, values);
  TTS_EQUAL(std::ranges::count(seen, 1), static_cast<std::ptrdiff_t>(values));

  count = 0;
  std::ranges::fill(seen, 0);
  TTS_ULP_RANGE_CHECK(tts::sample(10.f, 11.f, values - 1), float, float, ref, same, 0.);
  TTS_EQUAL(count, values - 1);
  TTS_EQUAL(std::ranges::count(seen, 0), std::ptrdiff_t {1});
};

TTS_CASE("tts::sample(n) holds its bounds, every binade, and no NaN")
{
  using l_t         = std::numeric_limits<float>;

  std::size_t count = 0;
  std::size_t nans  = 0;
  bool        low   = false;
  bool        high  = false;
  std::vector binade(512, false);

  auto        ref = [ &count, &nans, &low, &high, &binade ](float x)
  {
    ++count;
    binade[ std::bit_cast<std::uint32_t>(x) >> 23 ] = true;
    low                                             = low || x == -l_t::infinity();
    high                                            = high || x == l_t::infinity();
    if(std::isnan(x)) ++nans;
    return x;
  };
  auto same = [](float x) { return x; };

  TTS_ULP_RANGE_CHECK(tts::sample(1 << 20), float, float, ref, same, 0.);

  TTS_EQUAL(count, std::size_t {1} << 20);
  TTS_EQUAL(nans, std::size_t {0});
  TTS_EXPECT(low && high);
  TTS_EQUAL(std::ranges::count(binade, true), std::ptrdiff_t {512});
};

// A sweep times a sample of 256: each value of the first argument meets the same 256 values.
TTS_CASE("Range check runs every combination of tts::sweep and tts::sample")
{
  using i16_t = std::int16_t;
  std::vector as(65536, 0);
  std::vector bs(65536, 0);

  auto        at  = [](int v) { return static_cast<std::size_t>(v + 32768); };
  auto        ref = [ &as, &bs, &at ](i16_t a, i16_t b)
  {
    ++as[ at(a) ];
    ++bs[ at(b) ];
    return a;
  };
  auto first = [](i16_t a, i16_t) { return a; };

  TTS_ULP_RANGE_CHECK((tts::sweep, tts::sample(i16_t {-32768}, i16_t {32767}, 256)),
                      (tts::types<i16_t, i16_t>),
                      (tts::types<i16_t, i16_t>),
                      ref,
                      first,
                      0.);

  TTS_EQUAL(std::ranges::count(as, 256), std::ptrdiff_t {65536});
  TTS_EQUAL(std::ranges::count(bs, 65536), std::ptrdiff_t {256});
  TTS_EQUAL(bs[ at(-32768) ] + bs[ at(32767) ], 2 * 65536);
};

// A ramp is neither tts::sweep nor tts::sample: it gives --block values, 4096, next to the sweep.
TTS_CASE("Range check gives another generator --block values next to tts::sweep")
{
  std::size_t count = 0;

  auto        ref   = [ &count ](std::int8_t x, int)
  {
    ++count;
    return x;
  };
  auto first = [](std::int8_t x, int) { return x; };
  auto ramp  = [](auto, auto i, auto) { return static_cast<int>(i); };

  TTS_ULP_RANGE_CHECK((tts::sweep, ramp),
                      (tts::types<std::int8_t, int>),
                      (tts::types<std::int8_t, int>),
                      ref,
                      first,
                      0.);

  TTS_EQUAL(count, std::size_t {256} * 4096);
};

// Two doubles have 2^128 combinations: the check fails before its first call.
TTS_CASE(TTS_XFAIL("Range check fails when its calls do not fit in std::size_t"))
{
  std::size_t count = 0;

  auto        ref   = [ &count ](double x, double)
  {
    ++count;
    return x;
  };
  auto first = [](double x, double) { return x; };

  TTS_ULP_RANGE_CHECK(
  tts::sweep, (tts::types<double, double>), (tts::types<double, double>), ref, first, 0.);
  TTS_EQUAL(count, std::size_t {0});
};

// A check reads --valmin and --valmax when it starts, so a case can set them for its checks.
TTS_CASE("--valmin and --valmax narrow tts::sweep and tts::sample, with a note")
{
  std::vector<int> values;

  auto             ref = [ &values ](std::int16_t x)
  {
    values.push_back(x);
    return x;
  };
  auto                       same  = [](std::int16_t x) { return x; };

  auto                       saved = ::tts::_::current_arguments;
  std::array<char const*, 3> argv {"range_sweep", "--valmin=-3", "--valmax=5"};
  ::tts::_::current_arguments = ::tts::options {3, argv.data()};

  tts::gathering_sink gs;
  std::vector<int>    whole;
  std::vector<int>    bounded;
  std::vector<int>    sampled;
  std::vector<int>    inside;
  {
    tts::scoped_sink scope(gs);

    TTS_ULP_RANGE_CHECK(tts::sweep, std::int16_t, std::int16_t, ref, same, 0.);
    whole = std::exchange(values, {});
    TTS_ULP_RANGE_CHECK(tts::sweep(0, 100), std::int16_t, std::int16_t, ref, same, 0.);
    bounded = std::exchange(values, {});
    TTS_ULP_RANGE_CHECK(tts::sample(0, 100, 9), std::int16_t, std::int16_t, ref, same, 0.);
    sampled = std::exchange(values, {});
    TTS_ULP_RANGE_CHECK(tts::sweep(-2, 2), std::int16_t, std::int16_t, ref, same, 0.);
    inside = std::exchange(values, {});
    TTS_ULP_RANGE_CHECK(tts::sweep(10, 20), std::int16_t, std::int16_t, ref, same, 0.);
  }

  ::tts::_::current_arguments = saved;

  auto has                    = [ &gs ](char const* s)
  {
    return std::strstr(gs.content().data(), s) != nullptr; // NOSONAR - contains() is C++23
  };

  TTS_EXPECT(whole == (std::vector {-3, -2, -1, 0, 1, 2, 3, 4, 5}));
  TTS_EXPECT(bounded == (std::vector {0, 1, 2, 3, 4, 5}));
  TTS_EXPECT(sampled == (std::vector {0, 1, 2, 3, 4, 5}));
  TTS_EXPECT(inside == (std::vector {-2, -1, 0, 1, 2}));
  TTS_EXPECT(values.empty());

  TTS_EXPECT(has("--valmin and --valmax narrow argument 1 to [-3, 5]"));
  TTS_EXPECT(has("--valmin and --valmax narrow argument 1 to [0, 5]"));
  TTS_EXPECT(!has("to [-2, 2]"));
  TTS_EXPECT(has("--valmin and --valmax leave no value of argument 1"));
};

// [1.5, 1.75] lies inside the options and keeps its own bounds.
TTS_CASE("--valmin and --valmax narrow tts::randoms")
{
  float       lo      = 0.f;
  float       hi      = 0.f;
  std::size_t outside = 0;

  auto        ref     = [ &lo, &hi, &outside ](float x)
  {
    outside += (x < lo || x > hi) ? 1 : 0;
    return x;
  };
  auto                       same  = [](float x) { return x; };

  auto                       saved = ::tts::_::current_arguments;
  std::array<char const*, 3> argv {"range_sweep", "--valmin=1", "--valmax=2"};
  ::tts::_::current_arguments = ::tts::options {3, argv.data()};

  tts::gathering_sink gs;
  {
    tts::scoped_sink scope(gs);

    lo = 1.f;
    hi = 2.f;
    TTS_ULP_RANGE_CHECK(tts::randoms(-1000.f, 1000.f), float, float, ref, same, 0.);
    lo = 1.5f;
    hi = 1.75f;
    TTS_ULP_RANGE_CHECK(tts::randoms(1.5f, 1.75f), float, float, ref, same, 0.);
  }

  ::tts::_::current_arguments = saved;

  TTS_EQUAL(outside, std::size_t {0});
};
