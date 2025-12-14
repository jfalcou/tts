//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace tts::_
{
  inline constexpr auto usage_text =
R"(
Flags:
  -h, --help        Display this help message
  -x, --hex         Print the floating results in hexfloat mode
  -s, --scientific  Print the floating results in scientific mode
  -v, --verbose     Display tests results regardless of their status.

Parameters:
  --precision=arg   Set the precision for displaying floating pint values
  --seed=arg        Set the PRNG seeds (default is time-based)

Range specifics Parameters:
  --block=arg       Set size of range checks samples (min. 32)
  --loop=arg        Repeat each range checks arg times
  --ulpmax=arg      Set global failure ulp threshold for range tests (default is 2.0)
  --valmax=arg      Set maximal value for range tests (default is code)
  --valmin=arg      Set minimal value for range tests (default is code)
)";

  //====================================================================================================================
  // Prints the CLI option of any TTS test executable
  //====================================================================================================================
  inline int usage(const char* name)
  {
    printf("TTS Unit Tests Driver\nUsage: %s [OPTION...]", name);
    puts(usage_text);
    return 0;
  }
}
