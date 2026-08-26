//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION sink_flag_main
#include <tts/tts.hpp>
#include <array>

TTS_CASE("validate_sink_name accepts empty and known values, rejects unknown ones")
{
  bool      ok  = false;
  tts::text err = tts::_::validate_sink_name(tts::text {}, ok);
  TTS_EXPECT(ok);
  TTS_EXPECT(err.is_empty());

  for(char const* name: {"colored", "tap", "diagnostics"})
  {
    ok  = false;
    err = tts::_::validate_sink_name(tts::text {name}, ok);
    TTS_EXPECT(ok);
    TTS_EXPECT(err.is_empty());
  }

  ok  = true;
  err = tts::_::validate_sink_name(tts::text {"bogus"}, ok);
  TTS_EXPECT_NOT(ok);
  TTS_EXPECT(strstr(err.data(), "bogus") != nullptr);
};

TTS_CASE("Dummy test so sink_flag_main has something to run")
{
  TTS_EXPECT(1 == 1);
};

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);

  bool ok = true;

  // --sink is a no-op for a custom driver, same as --shard: a sink pre-installed by the caller
  // stays active untouched, not swapped out for colorized_sink.
  {
    std::array<char const*, 2> sink_argv {argv[ 0 ], "--sink=colored"};
    ::tts::_::current_arguments = ::tts::options {2, sink_argv.data()};

    tts::gathering_sink target;
    tts::scoped_sink    scope(target);
    sink_flag_main(argc, argv);

    char const* content = target.content().data();
    ok                  = ok && (strstr(content, "\033[32m") == nullptr); // NOSONAR
    ok                  = ok && (strstr(content, "[PASSED]") != nullptr); // NOSONAR
  }

  return ok ? 0 : 1;
}
