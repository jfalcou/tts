//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <cstdint>
#include <vector>

TTS_CASE("A swept range check draws from the seed of the run")
{
  auto draws = [](std::uint64_t seed)
  {
    std::vector<int> values;
    auto             ref = [ &values ](int x)
    {
      values.push_back(tts::random_value(0, 1000));
      return x;
    };
    auto same = [](int x) { return x; };

    tts::set_random_seed(seed);
    TTS_ULP_RANGE_CHECK(tts::sweep(0, 99), int, int, ref, same, 0.);
    return values;
  };

  TTS_EXPECT(draws(1) == draws(1));
  TTS_EXPECT(draws(1) != draws(2));
};

TTS_CASE("tts::sample checks the same values under one seed and others under another")
{
  auto values = [](std::uint64_t seed)
  {
    std::vector<int> seen;
    auto             ref = [ &seen ](int x)
    {
      seen.push_back(x);
      return x;
    };
    auto same = [](int x) { return x; };

    tts::set_random_seed(seed);
    TTS_ULP_RANGE_CHECK(tts::sample(0, 1 << 20, 100), int, int, ref, same, 0.);
    return seen;
  };

  TTS_EXPECT(values(1) == values(1));
  TTS_EXPECT(values(1) != values(2));
};
