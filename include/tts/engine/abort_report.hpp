//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/deps.hpp>
#include <tts/engine/environment.hpp>

namespace tts::_
{
  inline std::size_t remaining_tests = 0; // NOSONAR - the loop updates it before each case

  inline void        report_abort(char const* headline, char const* reason)
  {
    // Without fatal() the Results: line reads 100% success on a run that died.
    ::tts::global_runtime.fatal();
    ::tts::global_runtime.unexpected();

    ::tts::output().writeln(headline);
    ::tts::output().suite_aborted();
    ::tts::output().writeln(
    "@@ ABORTING DUE TO %s @@ - %d Tests not run", reason, static_cast<int>(remaining_tests));

    ::tts::report(0, 0);
    ::tts::output().finish();
  }
}
