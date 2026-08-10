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
  // Escapes t for embedding in XML text/attribute content - test names and failure messages are
  // arbitrary user text, so this is the only thing standing between them and invalid XML.
  inline ::tts::text xml_escape(::tts::text const& t)
  {
    ::tts::text out;
    for(char c: t)
    {
      switch(c)
      {
      case '&': out += "&amp;"; break;
      case '<': out += "&lt;"; break;
      case '>': out += "&gt;"; break;
      case '"': out += "&quot;"; break;
      case '\'': out += "&apos;"; break;
      default:
        // XML 1.0 forbids raw control characters outright (no valid escape for them) except
        // tab/newline/CR - drop the rest instead of producing a document no parser can read.
        if(static_cast<unsigned char>(c) >= 0x20 || c == '\t' || c == '\n' || c == '\r')
          out += ::tts::text {"%c", c};
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
    @brief output_sink rendering the run as JUnit XML, the de facto standard consumed by Jenkins,
    GitLab CI, CircleCI, Azure DevOps, Bitbucket Pipelines and most CI dashboards.

    Built from @ref output_sink's structured hooks rather than by parsing the human-readable text
    @ref tts::stdout_sink prints, so it stays correct regardless of `-v`/`-q`. Every @ref TTS_CASE
    becomes one `<testcase>` (name, classname, elapsed time in seconds) inside a single
    `<testsuite>`, with a `<skipped/>` child for an invalid case or a `<failure>` child gathering
    every failing assertion's message for a failed one. Rendered by `dump()`, or automatically via
    `finish()`, to the target given at construction (`tts::output_handler::default_sink()` by
    default). It does not (currently) reflect @ref TTS_CASE_TPL's per-type breakdown or
    @ref TTS_WHEN / @ref TTS_AND_THEN sub-scenarios, since those don't have their own hook - only
    the enclosing @ref TTS_CASE does.

    @groupheader{Example}
    @code
    tts::junit_sink junit;
    tts::output().sink(junit);

    // ... run the test suite ...

    junit.dump(); // stream the JUnit XML report to stdout
    @endcode
  **/
  //====================================================================================================================
  struct junit_sink : output_sink
  {
    explicit junit_sink(output_sink& target = output_handler::default_sink())
        : target_(&target)
    {
    }

    void write(text const&) override
    {
      // Intentionally empty: the JUnit report is built entirely from the structured hooks below.
    }

    void assertion_failed(text const&           location,
                          text const&           message,
                          [[maybe_unused]] bool fatal) override
    {
      // location is "[file:line]" (see tts::_::source_location) - strip the brackets to match
      // tts::diagnostics_sink's own "file:line: message" convention.
      char const* loc = location.data();
      std::size_t len = strlen(loc); // NOSONAR - loc always comes from a well-formed tts::text

      if(!current_failures_.is_empty()) current_failures_ += "&#10;"; // XML newline entity
      current_failures_ +=
      text {"%.*s: %s", static_cast<int>(len - 2), loc + 1, _::xml_escape(message).data()};

      if(first_failure_.is_empty()) first_failure_ = _::xml_escape(message);
    }

    void test_finished(text const&        name,
                       bool               passed,
                       bool               invalid,
                       unsigned long long duration_ns) override
    {
      if(invalid) ++invalid_count_;
      else if(passed) ++passed_count_;
      else ++failed_count_;
      total_duration_ns_ += duration_ns;

      text escaped_name   = _::xml_escape(name);
      text seconds        = text {"%.6f", static_cast<double>(duration_ns) / 1'000'000'000.0};

      if(invalid)
      {
        body_ += text {R"(    <testcase name="%s" classname="%s" time="%s"><skipped/></testcase>)"
                       "\n",
                       escaped_name.data(),
                       escaped_name.data(),
                       seconds.data()};
      }
      else if(!passed)
      {
        body_ += text {R"(    <testcase name="%s" classname="%s" time="%s"><failure )"
                       R"(message="%s">%s</failure></testcase>)"
                       "\n",
                       escaped_name.data(),
                       escaped_name.data(),
                       seconds.data(),
                       first_failure_.data(),
                       current_failures_.data()};
      }
      else
      {
        body_ += text {R"(    <testcase name="%s" classname="%s" time="%s"/>)"
                       "\n",
                       escaped_name.data(),
                       escaped_name.data(),
                       seconds.data()};
      }

      current_failures_ = text {};
      first_failure_    = text {};
    }

    /// Renders everything gathered so far as a single JUnit XML document.
    text render() const
    {
      unsigned long long total   = passed_count_ + failed_count_ + invalid_count_;
      double             seconds = static_cast<double>(total_duration_ns_) / 1'000'000'000.0;

      return text {"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   R"(<testsuites><testsuite name="TTS" tests="%llu" failures="%llu" errors="0")"
                   R"( skipped="%llu" time="%.6f">)"
                   "\n%s  </testsuite></testsuites>\n",
                   total,
                   failed_count_,
                   invalid_count_,
                   seconds,
                   body_.data()};
    }

    /// Forwards the JUnit-rendered report to target, then clears the gathered results.
    void dump(output_sink& target)
    {
      target.write(render());
      clear();
    }

    /// Streams the JUnit-rendered report to `stdout`, then clears the gathered results.
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
      first_failure_     = text {};
      passed_count_      = 0;
      failed_count_      = 0;
      invalid_count_     = 0;
      total_duration_ns_ = 0;
    }

    /// Dumps the JUnit-rendered report to the target given at construction.
    void finish() override
    {
      dump(*target_);
    }

  private:
    output_sink*       target_;
    text               body_;
    text               current_failures_;
    text               first_failure_;
    unsigned long long passed_count_      = 0;
    unsigned long long failed_count_      = 0;
    unsigned long long invalid_count_     = 0;
    unsigned long long total_duration_ns_ = 0;
  };
}
