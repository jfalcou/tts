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
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<unsigned long long>(ts.tv_sec) * 1'000'000'000ULL +
           static_cast<unsigned long long>(ts.tv_nsec);
#endif
  }
}
