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
  std::vector<float> input;
  for(auto const& [ b, v ]: samples)
    input.push_back(static_cast<float>(v));

  std::vector<float> output(input.size());
  std::adjacent_difference(input.begin(),
                           input.end(),
                           output.begin(),
                           [](auto a, auto b) { return std::abs(a) - std::abs(b); });

  float average_variation_per_bucket = 0;
  for(std::size_t i = 1; i < output.size() - 1; ++i)
    average_variation_per_bucket += output[ i ];

  return average_variation_per_bucket / static_cast<float>(nb);
}

template<typename T> double compute_mean(std::vector<T> const& data)
{
  if(data.empty()) return 0.0;
  T sum = std::accumulate(data.begin(), data.end(), T {0});
  return static_cast<double>(sum) / static_cast<double>(data.size());
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
  return sum_sq_diff / static_cast<double>(data.size());
}

// Checks results' mean and variance against the theoretical values for a discrete uniform
// distribution over [lo, hi]. Relative error of both estimators is independent of the range's
// magnitude (signal and sampling noise both scale with the range and cancel out in the ratio),
// so the same tolerances are valid whether [lo, hi] spans a handful of values or a 64-bit range.
void check_uniform_moments(std::vector<double> const& results,
                           double                     lo,
                           double                     hi,
                           double                     mean_tol,
                           double                     var_tol)
{
  double expected_mean = (lo + hi) / 2.0;
  double actual_mean   = compute_mean(results);
  TTS_RELATIVE_EQUAL(actual_mean, expected_mean, mean_tol);

  double range_n      = hi - lo + 1.0;
  double expected_var = ((range_n * range_n) - 1.0) / 12.0;
  double actual_var   = compute_variance(results, actual_mean);
  TTS_RELATIVE_EQUAL(actual_var, expected_var, var_tol);
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

  check_uniform_moments(
  results, static_cast<double>(min_v), static_cast<double>(max_v), 0.01, 0.05);
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
  TTS_RELATIVE_EQUAL(actual_log_mean, expected_log_mean, 0.01);
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

  double positive_ratio = static_cast<double>(positive_count) / static_cast<double>(samples);

  TTS_RELATIVE_EQUAL(positive_ratio, 0.8, 0.01);
};

TTS_CASE_TPL("Check random_bits Uniformity [Mean & Variance]", tts::uint_types)
<typename T>(tts::type<T>)
{
  tts::random_bits    gen;
  std::size_t         samples = 100000;

  std::vector<double> results;
  results.reserve(samples);

  for(std::size_t i = 0; i < samples; ++i)
    results.push_back(static_cast<double>(gen(tts::type<T> {})));

  double n = std::pow(2.0, static_cast<double>(8 * sizeof(T)));
  check_uniform_moments(results, 0.0, n - 1.0, 0.01, 0.05);
};

TTS_CASE_TPL("Check degenerate range for reals", float, double)
<typename T>(tts::type<T>)
{
  TTS_EQUAL(tts::random_value(T(0), T(0)), T(0));
  TTS_EQUAL(tts::random_value(T(-1.5), T(-1.5)), T(-1.5));
  TTS_EQUAL(tts::random_value(T(42), T(42)), T(42));
};

TTS_CASE_TPL("Check degenerate range for integers", int, long, unsigned int)
<typename T>(tts::type<T>)
{
  TTS_EQUAL(tts::random_value(T(0), T(0)), T(0));
  TTS_EQUAL(tts::random_value(T(7), T(7)), T(7));
};
