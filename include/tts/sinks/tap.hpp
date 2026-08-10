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
    @brief output_sink rendering the run as TAP (Test Anything Protocol).

    Built from @ref output_sink's structured `test_finished` hook rather than by parsing the
    human-readable text @ref tts::stdout_sink prints, so it reports one "ok N - name" / "not ok
    N - name" line per @ref TTS_CASE regardless of `-v`/`-q`, followed by a trailing "1..N" plan
    line once dump()ed. It does not (currently) reflect @ref TTS_CASE_TPL's per-type breakdown or
    @ref TTS_WHEN / @ref TTS_AND_THEN sub-scenarios, since those don't have their own hook - only
    the enclosing @ref TTS_CASE does.

    @groupheader{Example}
    @code
    tts::tap_sink tap;
    tts::output().sink(tap);

    // ... run the test suite ...

    tap.dump(); // stream the TAP-formatted report to stdout
    @endcode
  **/
  //====================================================================================================================
  struct tap_sink : output_sink
  {
    void write(text const&) override
    {
      // Intentionally empty: TAP output is built entirely from test_finished() below.
    }

    void test_finished(text const& name, bool passed, [[maybe_unused]] bool invalid) override
    {
      ++count_;
      body_ += passed ? text {"ok %zu - %s\n", count_, name.data()}
                      : text {"not ok %zu - %s\n", count_, name.data()};
    }

    /// Renders everything gathered so far as TAP-formatted text.
    text render() const
    {
      return text {"1..%zu\n", count_} + body_;
    }

    /// Forwards the TAP-rendered report to target, then clears the gathered results.
    void dump(output_sink& target)
    {
      target.write(render());
      clear();
    }

    /// Streams the TAP-rendered report to `stdout`, then clears the gathered results.
    void dump()
    {
      stdout_sink target;
      dump(target);
    }

    /// Discards everything gathered so far.
    void clear()
    {
      body_  = text {};
      count_ = 0;
    }

  private:
    text        body_;
    std::size_t count_ = 0;
  };
}
