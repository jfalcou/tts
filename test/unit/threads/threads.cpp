//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <utility>

// Pure functions a few ULP apart, so that each histogram fills several buckets.
float root(float x)
{
  return static_cast<float>(std::sqrt(static_cast<double>(x)));
}

float rough_root(float x)
{
  float r    = std::sqrt(x);
  auto  bits = std::bit_cast<std::uint32_t>(x);
  if(bits % 7 == 0) r = std::nextafter(r, 1e30f);
  if(bits % 11 == 0) r = std::nextafter(std::nextafter(r, 0.f), 0.f);
  return r;
}

// The verbose report of the checks run by f on the given number of threads.
template<typename F> tts::text report(int threads, F f)
{
  ::tts::_::scoped_verbosity verbosity({.verbose = true, .quiet = false});
  tts::gathering_sink        gs;
  {
    tts::scoped_sink scope(gs);
    omp_set_num_threads(threads);
    f();
  }
  return gs.content();
}

TTS_CASE("Range checks report the same on one thread and on four")
{
  auto checks = []
  {
    auto ratio = [](std::int16_t a, std::int16_t b)
    { return static_cast<float>(a) / static_cast<float>(b | 1); };
    auto rough_ratio = [ ratio ](std::int16_t a, std::int16_t b)
    { return a % 5 == 0 ? std::nextafter(ratio(a, b), 1e30f) : ratio(a, b); };
    auto both       = [](float x) { return std::pair {root(x), x * x}; };
    auto rough_both = [](float x) { return std::pair {rough_root(x), x * x}; };

    TTS_ULP_RANGE_CHECK(tts::sweep(1.f, 2.f), float, float, root, rough_root, 4.);
    TTS_ULP_RANGE_CHECK(tts::sample(0.5f, 64.f, 1 << 20), float, float, root, rough_root, 4.);
    TTS_ULP_RANGE_CHECK(tts::sample(1 << 10),
                        (tts::types<std::int16_t, std::int16_t>),
                        (tts::types<std::int16_t, std::int16_t>),
                        ratio,
                        rough_ratio,
                        4.);
    TTS_ULP_RANGE_CHECK(tts::sample(1.f, 4.f, 1 << 20), float, float, both, rough_both, 4.);
  };

  auto one  = report(1, checks);
  auto four = report(4, checks);

  TTS_EXPECT(std::strstr(one.data(), "Max ULP") != nullptr); // NOSONAR - contains() is C++23
  TTS_EXPECT(std::strcmp(one.data(), four.data()) == 0) << one.data() << four.data();
};
