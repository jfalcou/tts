//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION crash_main
#include <tts/tts.hpp>
#include "unit/driver/recording_sink.hpp"

#if !defined(__EMSCRIPTEN__)
#include <csignal>

TTS_CASE("Case crashing halfway through")
{
  TTS_EXPECT(true);

  // A raised signal keeps memcheck and the sanitizers silent: no invalid access happens.
  std::raise(SIGSEGV);
};
#endif

int main([[maybe_unused]] int argc, [[maybe_unused]] char const** argv)
{
#if defined(__EMSCRIPTEN__)
  return 77; // no crash guard here, so nothing to check
#else
  return tts::test::run_and_report(
  argc,
  argv,
  "CRASH ATTRIBUTION OK",
  [](int c, char const** v) { crash_main(c, v); },
  [](tts::test::recording_sink const& sink, int reason)
  {
    tts::text attribution {"'%s' - @@ CRASHED @@ %s", "Case crashing halfway through", "SIGSEGV"};

    return reason == SIGSEGV && sink.aborted && sink.says(attribution.data()) &&
           sink.says("Results:");
  });
#endif
}
