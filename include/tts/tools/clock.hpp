//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <profileapi.h>
#else
#include <time.h>
#endif

#include <tts/tools/text.hpp>

namespace tts::_
{
  // Monotonic elapsed-time only, not std::chrono - see doc/compile_time.hpp.
  inline unsigned long long now_ns()
  {
#if defined(_WIN32)
    LARGE_INTEGER freq;
    LARGE_INTEGER count;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&count);
    return static_cast<unsigned long long>(static_cast<double>(count.QuadPart) * 1e9 /
                                           static_cast<double>(freq.QuadPart));
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // NOSONAR - avoiding <chrono>, see comment above
    return static_cast<unsigned long long>(ts.tv_sec) * 1'000'000'000ULL +
           static_cast<unsigned long long>(ts.tv_nsec);
#endif
  }

  // Thresholds sit just below each power of 1000 so rounding can't show e.g. "1000.000 ms".
  inline ::tts::text format_duration(double duration_ns)
  {
    if(duration_ns < 999.5) return ::tts::text {"%.0f ns", duration_ns};
    if(duration_ns < 999'999.5) return ::tts::text {"%.3f us", duration_ns / 1'000.0};
    if(duration_ns < 999'999'500.0) return ::tts::text {"%.3f ms", duration_ns / 1'000'000.0};
    return ::tts::text {"%.3f s", duration_ns / 1'000'000'000.0};
  }
}
