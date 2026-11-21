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
#include <limits>
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

// When the values, or their combinations, fit in the scope, each one is called exactly once.
TTS_CASE("tts::sweep calls every value of a 16-bit integer once")
{
  std::size_t count = 0;
  std::vector seen(65536, 0);

  auto        ref = [ &count, &seen ](std::int16_t x)
  {
    ++count;
    ++seen[ static_cast<std::size_t>(x + 32768) ];
    return x;
  };
  auto same = [](std::int16_t x) { return x; };

  TTS_ULP_RANGE_CHECK(tts::sweep, std::int16_t, std::int16_t, ref, same, 0.);

  TTS_EQUAL(count, std::size_t {65536});
  TTS_EQUAL(std::ranges::count(seen, 1), std::ptrdiff_t {65536});
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

// 256 values, 256^2 pairs, and 256^3 = 2^24 triples: each fits in the scope of 2^24.
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

// The floats from 10 to 11 are 2^20 + 1 values: a scope of that many calls each once, one less
// samples them and leaves one out.
TTS_CASE("tts::sweep(lo, hi, n) calls every value once when n reaches their number")
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

  TTS_ULP_RANGE_CHECK(tts::sweep(10.f, 11.f, values), float, float, ref, same, 0.);
  TTS_EQUAL(count, values);
  TTS_EQUAL(std::ranges::count(seen, 1), static_cast<std::ptrdiff_t>(values));

  count = 0;
  std::ranges::fill(seen, 0);
  TTS_ULP_RANGE_CHECK(tts::sweep(10.f, 11.f, values - 1), float, float, ref, same, 0.);
  TTS_EQUAL(count, values - 1);
  TTS_EQUAL(std::ranges::count(seen, 0), std::ptrdiff_t {1});
};

// More floats than the scope: 2^24 calls, with the special values, every binade, and no NaN.
TTS_CASE("Range check samples floats with their special values and every binade")
{
  using l_t         = std::numeric_limits<float>;

  std::size_t count = 0;
  std::size_t nans  = 0;
  std::vector binade(512, false);
  std::vector special(5, false);

  auto        ref = [ &count, &nans, &binade, &special ](float x)
  {
    ++count;
    binade[ std::bit_cast<std::uint32_t>(x) >> 23 ] = true;

    std::array<float, 5> const values               = {
    -l_t::infinity(), l_t::infinity(), -0.f, 0.f, l_t::denorm_min()};
    for(std::size_t k = 0; k < values.size(); ++k)
      if(std::bit_cast<std::uint32_t>(x) == std::bit_cast<std::uint32_t>(values[ k ]))
        special[ k ] = true;
    if(std::isnan(x)) ++nans;
    return x;
  };
  auto same = [](float x) { return x; };

  TTS_ULP_RANGE_CHECK(tts::sweep, float, float, ref, same, 0.);

  TTS_EQUAL(count, std::size_t {1} << 24);
  TTS_EQUAL(nans, std::size_t {0});
  TTS_EQUAL(std::ranges::count(binade, true), std::ptrdiff_t {512});
  TTS_EQUAL(std::ranges::count(special, true), std::ptrdiff_t {5});
};

// 2^32 pairs, more than the scope: in 2^24 calls, each argument takes each of its values 256 times.
TTS_CASE("Range check above the scope gives each 16-bit argument all its values")
{
  std::vector as(65536, 0);
  std::vector bs(65536, 0);
  std::size_t equal = 0;

  auto        ref   = [ &as, &bs, &equal ](std::int16_t a, std::int16_t b)
  {
    ++as[ static_cast<std::size_t>(tts::enumeration<std::int16_t>::index(a)) ];
    ++bs[ static_cast<std::size_t>(tts::enumeration<std::int16_t>::index(b)) ];
    equal += a == b ? 1 : 0;
    return a;
  };
  auto first = [](std::int16_t a, std::int16_t) { return a; };

  TTS_ULP_RANGE_CHECK(tts::sweep,
                      (tts::types<std::int16_t, std::int16_t>),
                      (tts::types<std::int16_t, std::int16_t>),
                      ref,
                      first,
                      0.);

  TTS_EQUAL(std::ranges::count(as, 256), std::ptrdiff_t {65536});
  TTS_EQUAL(std::ranges::count(bs, 256), std::ptrdiff_t {65536});
  TTS_LESS(equal, std::size_t {512});
};

// Three floats above the scope: each argument still covers every binade.
TTS_CASE("Range check above the scope gives each float argument every binade")
{
  std::vector xs(512, false);
  std::vector ys(512, false);
  std::vector zs(512, false);

  auto        ref = [ &xs, &ys, &zs ](float x, float y, float z)
  {
    xs[ std::bit_cast<std::uint32_t>(x) >> 23 ] = true;
    ys[ std::bit_cast<std::uint32_t>(y) >> 23 ] = true;
    zs[ std::bit_cast<std::uint32_t>(z) >> 23 ] = true;
    return x;
  };
  auto first = [](float x, float, float) { return x; };

  TTS_ULP_RANGE_CHECK(
  tts::sweep, (tts::types<float, float, float>), (tts::types<float, float, float>), ref, first, 0.);

  TTS_EQUAL(std::ranges::count(xs, true), std::ptrdiff_t {512});
  TTS_EQUAL(std::ranges::count(ys, true), std::ptrdiff_t {512});
  TTS_EQUAL(std::ranges::count(zs, true), std::ptrdiff_t {512});
};

// A ramp is not tts::sweep: the check keeps the --block grid, 64 values per argument.
TTS_CASE("Range check mixing tts::sweep and another generator uses --block")
{
  std::size_t count = 0;

  auto        ref   = [ &count ](float x, int)
  {
    ++count;
    return x;
  };
  auto first = [](float x, int) { return x; };
  auto ramp  = [](auto, auto i, auto) { return static_cast<int>(i); };

  TTS_ULP_RANGE_CHECK(
  (tts::sweep, ramp), (tts::types<float, int>), (tts::types<float, int>), ref, first, 0.);

  TTS_EQUAL(count, std::size_t {4096});
};

// 16777217 floats from 1 to 4, more than the scope: the sample still holds both bounds.
TTS_CASE("tts::sweep(lo, hi) sample includes its bounds")
{
  bool low  = false;
  bool high = false;

  auto ref  = [ &low, &high ](float x)
  {
    low  = low || x == 1.f;
    high = high || x == 4.f;
    return x;
  };
  auto same = [](float x) { return x; };

  TTS_ULP_RANGE_CHECK(tts::sweep(1.f, 4.f), float, float, ref, same, 0.);

  TTS_EXPECT(low);
  TTS_EXPECT(high);
};

// A scope given to tts::sweep replaces the default for its check alone.
TTS_CASE("tts::sweep(n) and tts::sweep(lo, hi, n) set the scope of their check")
{
  std::size_t count = 0;

  auto        ref   = [ &count ](float x)
  {
    ++count;
    return x;
  };
  auto same = [](float x) { return x; };

  TTS_ULP_RANGE_CHECK(tts::sweep(1024), float, float, ref, same, 0.);
  TTS_EQUAL(count, std::size_t {1024});

  count = 0;
  TTS_ULP_RANGE_CHECK(tts::sweep(1.f, 4.f, 4096), float, float, ref, same, 0.);
  TTS_EQUAL(count, std::size_t {4096});
};

// A check reads --valmin and --valmax when it starts, so a case can set them for its checks.
// --cores=1 keeps the reference on one thread under OpenMP, since it records its calls.
TTS_CASE("--valmin and --valmax replace the bounds of tts::sweep")
{
  std::vector<int> values;

  auto             ref = [ &values ](std::int16_t x)
  {
    values.push_back(x);
    return x;
  };
  auto                       same  = [](std::int16_t x) { return x; };

  auto                       saved = ::tts::_::current_arguments;
  std::array<char const*, 4> argv {"range_sweep", "--valmin=-3", "--valmax=5", "--cores=1"};
  ::tts::_::current_arguments = ::tts::options {4, argv.data()};

  TTS_ULP_RANGE_CHECK(tts::sweep, std::int16_t, std::int16_t, ref, same, 0.);
  auto whole = values;

  values.clear();
  TTS_ULP_RANGE_CHECK(tts::sweep(0, 100), std::int16_t, std::int16_t, ref, same, 0.);

  ::tts::_::current_arguments = saved;

  std::vector expected {-3, -2, -1, 0, 1, 2, 3, 4, 5};
  TTS_EXPECT(whole == expected);
  TTS_EXPECT(values == expected);
};

TTS_CASE("--valmin and --valmax replace the bounds of tts::randoms")
{
  std::size_t outside = 0;

  auto        ref     = [ &outside ](float x)
  {
    outside += (x < 1.f || x > 2.f) ? 1 : 0;
    return x;
  };
  auto                       same  = [](float x) { return x; };

  auto                       saved = ::tts::_::current_arguments;
  std::array<char const*, 3> argv {"range_sweep", "--valmin=1", "--valmax=2"};
  ::tts::_::current_arguments = ::tts::options {3, argv.data()};

  TTS_ULP_RANGE_CHECK(tts::randoms(-1000.f, 1000.f), float, float, ref, same, 0.);

  ::tts::_::current_arguments = saved;

  TTS_EQUAL(outside, std::size_t {0});
};
