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
    @public
    @brief output_sink rewriting failure/fatal lines as compiler-style diagnostics.

    Forwards every message to a target sink (tts::output_handler::default_sink() by default),
    rewriting lines that look like a @ref TTS_CASE failure or fatal error - which already embed a
    `[file:line]` location - into `file:line: error: message` / `file:line: fatal error: message`,
    so editors/IDEs with a GCC/Clang-style problem matcher (VSCode's `$gcc`, vim's quickfix, ...)
    can jump straight to the failing assertion. Every other message passes through unchanged.

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
      char const* s      = t.data();
      char const* marker = "** FAILURE **";
      char const* level  = "error";
      char const* found  = strstr(s, marker);

      if(!found)
      {
        marker = "@@ FATAL @@";
        level  = "fatal error";
        found  = strstr(s, marker);
      }

      // The line looks like "  [X] [file:line] : ** FAILURE ** : message" (or [@] / FATAL) -
      // find the second bracketed group, the first one being the "[X]"/"[@]" status marker.
      char const* p1 = found ? strchr(s, '[') : nullptr;
      char const* e1 = p1 ? strchr(p1, ']') : nullptr;
      char const* p2 = e1 ? strchr(e1 + 1, '[') : nullptr;
      char const* e2 = p2 ? strchr(p2, ']') : nullptr;

      if(p2 && e2)
      {
        text        location {"%.*s", static_cast<int>(e2 - p2 - 1), p2 + 1}; // NOSONAR
        char const* msg =
        found + strlen(marker) + 3; // skip "marker : " - NOSONAR, marker is always a literal
        target_->write(text {"%s: %s: %s", location.data(), level, msg});
      }
      else
      {
        target_->write(t);
      }
    }

    void flush() override
    {
      target_->flush();
    }

  private:
    output_sink* target_;
  };
}
