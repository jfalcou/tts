//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION dry_flag_main
#include <tts/tts.hpp>
#include <array>

TTS_CASE("First dummy test for --dry")
{
  TTS_EXPECT(1 == 1);
};

TTS_CASE("Second dummy test for --dry")
{
  TTS_EXPECT(1 == 1);
};

TTS_CASE_TPL("Templated dummy test for --dry", std::int32_t, double)
<typename T>(::tts::type<T>)
{
  TTS_EXPECT(1 == 1);
};

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);

  bool                       ok = true;

  std::array<char const*, 2> dry_argv {argv[ 0 ], "--dry"};
  ::tts::_::current_arguments           = ::tts::options {2, dry_argv.data()};

  auto                test_count_before = ::tts::global_runtime.test_count;
  tts::gathering_sink gs;
  {
    tts::scoped_sink scope(gs);
    int              rc = dry_flag_main(argc, argv);
    ok                  = ok && (rc == 0);
  }

  // --dry must not execute any test.
  ok = ok && (::tts::global_runtime.test_count == test_count_before);

  ok = ok && (strstr(gs.content().data(), "First dummy test for --dry") != nullptr);  // NOSONAR
  ok = ok && (strstr(gs.content().data(), "Second dummy test for --dry") != nullptr); // NOSONAR

  // TTS_CASE_TPL entries must list their types, without pinning down the exact spelling
  // (e.g. int32_t vs int) since that's compiler/platform dependent.
  ok =
  ok && (strstr(gs.content().data(), "Templated dummy test for --dry <") != nullptr); // NOSONAR

  return ok ? 0 : 1;
}
