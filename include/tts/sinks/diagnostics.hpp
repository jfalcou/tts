//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/output.hpp>

namespace tts
{
  //====================================================================================================================
  /**
    @ingroup tools-sinks
    @public
    @brief output_sink adding compiler-style diagnostics for every failing/fatal assertion.

    Forwards every message to a target sink (tts::output_handler::default_sink() by default)
    unchanged, and additionally prints one `path:line: error: message` /
    `path:line: fatal error: message` line per failing/fatal assertion - built from
    @ref output_sink's structured `assertion_failed()` hook, not by parsing text - so editors/IDEs
    with a GCC/Clang-style problem matcher (VS Code's C/C++ extension provides `$gcc`, vim has
    quickfix, ...) can jump straight to it. Prints even under `-q`, when the raw failure line it
    complements is itself suppressed.

    @groupheader{Example}
    @code
    tts::diagnostics_sink diagnostics;
    tts::output().sink(diagnostics);
    @endcode
  **/
  //====================================================================================================================
  struct diagnostics_sink : output_sink
  {
    explicit diagnostics_sink(output_sink& target = output_handler::default_sink())
        : target_(&target)
    {
    }

    void write(text const& t) override
    {
      target_->write(t);
    }

    void assertion_failed(text const& location, text const& message, bool fatal) override
    {
      // location is already "[file:line]" (see tts::_::source_location) - strip the brackets.
      char const* loc = location.data();
      std::size_t len = strlen(loc); // NOSONAR - loc always comes from a well-formed tts::text
      target_->write(text {"%.*s: %s: %s\n",
                           static_cast<int>(len - 2),
                           loc + 1,
                           fatal ? "fatal error" : "error",
                           message.data()});
    }

    void flush() override
    {
      target_->flush();
    }

  private:
    output_sink* target_;
  };
}
