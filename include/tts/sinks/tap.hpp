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
    @brief output_sink gathering every message, then rendering it as TAP (Test Anything Protocol).

    Accumulates the run's output like tts::gathering_sink, then dump() reconstructs one "ok N -
    name" / "not ok N - name" line per @ref TTS_CASE from it, followed by a trailing "1..N" plan
    line. This is derived from the same human-readable text @ref tts::stdout_sink prints - there
    is no structured per-test event to draw from yet - so it only recognizes TTS's own "TEST:
    'name'" / "[PASSED]" markers and does not (currently) reflect @ref TTS_CASE_TPL's per-type
    breakdown or @ref TTS_WHEN / @ref TTS_AND_THEN sub-scenarios.

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
    void write(text const& t) override
    {
      buffer_ += t;
    }

    /// Renders everything gathered so far as TAP-formatted text.
    text render() const
    {
      text        out;
      text        pending;
      std::size_t n = 0;

      char const* p = buffer_.data();
      while(*p)
      {
        char const* nl  = strchr(p, '\n');
        std::size_t len = nl ? static_cast<std::size_t>(nl - p) : strlen(p);
        text        line {"%.*s", static_cast<int>(len), p}; // NOSONAR - avoids std::string

        if(strncmp(line.data(), "TEST: '", 7) == 0 && !strstr(line.data(), "[PASSED]"))
        {
          if(!pending.is_empty())
          {
            ++n;
            out += text {"not ok %zu - %s\n", n, pending.data()};
          }

          char const* start = line.data() + 7;
          char const* end   = strchr(start, '\'');
          pending           = end ? text {"%.*s", static_cast<int>(end - start), start} : text {};
        }
        else if(!pending.is_empty() && strstr(line.data(), "[PASSED]"))
        {
          ++n;
          out     += text {"ok %zu - %s\n", n, pending.data()};
          pending  = text {};
        }

        p = nl ? nl + 1 : p + len;
      }

      if(!pending.is_empty())
      {
        ++n;
        out += text {"not ok %zu - %s\n", n, pending.data()};
      }

      return text {"1..%zu\n", n} + out;
    }

    /// Forwards the TAP-rendered report to target, then clears the gathered buffer.
    void dump(output_sink& target)
    {
      target.write(render());
      clear();
    }

    /// Streams the TAP-rendered report to `stdout`, then clears the gathered buffer.
    void dump()
    {
      stdout_sink target;
      dump(target);
    }

    /// Discards everything gathered so far.
    void clear()
    {
      buffer_ = text {};
    }

  private:
    text buffer_;
  };
}
