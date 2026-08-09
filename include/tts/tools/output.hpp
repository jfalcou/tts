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

  inline verbosity current_verbosity = {};

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
  struct verbosity_scope
  {
    verbosity_scope()
        : saved(current_verbosity)
    {
    }

    ~verbosity_scope()
    {
      current_verbosity = saved;
    }

    verbosity saved;
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
    virtual ~output_sink()            = default;
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

    report.dump();                        // stream everything gathered so far at once
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

    /// Streams everything gathered so far to `stdout`, then clears the buffer.
    void dump()
    {
      fputs(buffer_.data(), stdout);
      clear();
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
