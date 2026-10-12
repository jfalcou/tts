//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/case.hpp>
#include <tts/engine/info.hpp>
#include <tts/engine/logger.hpp>

// What every check macro shares: where it is written, its count and its report.
namespace tts::_
{
  struct check_site
  {
    char const* file;
    int         line;
    bool        fatal;
    char const* lhs     = "";
    char const* op      = "";
    char const* rhs     = "";
    char const* negated = "";
  };

  inline bool check_passed()
  {
    ::tts::global_runtime.pass();
    return ::tts::is_detailed();
  }

  inline void check_report(check_site const& site, ::tts::text const& message)
  {
    report_pass(source_location::current(site.file, site.line).data(), message.data());
  }

  inline logger check_failed(check_site const& site, ::tts::text const& message)
  {
    ::tts::global_runtime.fail();
    if(!::tts::global_runtime.fail_status) ::tts::global_runtime.fail_status = true;

    auto location = source_location::current(site.file, site.line);
    if(site.fatal)
    {
      report_fatal(location.data(), message.data(), current_type);
      ::tts::fatal_error_status = true;
      [[maybe_unused]] logger local_tts_fail_hard {}; // throws from its destructor
    }
    else
    {
      report_fail(location.data(), message.data(), current_type);
    }

    return logger {};
  }
}

// FAILURE is TTS_FAIL or TTS_FATAL: it is pasted onto TTS_IS_FATAL_.
#define TTS_CHECK_SITE(FAILURE, LHS, OP, RHS, NEGATED)                                             \
  ::tts::_::check_site                                                                             \
  {                                                                                                \
    __builtin_FILE(), __builtin_LINE(), TTS_IS_FATAL_##FAILURE, LHS, OP, RHS, NEGATED              \
  }

#define TTS_IS_FATAL_TTS_FAIL  false
#define TTS_IS_FATAL_TTS_FATAL true
