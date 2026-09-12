//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION timeout_main
#include <tts/tts.hpp>
#include "unit/driver/recording_sink.hpp"

#if !defined(__EMSCRIPTEN__)

TTS_CASE(TTS_TIMEOUT(150, "Case that never returns"))
{
  TTS_EXPECT(true);

  bool volatile never = false; // NOSONAR - no thread here, this only keeps the loop from folding
  while(!never)
  {
    // Spinning until the deadline kills us is the whole point of this case.
  }
};
#endif

int main([[maybe_unused]] int argc, [[maybe_unused]] char const** argv)
{
#if defined(__EMSCRIPTEN__)
  return 77; // no process timer here, so nothing to check
#else
  return tts::test::run_and_report(
  argc,
  argv,
  "TIMEOUT ATTRIBUTION OK",
  [](int c, char const** v) { timeout_main(c, v); },
  [](tts::test::recording_sink const& sink, int reason)
  {
    tts::text attribution {
    "'%s' - @@ TIMEOUT @@ still running after %llu ms", "Case that never returns", 150ULL};

    return reason == 0 && sink.aborted && sink.says(attribution.data()) &&
           sink.says("ABORTING DUE TO TIMEOUT") && sink.says("Results:");
  });
#endif
}
