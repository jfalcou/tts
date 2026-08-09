//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE("Check set_verbose and set_quiet are the only writers of current_verbosity")
{
  ::tts::_::verbosity_scope scope;

  ::tts::_::set_verbose(false);
  ::tts::_::set_quiet(false);
  TTS_EXPECT_NOT(::tts::_::current_verbosity.verbose);
  TTS_EXPECT_NOT(::tts::_::current_verbosity.quiet);

  ::tts::_::set_verbose(true);
  TTS_EXPECT(::tts::_::current_verbosity.verbose);
  TTS_EXPECT_NOT(::tts::_::current_verbosity.quiet);

  ::tts::_::set_quiet(true);
  TTS_EXPECT(::tts::_::current_verbosity.verbose);
  TTS_EXPECT(::tts::_::current_verbosity.quiet);

  ::tts::_::set_verbose(false);
  TTS_EXPECT_NOT(::tts::_::current_verbosity.verbose);
  TTS_EXPECT(::tts::_::current_verbosity.quiet);
};

TTS_CASE("Check verbosity_scope saves and restores current_verbosity")
{
  ::tts::_::verbosity_scope outer; // restores whatever was in place before this test ran

  ::tts::_::set_verbose(false);
  ::tts::_::set_quiet(true);

  {
    ::tts::_::verbosity_scope inner;
    ::tts::_::set_verbose(true);
    ::tts::_::set_quiet(false);
    TTS_EXPECT(tts::is_verbose());
    TTS_EXPECT_NOT(tts::is_quiet());
  }

  TTS_EXPECT_NOT(tts::is_verbose());
  TTS_EXPECT(tts::is_quiet());
};

TTS_CASE("Check is_verbose and is_quiet reflect the current verbosity state")
{
  ::tts::_::verbosity_scope scope;

  ::tts::_::set_verbose(false);
  ::tts::_::set_quiet(false);
  TTS_EXPECT_NOT(tts::is_verbose());
  TTS_EXPECT_NOT(tts::is_quiet());

  ::tts::_::set_verbose(true);
  TTS_EXPECT(tts::is_verbose());
  TTS_EXPECT_NOT(tts::is_quiet());

  ::tts::_::set_quiet(true);
  TTS_EXPECT(tts::is_verbose());
  TTS_EXPECT(tts::is_quiet());
};

TTS_CASE("Check is_detailed is true only when verbose is set and quiet is not")
{
  ::tts::_::verbosity_scope scope;

  ::tts::_::set_verbose(false);
  ::tts::_::set_quiet(false);
  TTS_EXPECT_NOT(tts::is_detailed());

  ::tts::_::set_verbose(true);
  ::tts::_::set_quiet(false);
  TTS_EXPECT(tts::is_detailed());

  ::tts::_::set_verbose(true);
  ::tts::_::set_quiet(true);
  TTS_EXPECT_NOT(tts::is_detailed());

  ::tts::_::set_verbose(false);
  ::tts::_::set_quiet(true);
  TTS_EXPECT_NOT(tts::is_detailed());
};

TTS_CASE("Check separator writes an 80-dash line only when printable is true")
{
  tts::gathering_sink gs;
  auto&               out      = tts::output();
  auto&               previous = out.sink();
  out.sink(gs);

  ::tts::_::separator(false);
  TTS_EXPECT(gs.content().is_empty());

  ::tts::_::separator(true);
  TTS_EQUAL(gs.content(),
            "--------------------------------------------------------------------------------\n");

  gs.clear();
  ::tts::_::separator();
  TTS_EQUAL(gs.content(),
            "--------------------------------------------------------------------------------\n");

  out.sink(previous);
};

TTS_CASE("Check output_handler defaults to tts::output_handler::default_sink")
{
  TTS_EXPECT(&tts::output().sink() == &tts::output_handler::default_sink());
};

TTS_CASE("Check gathering_sink accumulates written text instead of streaming it")
{
  tts::gathering_sink gs;
  auto&               out      = tts::output();
  auto&               previous = out.sink();

  out.sink(gs);
  out.write("Hello ");
  out.write("World %d", 42);
  out.writeln("!");
  out.sink(previous);

  TTS_EQUAL(gs.content(), "Hello World 42!\n");
};

TTS_CASE("Check gathering_sink::clear discards gathered content")
{
  tts::gathering_sink gs;
  gs.write(tts::text {"some text"});
  TTS_EXPECT_NOT(gs.content().is_empty());

  gs.clear();
  TTS_EXPECT(gs.content().is_empty());
};

TTS_CASE("Check gathering_sink::dump streams then clears the gathered content")
{
  tts::gathering_sink gs;
  gs.write(tts::text {"dumped content\n"});

  gs.dump();

  TTS_EXPECT(gs.content().is_empty());
};

TTS_CASE("Check output_handler dispatches to a power user defined output_sink")
{
  struct counting_sink : tts::output_sink
  {
    int       calls = 0;
    tts::text last;

    void      write(tts::text const& t) override
    {
      calls++;
      last = t;
    }
  };

  counting_sink custom;
  auto&         out      = tts::output();
  auto&         previous = out.sink();

  out.sink(custom);
  out.write("custom output");
  out.sink(previous);

  TTS_EQUAL(custom.calls, 1);
  TTS_EQUAL(custom.last, "custom output");
};

TTS_CASE("Check output_handler::flush dispatches to the current output_sink")
{
  struct flushable_sink : tts::output_sink
  {
    int  flushes = 0;
    void write(tts::text const&) override
    {
      // Unused: this test only exercises flush().
    }
    void flush() override
    {
      flushes++;
    }
  };

  flushable_sink sink;
  auto&          out      = tts::output();
  auto&          previous = out.sink();

  out.sink(sink);
  out.flush();
  out.sink(previous);

  TTS_EQUAL(sink.flushes, 1);
};

TTS_CASE("Check output_sink::flush defaults to a no-op")
{
  tts::gathering_sink gs;
  gs.write(tts::text {"kept"});

  gs.flush();

  TTS_EQUAL(gs.content(), "kept");
};

TTS_CASE("Check report_fail only shows the failing type outside verbose mode")
{
  ::tts::_::verbosity_scope scope;

  tts::gathering_sink       gs;
  auto&                     out      = tts::output();
  auto&                     previous = out.sink();
  out.sink(gs);

  // Quiet hides the "[X] ... FAILURE ..." line so only the type hint remains, isolating it.
  ::tts::_::set_verbose(false);
  ::tts::_::set_quiet(true);
  ::tts::_::report_fail("[loc]", "msg", ::tts::text {"int"});
  TTS_EQUAL(gs.content(), ">  With <T = int>\n");

  gs.clear();
  ::tts::_::set_verbose(true);
  ::tts::_::report_fail("[loc]", "msg", ::tts::text {"int"});
  TTS_EXPECT(gs.content().is_empty());

  out.sink(previous);
};

TTS_CASE("Check report_fatal shares the same type hint behavior as report_fail")
{
  ::tts::_::verbosity_scope scope;

  tts::gathering_sink       gs;
  auto&                     out      = tts::output();
  auto&                     previous = out.sink();
  out.sink(gs);

  ::tts::_::set_verbose(false);
  ::tts::_::set_quiet(false);
  ::tts::_::report_fatal("[loc]", "msg", ::tts::text {"int"});
  TTS_EQUAL(gs.content(), ">  With <T = int>\n  [@] [loc] : @@ FATAL @@ : msg\n");

  gs.clear();
  ::tts::_::set_verbose(true);
  ::tts::_::report_fatal("[loc]", "msg", ::tts::text {"int"});
  TTS_EQUAL(gs.content(), "  [@] [loc] : @@ FATAL @@ : msg\n");

  out.sink(previous);
};
