//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

TTS_CASE(TTS_XINVALID("A range check on a sample of no value records no check"))
{
  auto same = [](int x) { return x; };

  TTS_ULP_RANGE_CHECK(tts::sample(0), int, int, same, same, 0.);
  TTS_ULP_RANGE_CHECK(tts::sample(0, 10, 0), int, int, same, same, 0.);
};

TTS_CASE("tts::sample(lo, hi, 1) calls the lower bound alone")
{
  std::vector<int> values;
  auto             ref = [ &values ](int x)
  {
    values.push_back(x);
    return x;
  };
  auto same = [](int x) { return x; };

  TTS_ULP_RANGE_CHECK(tts::sample(10, 0, 1), int, int, ref, same, 0.);

  TTS_EXPECT(values == (std::vector<int> {0}));
};

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
