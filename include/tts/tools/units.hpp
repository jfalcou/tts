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
  // An elapsed time and a deadline, each in its own unit, neither a std::chrono type.
  using nanoseconds  = unsigned long long;
  using milliseconds = unsigned long long;

  // How many tests, failures, invalids: everything the suite tallies.
  using counter = unsigned long long;
}
