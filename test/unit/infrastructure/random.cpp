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

// Helper function to extract and calculate the average variation from the sampled buckets
inline float compute_average_variation(std::map<int, int> const& samples, int nb)
{
  // The number of values per bucket should, in average, vary very little
  std::vector<int> input;
  for(auto const& [ b, v ]: samples)
    input.push_back(v);

  std::vector<float> output(input.size());
  std::adjacent_difference(input.begin(),
                           input.end(),
                           output.begin(),
                           [](auto a, auto b)
                           { return static_cast<float>(std::abs(a) - std::abs(b)); });

  float average_variation_per_bucket = 0;
  for(std::size_t i = 1; i < output.size() - 1; ++i)
    average_variation_per_bucket += output[ i ];

  return average_variation_per_bucket / static_cast<float>(nb);
}

TTS_CASE("Check realistic integral distribution")
{
  int                mini = -10'000;
  int                maxi = +10'000;

  std::map<int, int> samples;
  int const          nb = 16'000;

  for(int i = 0; i < nb; ++i)
  {
    auto v      = tts::random_value(mini, maxi);
    auto bucket = v ? static_cast<int>(std::log2(std::abs(v))) + 1 : 0;
    samples[ bucket ]++;
  }

  float avg_variation = compute_average_variation(samples, nb);
  TTS_LESS(avg_variation, 0.5f);
};

TTS_CASE_TPL("Check realistic real distribution", float, double)
<typename Type>(::tts::type<Type>)
{
  Type               mini = -10'000;
  Type               maxi = +10'000;

  std::map<int, int> samples;
  int const          nb = 16'000;

  for(int i = 0; i < nb; ++i)
  {
    auto v = tts::random_value(mini, maxi);
    int  exp;
    std::frexp(v, &exp);
    samples[ exp ]++;
  }

  float avg_variation = compute_average_variation(samples, nb);
  TTS_LESS(avg_variation, 0.25f);
};
