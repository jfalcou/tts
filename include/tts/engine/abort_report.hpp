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
#include <tts/engine/deps.hpp>
#include <tts/engine/environment.hpp>
#include <tts/engine/test.hpp>

namespace tts::_
{
  inline std::size_t remaining_tests = 0; // NOSONAR - the loop updates it before each case

  inline void        report_abort(char const* headline, char const* reason)
  {
    using enum ::tts::expected_outcome;

    // Without fatal() the Results: line reads 100% success on a run that died.
    ::tts::global_runtime.fatal();

    // A case tagged xfail or may_fail asked for this end, so the run is not surprised by it.
    if(current_tag != xfail && current_tag != may_fail) ::tts::global_runtime.unexpected();

    ::tts::output().writeln(headline);
    ::tts::output().suite_aborted();
    ::tts::output().writeln(
    "@@ ABORTING DUE TO %s @@ - %d Tests not run", reason, static_cast<int>(remaining_tests));

    abort_exit_code = ::tts::report(0, 0);
    ::tts::output().finish();
  }
}
