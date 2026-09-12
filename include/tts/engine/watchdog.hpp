//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/abort.hpp>
#include <tts/engine/environment.hpp>
#include <tts/engine/test.hpp>

#if defined(__EMSCRIPTEN__)
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <csignal>
#include <sys/time.h>
#endif

namespace tts::_
{
  inline unsigned long long watchdog_ms = 0; // NOSONAR - the loop sets it before each case

  inline unsigned long long default_timeout_ms()
  {
    static unsigned long long that = ::tts::arguments().value<unsigned long long>("--timeout");
    return that;
  }

  inline void report_timeout()
  {
    ::tts::text line {
    "TEST: '%s' - @@ TIMEOUT @@ still running after %llu ms", current_test, watchdog_ms};

    report_abort(line.data(), "TIMEOUT");
  }
}

#if defined(__EMSCRIPTEN__)
namespace tts::_
{
  // No process-wide timer here: nothing to arm.
  struct watchdog
  {
    explicit watchdog(unsigned long long)
    {
    }
  };
}
#elif defined(_WIN32)
namespace tts::_
{
  inline VOID CALLBACK watchdog_expired(PVOID, BOOLEAN)
  {
    static bool reporting = false;
    if(reporting) exit_now();
    reporting = true;

    report_timeout();
    perform_abort(0);
  }

  struct watchdog
  {
    explicit watchdog(unsigned long long ms)
    {
      if(!ms) return;

      watchdog_ms = ms;
      CreateTimerQueueTimer(&timer_,
                            nullptr,
                            &watchdog_expired,
                            nullptr,
                            static_cast<DWORD>(ms),
                            0,
                            WT_EXECUTEINTIMERTHREAD);
    }

    ~watchdog()
    {
      // INVALID_HANDLE_VALUE waits for a callback already running, so a case finishing on the
      // deadline cannot race its own teardown.
      if(timer_) DeleteTimerQueueTimer(nullptr, timer_, INVALID_HANDLE_VALUE);
    }

    watchdog(watchdog const&)            = delete;
    watchdog& operator=(watchdog const&) = delete;

    HANDLE    timer_                     = nullptr;
  };
}
#else
extern "C"
{
  [[noreturn]] inline void tts_watchdog_expired(int)
  {
    static bool reporting = false;
    if(reporting) ::tts::_::exit_now();
    reporting = true;

    ::tts::_::report_timeout();
    ::tts::_::perform_abort(0);
  }
}

namespace tts::_
{
  struct watchdog
  {
    explicit watchdog(unsigned long long ms)
    {
      if(!ms) return;

      watchdog_ms             = ms;

      struct sigaction action = {};
      action.sa_handler       = &tts_watchdog_expired;
      action.sa_flags         = 0;
      sigemptyset(&action.sa_mask);
      sigaction(SIGALRM, &action, &previous_);

      // A thread blocked in a syscall still takes the signal, which a polling thread would not.
      itimerval deadline {};
      deadline.it_value.tv_sec  = static_cast<time_t>(ms / 1000u);
      deadline.it_value.tv_usec = static_cast<suseconds_t>((ms % 1000u) * 1000u);
      setitimer(ITIMER_REAL, &deadline, nullptr);

      armed_ = true;
    }

    ~watchdog()
    {
      if(!armed_) return;

      itimerval off {};
      setitimer(ITIMER_REAL, &off, nullptr);
      sigaction(SIGALRM, &previous_, nullptr);
    }

    watchdog(watchdog const&)            = delete;
    watchdog& operator=(watchdog const&) = delete;

  private:
    struct sigaction previous_ = {};
    bool             armed_    = false;
  };
}
#endif
