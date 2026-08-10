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
  // Monotonic timestamp in nanoseconds, for measuring elapsed durations between two calls - never
  // a meaningful absolute point in time on its own. Not std::chrono: measured empirically at
  // ~0.55-0.6s (5-6x) of extra compile time per translation unit, unacceptable in a header
  // included by every user's test binary. windows.h with WIN32_LEAN_AND_MEAN/NOMINMAX measured
  // as free (no significant cost vs. an empty translation unit) with cl.exe.
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

  // Picks whichever of ns/us/ms/s keeps the displayed value in a readable range, instead of a
  // fixed unit that reads as "0.000ms" for a trivial test or "15455566.123ms" for a slow suite.
  // Thresholds sit half a displayed-digit below each power of 1000, so a value that would round
  // up to e.g. "1000.000 ms" bumps to the next unit ("1.000 s") instead of showing four digits.
  inline ::tts::text format_duration(double duration_ns)
  {
    if(duration_ns < 999.5) return ::tts::text {"%.0f ns", duration_ns};
    if(duration_ns < 999'999.5) return ::tts::text {"%.3f us", duration_ns / 1'000.0};
    if(duration_ns < 999'999'500.0) return ::tts::text {"%.3f ms", duration_ns / 1'000'000.0};
    return ::tts::text {"%.3f s", duration_ns / 1'000'000'000.0};
  }
}
