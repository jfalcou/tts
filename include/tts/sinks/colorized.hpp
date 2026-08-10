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
    @brief output_sink wrapping another sink, colorizing pass/fail/fatal lines with ANSI escapes.

    Wraps a target sink (tts::output_handler::default_sink() by default). Opt-in: not every
    terminal or CI log renders ANSI escapes usefully. See @ref output-sinks for a colored example
    of a full run.

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

      // "\n" ends the current line's color only, not active_color_, so a hook's color can span
      // several write() calls until the next hook changes it.
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

      // suite_metric() colors only its own segment - fall back once it's written.
      if(revert_to_)
      {
        active_color_  = revert_to_;
        color_applied_ = false;
        revert_to_     = nullptr;
      }
    }

    // Each hook below sets or clears active_color_ decisively, even under -q.

    void test_started([[maybe_unused]] text const& name) override
    {
      set_color(nullptr);
    }

    void assertion_failed([[maybe_unused]] text const& location,
                          [[maybe_unused]] text const& message,
                          [[maybe_unused]] bool        fatal) override
    {
      set_color("\033[31m"); // red - NOSONAR, \o{} is C++23-only
    }

    void test_finished([[maybe_unused]] text const&        name,
                       bool                                passed,
                       bool                                invalid,
                       [[maybe_unused]] unsigned long long duration_ns) override
    {
      if(invalid) set_color("\033[33m");     // yellow - NOSONAR, \o{} is C++23-only
      else if(passed) set_color("\033[32m"); // green - NOSONAR, \o{} is C++23-only
      else set_color(nullptr); // plain failure - assertion_failed() already colored its line
    }

    void suite_finished([[maybe_unused]] unsigned long long fail_count,
                        [[maybe_unused]] unsigned long long invalid_count) override
    {
      set_color("\033[1m"); // bold, neutral - NOSONAR, \o{} is C++23-only
    }

    void suite_metric(outcome                             kind,
                      [[maybe_unused]] unsigned long long count,
                      [[maybe_unused]] unsigned long long total) override
    {
      using enum outcome;
      revert_to_ = active_color_; // fall back to this once the segment below is written
      switch(kind)
      {
      case success: set_color("\033[1;32m"); break; // bold green - NOSONAR
      case failure: set_color("\033[1;31m"); break; // bold red - NOSONAR
      case invalid: set_color("\033[1;33m"); break; // bold yellow - NOSONAR
      }
    }

    void suite_aborted() override
    {
      set_color("\033[31m"); // red - NOSONAR, \o{} is C++23-only
    }

    void flush() override
    {
      target_->flush();
    }

  private:
    void set_color(char const* color)
    {
      active_color_  = color;
      color_applied_ = false;
    }

    output_sink* target_;
    char const*  active_color_  = nullptr;
    bool         color_applied_ = false;
    char const*  revert_to_     = nullptr;
  };
}
