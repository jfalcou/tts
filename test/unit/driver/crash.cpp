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

#if !defined(__EMSCRIPTEN__)
#include <csignal>

TTS_CASE("Case crashing halfway through")
{
  TTS_EXPECT(true);

  // A raised signal keeps memcheck and the sanitizers silent: no invalid access happens.
  std::raise(SIGSEGV);
};

namespace
{
  struct recording_sink : tts::output_sink
  {
    void write(tts::text const& t) override
    {
      seen += t;
    }

    void suite_aborted() override
    {
      aborted = true;
    }

    bool says(char const* what) const
    {
      return std::strstr(seen.data(), what) != nullptr; // NOSONAR - contains() is C++23
    }

    tts::text seen    = {};
    bool      aborted = false;
  };
}
#endif

int main([[maybe_unused]] int argc, [[maybe_unused]] char const** argv)
{
#if defined(__EMSCRIPTEN__)
  return 77; // no crash guard here, so nothing to check
#else
  ::tts::initialize(argc, argv);

  // main is still on the stack when the handler runs, so the capture stays valid.
  recording_sink sink;

  tts::set_abort_handler(
  [ &sink ](int reason)
  {
    bool ok = reason == SIGSEGV && sink.aborted && sink.says("@@ CRASHED @@") &&
              sink.says("SIGSEGV") && sink.says("Case crashing halfway through") &&
              sink.says("Results:");

    // The run exits 1 whatever happens, so ctest reads this line instead of the code.
    if(ok) std::puts("CRASH ATTRIBUTION OK");
  });

  tts::scoped_sink scope(sink);
  crash_main(argc, argv);

  return 1; // the case above must not let us reach this
#endif
}
