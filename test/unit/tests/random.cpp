//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>

template<typename T> double compute_mean(std::vector<T> const& data)
{
  if(data.empty()) return 0.0;
  double sum = std::accumulate(data.begin(), data.end(), 0.0);
  return sum / data.size();
}

template<typename T> double compute_variance(std::vector<T> const& data, double mean)
{
  if(data.size() < 2) return 0.0;
  double sum_sq_diff = 0.0;
  for(T val: data)
  {
    double diff  = val - mean;
    sum_sq_diff += diff * diff;
  }
  return sum_sq_diff / data.size();
}

TTS_CASE_TPL("Check Integer Uniformity [Mean & Variance]", int, long, unsigned int)
<typename T>(tts::type<T>)
{
  T                   min_v   = 10;
  T                   max_v   = 30;
  std::size_t         samples = 100000;

  std::vector<double> results;
  results.reserve(samples);

  for(std::size_t i = 0; i < samples; ++i)
  {
    T val = tts::random_value(min_v, max_v);
    results.push_back(static_cast<double>(val));

    TTS_EXPECT(val >= min_v);
    TTS_EXPECT(val <= max_v);
  }

  double expected_mean = (static_cast<double>(min_v) + max_v) / 2.0;
  double actual_mean   = compute_mean(results);
  TTS_RELATIVE_EQUAL(actual_mean, expected_mean, 1.0);

  double range_n      = static_cast<double>(max_v - min_v + 1);
  double expected_var = ((range_n * range_n) - 1.0) / 12.0;
  double actual_var   = compute_variance(results, actual_mean);
  TTS_RELATIVE_EQUAL(actual_var, expected_var, 5.0);
};

TTS_CASE_TPL("Check Float Log-Uniformity", float, double)
<typename T>(tts::type<T>)
{
  T                   min_v   = 1;
  T                   max_v   = 1000;
  std::size_t         samples = 100000;

  std::vector<double> log_results;
  log_results.reserve(samples);

  for(std::size_t i = 0; i < samples; ++i)
  {
    T val = tts::random_value(min_v, max_v);

    TTS_EXPECT(val >= min_v);
    TTS_EXPECT(val <= max_v);
    log_results.push_back(std::log10(val));
  }

  double expected_log_mean = (std::log10(min_v) + std::log10(max_v)) / 2.0;
  double actual_log_mean   = compute_mean(log_results);
  TTS_RELATIVE_EQUAL(actual_log_mean, expected_log_mean, 1.0);
};

TTS_CASE_TPL("Check Zero Crossing Probabilities", float, double)
<typename T>(tts::type<T>)
{
  T           min_v          = -20;
  T           max_v          = 80;
  std::size_t samples        = 100000;
  int         positive_count = 0;

  for(std::size_t i = 0; i < samples; ++i)
  {
    T val = tts::random_value(min_v, max_v);
    if(val > 0) positive_count++;
  }

  double positive_ratio = static_cast<double>(positive_count) / samples;

  TTS_RELATIVE_EQUAL(positive_ratio, 0.8, 1.0);
};
