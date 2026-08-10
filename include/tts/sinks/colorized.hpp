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
    wrapping a failure/fatal/invalid/pass line, or the final `Results: ...` summary, in the
    corresponding ANSI color - decided from @ref output_sink's structured hooks, not by parsing
    text. Opt-in: not every terminal or CI log renders ANSI escapes usefully.

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
      // "\n" only closes the CURRENT line's coloring (color_applied_), not active_color_ itself,
      // so a single hook can color several consecutive lines (e.g. the separator right before
      // Results:) the same way, until the next hook decides otherwise.
      if(strcmp(s, "\n") == 0) // NOSONAR - avoids std::string
      {
        if(color_applied_) target_->write(text {"\033[0m"}); // NOSONAR - \o{} is C++23-only
        color_applied_ = false;
        target_->write(t);
        return;
      }

      if(active_color_ && !color_applied_)
      {
        target_->write(text {active_color_});
        color_applied_ = true;
      }

      target_->write(t);
    }

    // Each hook below decisively sets or clears active_color_, rather than only setting it - a
    // hook whose corresponding text is suppressed by -q must not leave a stale color for some
    // later, unrelated write() to inherit.

    void test_started([[maybe_unused]] text const& name) override
    {
      active_color_  = nullptr;
      color_applied_ = false;
    }

    void assertion_failed([[maybe_unused]] text const& location,
                          [[maybe_unused]] text const& message,
                          [[maybe_unused]] bool        fatal) override
    {
      active_color_  = "\033[31m"; // red - NOSONAR, \o{} is C++23-only
      color_applied_ = false;
    }

    void test_finished([[maybe_unused]] text const& name, bool passed, bool invalid) override
    {
      if(invalid) active_color_ = "\033[33m";     // yellow - NOSONAR, \o{} is C++23-only
      else if(passed) active_color_ = "\033[32m"; // green - NOSONAR, \o{} is C++23-only
      else active_color_ = nullptr; // plain failure - assertion_failed() already colored its line
      color_applied_ = false;
    }

    void suite_finished(unsigned long long fail_count, unsigned long long invalid_count) override
    {
      active_color_  = (fail_count || invalid_count) ? "\033[31m" : "\033[32m"; // NOSONAR
      color_applied_ = false;
    }

    void suite_aborted() override
    {
      active_color_  = "\033[31m"; // red - NOSONAR, \o{} is C++23-only
      color_applied_ = false;
    }

    void flush() override
    {
      target_->flush();
    }

  private:
    output_sink* target_;
    char const*  active_color_  = nullptr;
    bool         color_applied_ = false;
  };
}
