//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <numeric>
#include <vector>
#include <map>
#include <cmath>

TTS_CASE("Check realistic integral distribution")
{
  int mini = -10'000;
  int maxi = +10'000;

  std::map<int, int> samples;
  int const nb = 16'000;

  for(int i=0;i<nb;++i)
  {
    auto v = tts::random_value(mini,maxi);
    auto bucket = v ? static_cast<int>(std::log2(std::abs(v)) ) + 1 : 0;
    samples[bucket]++;
  }

  // The number of values per bucket should, in average, vary very little
  std::vector<int> input;
  for(auto[b,v] : samples) input.push_back(v);

  std::vector<int> output(input.size());
  std::adjacent_difference( input.begin(), input.end(), output.begin()
                          , [](auto a, auto b) { return std::abs(a) - std::abs(b); }
                          );

  float average_variation_per_bucket = 0;
  for(std::size_t i=1;i<output.size()-1;++i) average_variation_per_bucket += output[i];
  average_variation_per_bucket /= nb;

  TTS_LESS(average_variation_per_bucket, 0.5f);
};

TTS_CASE_TPL("Check realistic real distribution"
            , float, double
            )
<typename Type>(::tts::type<Type>)
{
  Type mini = -10'000;
  Type maxi = +10'000;

  std::map<int, int> samples;
  int nb = 16'000;

  for(int i=0;i<nb;++i)
  {
    auto v = tts::random_value(mini,maxi);
    int exp;
    std::frexp(v, &exp);
    samples[exp]++;
  }

  // The number of values per bucket should, in average, vary very little
  std::vector<int> input;
  for(auto[b,v] : samples) input.push_back(v);

  std::vector<int> output(input.size());
  std::adjacent_difference( input.begin(), input.end(), output.begin()
                          , [](auto a, auto b) { return std::abs(a) - std::abs(b); }
                          );

  float average_variation_per_bucket = 0;
  for(std::size_t i=1;i<output.size()-1;++i) average_variation_per_bucket += output[i];
  average_variation_per_bucket /= nb;

  TTS_LESS(average_variation_per_bucket, 0.25f);
};