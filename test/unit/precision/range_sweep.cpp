//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <algorithm>
#include <cstdint>
#include <vector>

TTS_CASE("tts::sweep(lo, hi) calls every value from lo to hi once")
{
  std::vector<int> values;

  auto             ref = [ &values ](int x)
  {
    values.push_back(x);
    return x;
  };
  auto same = [](int x) { return x; };

  TTS_ULP_RANGE_CHECK(tts::sweep(0, 10), int, int, ref, same, 0.);

  TTS_EXPECT(values == (std::vector<int> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
};

TTS_CASE("tts::sweep calls false, then true, once each")
{
  std::vector<int> values;
  auto             ref = [ &values ](bool b)
  {
    values.push_back(b ? 1 : 0);
    return b;
  };
  auto same = [](bool b) { return b; };

  TTS_ULP_RANGE_CHECK(tts::sweep, bool, bool, ref, same, 0.);

  TTS_EXPECT(values == (std::vector<int> {0, 1}));
};

TTS_CASE(
"tts::sweep calls every value and pair of 8-bit integers, and every triple of a range, once")
{
  using u8_t = std::uint8_t;
  std::vector<unsigned char> seen(std::size_t {1} << 16, 0);
  auto                       once = [ &seen ](std::size_t n)
  { return std::count(seen.begin(), seen.begin() + static_cast<std::ptrdiff_t>(n), 1); };

  auto one = [ &seen ](u8_t a)
  {
    ++seen[ a ];
    return a;
  };
  auto two = [ &seen ](u8_t a, u8_t b)
  {
    ++seen[ a * 256u + b ];
    return a;
  };
  auto three = [ &seen ](u8_t a, u8_t b, u8_t c)
  {
    ++seen[ (a * 16u + b) * 16u + c ];
    return a;
  };
  auto first1 = [](u8_t a) { return a; };
  auto first2 = [](u8_t a, u8_t) { return a; };
  auto first3 = [](u8_t a, u8_t, u8_t) { return a; };

  TTS_ULP_RANGE_CHECK(tts::sweep, u8_t, u8_t, one, first1, 0.);
  TTS_EQUAL(once(256), std::ptrdiff_t {256});

  std::ranges::fill(seen, 0);
  TTS_ULP_RANGE_CHECK(
  tts::sweep, (tts::types<u8_t, u8_t>), (tts::types<u8_t, u8_t>), two, first2, 0.);
  TTS_EQUAL(once(65536), std::ptrdiff_t {65536});

  std::ranges::fill(seen, 0);
  TTS_ULP_RANGE_CHECK(tts::sweep(u8_t {0}, u8_t {15}),
                      (tts::types<u8_t, u8_t, u8_t>),
                      (tts::types<u8_t, u8_t, u8_t>),
                      three,
                      first3,
                      0.);
  TTS_EQUAL(once(4096), std::ptrdiff_t {4096});
};

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

  TTS_ULP_RANGE_CHECK(
  (tts::sweep(i16_t {-128}, i16_t {127}), tts::sample(i16_t {-32768}, i16_t {32767}, 64)),
  (tts::types<i16_t, i16_t>),
  (tts::types<i16_t, i16_t>),
  ref,
  first,
  0.);

  TTS_EQUAL(std::ranges::count(as, 64), std::ptrdiff_t {256});
  TTS_EQUAL(std::ranges::count(bs, 256), std::ptrdiff_t {64});
  TTS_EQUAL(bs[ at(-32768) ] + bs[ at(32767) ], 2 * 256);
};

TTS_CASE("Range check gives another generator 4096 values next to tts::sweep")
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
