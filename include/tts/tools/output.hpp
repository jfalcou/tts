//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/text.hpp>

namespace tts::_
{
  //====================================================================================================================
  // Current test suite verbosity settings, set from the -v/--verbose and -q/--quiet CLI flags.
  // This is the single source of truth for what should be reported and is kept independent from
  // how a message actually gets emitted, which is the job of tts::output_handler below.
  //====================================================================================================================
  struct verbosity
  {
    bool verbose = false;
    bool quiet   = false;
  };

  inline verbosity current_verbosity = {}; // NOSONAR

  //====================================================================================================================
  // Sole writers for current_verbosity: every read goes through is_verbose()/is_quiet()/
  // is_detailed(), so every write is funneled through these instead of poking the fields directly.
  //====================================================================================================================
  inline void set_verbose(bool verbose)
  {
    current_verbosity.verbose = verbose;
  }

  inline void set_quiet(bool quiet)
  {
    current_verbosity.quiet = quiet;
  }

  //====================================================================================================================
  // RAII guard saving current_verbosity on construction and restoring it on destruction. Meant for
  // tests that need to exercise a specific verbose/quiet combination without leaking that state
  // into the rest of the running test suite, even if the test exits early.
  //====================================================================================================================
  struct scoped_verbosity
  {
    scoped_verbosity() = default;

    ~scoped_verbosity()
    {
      current_verbosity = saved;
    }

    scoped_verbosity(scoped_verbosity const&)            = delete;
    scoped_verbosity& operator=(scoped_verbosity const&) = delete;
    scoped_verbosity(scoped_verbosity&&)                 = delete;
    scoped_verbosity& operator=(scoped_verbosity&&)      = delete;

    verbosity         saved                              = current_verbosity;
  };
}

namespace tts
{
  //====================================================================================================================
  /**
    @defgroup tools-output Output Utilities
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @public
    @brief Customization point for where TTS output actually goes.

    All non-usage related output produced while running a test suite - tests progress, pass/fail/
    fatal reports, additional logging information and the final results report - is written
    through the tts::output_handler currently in use, which itself forwards every message to an
    output_sink. Deriving from output_sink and overriding write() lets power users redirect that
    output wherever they need to: a file, a GUI widget, a socket, another logging system...

    @groupheader{Example}
    @snippet doc/custom_output_sink.cpp snippet

    @see tts::output_handler
    @see tts::stdout_sink
    @see tts::gathering_sink
  **/
  //====================================================================================================================
  struct output_sink
  {
    /// Writes t to this sink's destination.
    virtual void write(text const& t) = 0;

    /// Flushes this sink's destination, if that means anything for it. No-op by default.
    virtual void flush()
    {
      // Intentionally empty: most sinks (e.g. gathering_sink) have nothing meaningful to flush.
    }

    /// Called when a @ref TTS_CASE / @ref TTS_CASE_TPL / @ref TTS_CASE_WITH is about to run.
    /// No-op by default - opt-in for sinks that want structured per-test events instead of (or
    /// in addition to) parsing the formatted text stream.
    virtual void test_started([[maybe_unused]] text const& name)
    {
      // Intentionally empty: most sinks only care about the formatted text stream.
    }

    /// Called once per failing/fatal assertion, always - even under -q, where the corresponding
    /// text (if any) is suppressed instead of being written first. No-op by default. Not called
    /// for passing assertions - test_finished()'s `passed` already covers that case, and most
    /// consumers of this hook don't want one event per assertion.
    virtual void assertion_failed([[maybe_unused]] text const& location,
                                  [[maybe_unused]] text const& message,
                                  [[maybe_unused]] bool        fatal)
    {
      // Intentionally empty: most sinks only care about the formatted text stream.
    }

    /// Called once a @ref TTS_CASE / @ref TTS_CASE_TPL / @ref TTS_CASE_WITH has finished
    /// running, with its outcome. No-op by default.
    virtual void test_finished([[maybe_unused]] text const& name,
                               [[maybe_unused]] bool        passed,
                               [[maybe_unused]] bool        invalid)
    {
      // Intentionally empty: most sinks only care about the formatted text stream.
    }

    virtual ~output_sink() = default;
  };

  //====================================================================================================================
  /**
    @public
    @brief Default output_sink, streaming every message straight to `stdout`.

    This is the sink TTS uses unless a power user installs another one, matching its historical
    behavior of streaming output as it is produced.
  **/
  //====================================================================================================================
  struct stdout_sink : output_sink
  {
    void write(text const& t) override
    {
      fputs(t.data(), stdout);
    }

    void flush() override
    {
      fflush(stdout);
    }
  };

  //====================================================================================================================
  /**
    @public
    @brief output_sink gathering every message into a single retrievable buffer.

    Instead of streaming messages as they are written, gathering_sink accumulates them into a
    single tts::text. That buffer can be inspected at any time via content(), streamed out at once
    via dump(), or discarded via clear().

    @groupheader{Example}
    @code
    tts::gathering_sink report;
    tts::output().sink(report);

    // ... run the test suite ...

    report.dump();                        // stream everything gathered so far to stdout
    // or
    report.dump(some_other_sink);         // forward it to another output_sink instead
    // or
    do_something_with(report.content());  // retrieve it to do something else entirely
    @endcode
  **/
  //====================================================================================================================
  struct gathering_sink : output_sink
  {
    void write(text const& t) override
    {
      buffer_ += t;
    }

    /// Retrieves everything gathered so far.
    text const& content() const
    {
      return buffer_;
    }

    /// Forwards everything gathered so far to target, then clears the buffer.
    void dump(output_sink& target)
    {
      target.write(buffer_);
      clear();
    }

    /// Streams everything gathered so far to `stdout`, then clears the buffer.
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

  //====================================================================================================================
  /**
    @public
    @brief Centralizes all TTS runtime output and dispatches it to the current output_sink.

    tts::output_handler is the class every piece of non-usage related TTS output goes through. It
    exposes formatted writing at the granularity actually needed by the engine - a fragment, a
    full line, raw text or a printf-style message - and forwards the result to whichever
    output_sink is currently installed (tts::stdout_sink by default).

    @see tts::output
    @see tts::output_sink
  **/
  //====================================================================================================================
  class output_handler
  {
  public:
    /// Constructs an output_handler using s as its initial output_sink.
    explicit output_handler(output_sink& s = default_sink())
        : sink_(&s)
    {
    }

    /// Writes a pre-built tts::text as-is.
    void write(text const& t)
    {
      sink_->write(t);
    }

    /// Writes a raw C-string as-is.
    void write(char const* s)
    {
      sink_->write(text(s));
    }

    /// Writes a printf-style formatted message.
    template<typename... Args> void write(char const* format, Args const&... args)
    {
      sink_->write(text(format, args...));
    }

    /// Writes a pre-built tts::text followed by a newline.
    void writeln(text const& t = text {})
    {
      sink_->write(t);
      sink_->write(text("\n"));
    }

    /// Writes a raw C-string followed by a newline.
    void writeln(char const* s)
    {
      writeln(text(s));
    }

    /// Writes a printf-style formatted message followed by a newline.
    template<typename... Args> void writeln(char const* format, Args const&... args)
    {
      writeln(text(format, args...));
    }

    /// Flushes the current output_sink, if that means anything for it.
    void flush()
    {
      sink_->flush();
    }

    /// Notifies the current output_sink that a test is about to run.
    void test_started(text const& name)
    {
      sink_->test_started(name);
    }

    /// Notifies the current output_sink that an assertion failed.
    void assertion_failed(text const& location, text const& message, bool fatal)
    {
      sink_->assertion_failed(location, message, fatal);
    }

    /// Notifies the current output_sink that a test has finished running.
    void test_finished(text const& name, bool passed, bool invalid)
    {
      sink_->test_finished(name, passed, invalid);
    }

    /// Installs s as the output_sink every subsequent write goes to.
    void sink(output_sink& s)
    {
      sink_ = &s;
    }

    /// Retrieves the output_sink currently in use.
    output_sink& sink() const
    {
      return *sink_;
    }

    /// The output_sink TTS uses unless a power user installs another one.
    static stdout_sink& default_sink()
    {
      static stdout_sink that = {};
      return that;
    }

  private:
    output_sink* sink_;
  };

  namespace _
  {
    inline output_handler current_output {};
  }

  //====================================================================================================================
  /**
    @public
    @brief Retrieves the current TTS output_handler.

    @return A reference to the tts::output_handler instance used by the current test suite.
  **/
  //====================================================================================================================
  inline output_handler& output()
  {
    return _::current_output;
  }

  //====================================================================================================================
  /**
    @public
    @brief RAII guard installing an output_sink for its scope, restoring the previous one on exit.

    Saves whichever output_sink is currently installed on tts::output(), installs `s`, then restores
    the saved one once the guard goes out of scope - even through an early return or an exception.

    @groupheader{Example}
    @snippet doc/custom_output_sink.cpp snippet

    @see tts::output_sink
    @see tts::output_handler
  **/
  //====================================================================================================================
  class scoped_sink
  {
  public:
    /// Installs s as the current output_sink, saving whichever one was previously installed.
    explicit scoped_sink(output_sink& s)
    {
      output().sink(s);
    }

    ~scoped_sink()
    {
      output().sink(saved_);
    }

    scoped_sink(scoped_sink const&)            = delete;
    scoped_sink& operator=(scoped_sink const&) = delete;
    scoped_sink(scoped_sink&&)                 = delete;
    scoped_sink& operator=(scoped_sink&&)      = delete;

  private:
    output_sink& saved_ = output().sink();
  };

  //====================================================================================================================
  //! @}
  //====================================================================================================================
}

namespace tts::_
{
  //====================================================================================================================
  // Shared separator line for display helpers to visually delimit sections of output (see
  // tts::_::header/results in ranges.hpp). printable lets a caller fold its own display condition
  // (e.g. !tts::is_quiet()) into the call instead of guarding it at every call site.
  //====================================================================================================================
  inline void separator(bool printable = true)
  {
    if(printable)
      ::tts::output().writeln(
      "--------------------------------------------------------------------------------");
  }
}
