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
#include <array>

#if defined(__EMSCRIPTEN__)
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <csignal>
#else
#include <csignal>
#endif

namespace tts::_
{
  inline std::size_t remaining_tests = 0; // NOSONAR - the loop updates it before each case

  // Read once: a debugger or a sanitizer wants the signal for itself.
  inline bool crash_guard_enabled()
  {
    static bool that = !::tts::arguments()("--no-crash-guard");
    return that;
  }

  // Not async-signal-safe: each handler guards its own re-entry before calling this.
  inline void report_crash(char const* cause, void const* address) // NOSONAR - an address is void*
  {
    // Without this the Results: line reads 100% success on a run that died.
    ::tts::global_runtime.fatal();
    ::tts::global_runtime.unexpected();

    if(address)
      ::tts::output().writeln("TEST: '%s' - @@ CRASHED @@ %s at %p", current_test, cause, address);
    else ::tts::output().writeln("TEST: '%s' - @@ CRASHED @@ %s", current_test, cause);

    ::tts::output().suite_aborted();
    ::tts::output().writeln("@@ ABORTING DUE TO CRASH @@ - %d Tests not run",
                            static_cast<int>(remaining_tests));

    ::tts::report(0, 0);
    ::tts::output().finish();
  }
}

#if defined(__EMSCRIPTEN__)
namespace tts::_
{
  // No process-wide signal delivery here: nothing to arm.
  struct crash_guard
  {
  };
}
#elif defined(_WIN32)
namespace tts::_
{
  inline LONG WINAPI crash_filter(PEXCEPTION_POINTERS info)
  {
    auto        code  = info->ExceptionRecord->ExceptionCode;

    char const* cause = nullptr;
    switch(code)
    {
    case EXCEPTION_ACCESS_VIOLATION: cause = "access violation"; break;
    case EXCEPTION_STACK_OVERFLOW: cause = "stack overflow"; break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO: cause = "integer divide by zero"; break;
    case EXCEPTION_ILLEGAL_INSTRUCTION: cause = "illegal instruction"; break;
    default: return EXCEPTION_CONTINUE_SEARCH;
    }

    static bool reporting = false;
    if(reporting) exit_now();
    reporting = true;

    report_crash(cause, info->ExceptionRecord->ExceptionAddress);
    perform_abort(static_cast<int>(code));
  }

}

extern "C"
{
  [[noreturn]] inline void tts_crash_on_abort_signal(int)
  {
    static bool reporting = false;
    if(reporting) ::tts::_::exit_now();
    reporting = true;

    ::tts::_::report_crash("SIGABRT", nullptr);
    ::tts::_::perform_abort(SIGABRT);
  }
}

namespace tts::_
{
  struct crash_guard
  {
    crash_guard()
    {
      if(!armed_) return;

      // The stack overflow exception needs this reserve to be delivered at all.
      ULONG guarantee = 64u * 1024u;
      SetThreadStackGuarantee(&guarantee);

      handle_   = AddVectoredExceptionHandler(1, &crash_filter);
      previous_ = signal(SIGABRT, &tts_crash_on_abort_signal);
    }

    ~crash_guard()
    {
      if(!armed_) return;

      if(handle_) RemoveVectoredExceptionHandler(handle_);
      if(previous_ != SIG_ERR) signal(SIGABRT, previous_);
    }

    crash_guard(crash_guard const&)            = delete;
    crash_guard& operator=(crash_guard const&) = delete;

    PVOID        handle_                       = nullptr;
    void (*previous_)(int)                     = SIG_ERR;
    bool armed_                                = crash_guard_enabled();
  };
}
#else
namespace tts::_
{
  inline constexpr std::array<int, 5> crash_signals {SIGSEGV, SIGBUS, SIGFPE, SIGILL, SIGABRT};

  inline char const*                  signal_name(int sig)
  {
    switch(sig)
    {
    case SIGSEGV: return "SIGSEGV";
    case SIGBUS: return "SIGBUS";
    case SIGFPE: return "SIGFPE";
    case SIGILL: return "SIGILL";
    case SIGABRT: return "SIGABRT";
    default: return "signal";
    }
  }

}

extern "C"
{
  // sigaction hands a mutable siginfo_t, so this parameter cannot take a const.
  [[noreturn]] inline void
  tts_crash_handler(int sig, siginfo_t* info, void*) // NOSONAR - the kernel picks this signature
  {
    static sig_atomic_t volatile reporting = 0;
    if(reporting)
    {
      signal(sig, SIG_DFL);
      raise(sig);
      ::tts::_::exit_now();
    }
    reporting = 1;

    ::tts::_::report_crash(::tts::_::signal_name(sig), sig == SIGABRT ? nullptr : info->si_addr);
    ::tts::_::perform_abort(sig);
  }
}

namespace tts::_
{
  struct crash_guard
  {
    crash_guard()
    {
      if(!armed_) return;

      // A stack overflow leaves no stack for the handler to run on, hence the alternate one.
      stack_t alt  = {};
      alt.ss_sp    = stack_.data();
      alt.ss_size  = stack_.size();
      alt.ss_flags = 0;
      sigaltstack(&alt, nullptr);

      struct sigaction action = {};
      action.sa_sigaction     = &tts_crash_handler;
      action.sa_flags         = SA_SIGINFO | SA_ONSTACK;
      sigemptyset(&action.sa_mask);

      for(std::size_t i = 0; i < crash_signals.size(); ++i)
        sigaction(crash_signals[ i ], &action, &previous_[ i ]);
    }

    ~crash_guard()
    {
      if(!armed_) return;

      for(std::size_t i = 0; i < crash_signals.size(); ++i)
        sigaction(crash_signals[ i ], &previous_[ i ], nullptr);
    }

    crash_guard(crash_guard const&)            = delete;
    crash_guard& operator=(crash_guard const&) = delete;

  private:
    // SIGSTKSZ stopped being a constant in glibc 2.34, so the alternate stack has a fixed size.
    static constexpr std::size_t                       alt_stack_size = 64u * 1024u;

    static inline std::array<char, alt_stack_size>     stack_ {};
    std::array<struct sigaction, crash_signals.size()> previous_ {};
    bool                                               armed_ = crash_guard_enabled();
  };
}
#endif
