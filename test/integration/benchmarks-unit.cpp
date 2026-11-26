#ifdef TTS_STANDALONE
#include "benchmarks.hpp"
#else
#include <tts/benchmarks.hpp>
#endif

#include <cmath>

double sqrt_time()
{
  auto                  x = tts::bench::random<double>(1., 100.);
  tts::bench::benchmark b("sqrt", 11, 10);
  return TTS_RUN_BENCHMARK(b, double, "std::sqrt", [](double v) { return std::sqrt(v); }, x).time;
}
