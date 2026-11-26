#define TTS_MAIN

#include <tts/benchmarks.hpp>

double sqrt_time();

TTS_CASE("Check that a benchmark measures a function in a unit without TTS_MAIN")
{
  double const time = sqrt_time();

  TTS_GREATER(time, 0.);
  TTS_LESS(time, 1.);
};
