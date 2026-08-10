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
    @brief output_sink wrapping another sink, colorizing pass/fail/fatal lines with ANSI escapes.

    Forwards every message to a target sink (tts::output_handler::default_sink() by default),
    wrapping messages that look like a failure, a fatal error, an invalid test, or a pass
    confirmation in the corresponding ANSI color. Opt-in: not every terminal or CI log renders
    ANSI escapes usefully.

    @groupheader{Example}
    @code
    tts::colorized_sink colorized;
    tts::output().sink(colorized);
    @endcode
  **/
  //====================================================================================================================
  struct colorized_sink : output_sink
  {
    explicit colorized_sink(output_sink& target = output_handler::default_sink())
        : target_(&target)
    {
    }

    void write(text const& t) override
    {
      char const* s = t.data();

      // A logical line (e.g. the final "Results: ..." summary) can span several write() calls -
      // keep the color active across all of them until the "\n" that ends the line.
      if(strcmp(s, "\n") == 0) // NOSONAR - avoids std::string
      {
        if(active_color_) target_->write(text {"\033[0m"});
        active_color_ = nullptr;
        target_->write(t);
        return;
      }

      if(!active_color_)
      {
        if(strstr(s, "** FAILURE **") || strstr(s, "@@ FATAL @@") || strstr(s, "@@ ABORTING"))
          active_color_ = "\033[31m";                                     // red
        else if(strstr(s, "EMPTY TEST CASE")) active_color_ = "\033[33m"; // yellow
        else if(strstr(s, "[PASSED]")) active_color_ = "\033[32m";        // green
        else if(strncmp(s, "Results:", 8) == 0) // NOSONAR - avoids std::string
          active_color_ = (strstr(s, "failure") || strstr(s, "invalid")) ? "\033[31m" : "\033[32m";

        if(active_color_) target_->write(text {active_color_});
      }

      target_->write(t);
    }

    void flush() override
    {
      target_->flush();
    }

  private:
    output_sink* target_;
    char const*  active_color_ = nullptr;
  };
}
