//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION empty_suite_main
#include <tts/tts.hpp>

// No TTS_CASE registered on purpose: this file's driver runs an empty suite.

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);
  empty_suite_main(argc, argv);

  bool        fails_by_default = (::tts::report(0, 0) != 0);

  char const* allow_argv[]     = {argv[ 0 ], "--allow-empty"};
  ::tts::_::current_arguments  = ::tts::options {2, allow_argv};

  bool passes_with_allow_empty = (::tts::report(0, 0) == 0);

  TTS_EXPECT(fails_by_default);
  TTS_EXPECT(passes_with_allow_empty);

  return ::tts::report(0, 0);
}
