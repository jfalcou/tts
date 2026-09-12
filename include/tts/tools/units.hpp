//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace tts
{
  // Every duration in the library, deadlines included. Not a std::chrono type.
  using nanoseconds = unsigned long long;

  // How many tests, failures, invalids: everything the suite tallies.
  using counter = unsigned long long;
}
