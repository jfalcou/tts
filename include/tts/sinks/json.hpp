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

namespace tts::_
{
  // Escapes t for embedding in a JSON string literal - test names and failure messages are
  // arbitrary user text, so this is the only thing standing between them and invalid JSON.
  inline ::tts::text json_escape(::tts::text const& t)
  {
    ::tts::text out;
    for(char c: t)
    {
      switch(c)
      {
      case '"': out += "\\\""; break;
      case '\\': out += "\\\\"; break;
      case '\n': out += "\\n"; break;
      case '\r': out += "\\r"; break;
      case '\t': out += "\\t"; break;
      default:
        if(static_cast<unsigned char>(c) < 0x20)
          out += ::tts::text {"\\u%04x", static_cast<unsigned>(static_cast<unsigned char>(c))};
        else out += ::tts::text {"%c", c};
        break;
      }
    }
    return out;
  }
}

namespace tts
{
  //====================================================================================================================
  /**
    @ingroup tools-sinks
    @public
    @brief output_sink rendering the run as a single structured JSON document.

    Built from @ref output_sink's structured hooks rather than by parsing the human-readable text
    @ref tts::stdout_sink prints, so it stays correct regardless of `-v`/`-q`. Every @ref TTS_CASE
    becomes one entry in a `"tests"` array (name, status, duration in nanoseconds, and any
    failing/fatal assertions gathered while it ran), summarized by a `"summary"` object counting
    passed/failed/invalid cases - counted from `test_finished()` directly, not from the suite's
    raw assertion totals, so `tests.length` always matches `summary.total`. Rendered by `dump()`,
    or automatically via `finish()`, to the target given at construction
    (`tts::output_handler::default_sink()` by default). It does not (currently) reflect
    @ref TTS_CASE_TPL's per-type breakdown or @ref TTS_WHEN / @ref TTS_AND_THEN sub-scenarios,
    since those don't have their own hook - only the enclosing @ref TTS_CASE does.

    @groupheader{Example}
    @code
    tts::json_sink json;
    tts::output().sink(json);

    // ... run the test suite ...

    json.dump(); // stream the JSON report to stdout
    @endcode
  **/
  //====================================================================================================================
  struct json_sink : output_sink
  {
    explicit json_sink(output_sink& target = output_handler::default_sink())
        : target_(&target)
    {
    }

    void write(text const&) override
    {
      // Intentionally empty: the JSON report is built entirely from the structured hooks below.
    }

    void assertion_failed(text const& location, text const& message, bool fatal) override
    {
      // location is "[file:line]" (see tts::_::source_location) - strip the brackets, then split
      // file from line so consumers get a real integer instead of a string to parse themselves.
      char const* loc      = location.data();
      std::size_t len      = strlen(loc); // NOSONAR - loc always comes from a well-formed tts::text
      text        stripped = text {"%.*s", static_cast<int>(len - 2), loc + 1};

      char const* colon    = strrchr(stripped.data(), ':'); // NOSONAR - stripped always has one
      text        file =
      colon ? text {"%.*s", static_cast<int>(colon - stripped.data()), stripped.data()} : stripped;
      int line = 0;
      if(colon) sscanf(colon + 1, "%d", &line); // NOSONAR - colon+1 is always digits here

      if(!current_failures_.is_empty()) current_failures_ += ",";
      current_failures_ += text {"{\"location\":{\"file\":\"%s\",\"line\":%d},\"message\":\"%s\","
                                 "\"fatal\":%s}",
                                 _::json_escape(file).data(),
                                 line,
                                 _::json_escape(message).data(),
                                 fatal ? "true" : "false"};
    }

    void test_finished(text const&        name,
                       bool               passed,
                       bool               invalid,
                       unsigned long long duration_ns) override
    {
      char const* status = invalid ? "invalid" : passed ? "passed" : "failed";
      if(invalid) ++invalid_count_;
      else if(passed) ++passed_count_;
      else ++failed_count_;
      total_duration_ns_ += duration_ns;

      if(!body_.is_empty()) body_ += ",";
      body_ += text {"{\"name\":\"%s\",\"status\":\"%s\",\"duration_ns\":%llu,\"failures\":[%s]}",
                     _::json_escape(name).data(),
                     status,
                     duration_ns,
                     current_failures_.data()};

      current_failures_ = text {};
    }

    /// Renders everything gathered so far as a single JSON document.
    text render() const
    {
      unsigned long long total = passed_count_ + failed_count_ + invalid_count_;
      return text {"{\"tests\":[%s],\"summary\":{\"total\":%llu,\"passed\":%llu,\"failed\":%llu,"
                   "\"invalid\":%llu,\"duration_ns\":%llu}}",
                   body_.data(),
                   total,
                   passed_count_,
                   failed_count_,
                   invalid_count_,
                   total_duration_ns_};
    }

    /// Forwards the JSON-rendered report to target, then clears the gathered results.
    void dump(output_sink& target)
    {
      target.write(render());
      clear();
    }

    /// Streams the JSON-rendered report to `stdout`, then clears the gathered results.
    void dump()
    {
      stdout_sink target;
      dump(target);
    }

    /// Discards everything gathered so far.
    void clear()
    {
      body_              = text {};
      current_failures_  = text {};
      passed_count_      = 0;
      failed_count_      = 0;
      invalid_count_     = 0;
      total_duration_ns_ = 0;
    }

    /// Dumps the JSON-rendered report to the target given at construction.
    void finish() override
    {
      dump(*target_);
    }

  private:
    output_sink*       target_;
    text               body_;
    text               current_failures_;
    unsigned long long passed_count_      = 0;
    unsigned long long failed_count_      = 0;
    unsigned long long invalid_count_     = 0;
    unsigned long long total_duration_ns_ = 0;
  };
}
