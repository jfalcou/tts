#define TTS_MAIN

#ifdef TTS_STANDALONE
#include "benchmarks.hpp"
#else
#include <tts/benchmarks.hpp>
#endif

double sqrt_time();

TTS_CASE("Check that a benchmark measures a function in a unit without TTS_MAIN")
{
  double const time = sqrt_time();

  TTS_GREATER(time, 0.);
  TTS_LESS(time, 1.);
};
