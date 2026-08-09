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
  ::tts::_::scoped_verbosity scope;

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

TTS_CASE("Check scoped_verbosity saves and restores current_verbosity")
{
  ::tts::_::scoped_verbosity outer; // restores whatever was in place before this test ran

  ::tts::_::set_verbose(false);
  ::tts::_::set_quiet(true);

  {
    ::tts::_::scoped_verbosity inner;
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
  ::tts::_::scoped_verbosity scope;

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
  ::tts::_::scoped_verbosity scope;

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
  tts::scoped_sink    scope(gs);

  ::tts::_::separator(false);
  TTS_EXPECT(gs.content().is_empty());

  ::tts::_::separator(true);
  TTS_EQUAL(gs.content(),
            "--------------------------------------------------------------------------------\n");

  gs.clear();
  ::tts::_::separator();
  TTS_EQUAL(gs.content(),
            "--------------------------------------------------------------------------------\n");
};

TTS_CASE("Check output_handler defaults to tts::output_handler::default_sink")
{
  TTS_EXPECT(&tts::output().sink() == &tts::output_handler::default_sink());
};

TTS_CASE("Check gathering_sink accumulates written text instead of streaming it")
{
  tts::gathering_sink gs;
  auto&               out = tts::output();

  {
    tts::scoped_sink scope(gs);
    out.write("Hello ");
    out.write("World %d", 42);
    out.writeln("!");
  }

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

TTS_CASE("Check gathering_sink::dump(sink) forwards content to it then clears the source")
{
  tts::gathering_sink source;
  tts::gathering_sink target;

  source.write(tts::text {"forwarded content"});
  source.dump(target);

  TTS_EQUAL(target.content(), "forwarded content");
  TTS_EXPECT(source.content().is_empty());
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
  auto&          out = tts::output();

  {
    tts::scoped_sink scope(sink);
    out.flush();
  }

  TTS_EQUAL(sink.flushes, 1);
};

TTS_CASE("Check scoped_sink installs a sink and restores the previous one on scope exit")
{
  tts::gathering_sink gs;
  auto&               out      = tts::output();
  auto&               previous = out.sink();

  {
    tts::scoped_sink scope(gs);
    TTS_EQUAL(&out.sink(), &gs);
    out.write("inside scope");
  }

  TTS_EQUAL(&out.sink(), &previous);
  TTS_EQUAL(gs.content(), "inside scope");
};

TTS_CASE("Check nested scoped_sink instances restore in the right order")
{
  tts::gathering_sink outer_gs;
  tts::gathering_sink inner_gs;
  auto&               out      = tts::output();
  auto&               previous = out.sink();

  {
    tts::scoped_sink outer(outer_gs);
    {
      tts::scoped_sink inner(inner_gs);
      out.write("inner");
    }
    TTS_EQUAL(&out.sink(), &outer_gs);
    out.write("outer");
  }

  TTS_EQUAL(&out.sink(), &previous);
  TTS_EQUAL(outer_gs.content(), "outer");
  TTS_EQUAL(inner_gs.content(), "inner");
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
  ::tts::_::scoped_verbosity scope;

  tts::gathering_sink        gs;
  tts::scoped_sink           sink_scope(gs);

  // Quiet hides the "[X] ... FAILURE ..." line so only the type hint remains, isolating it.
  ::tts::_::set_verbose(false);
  ::tts::_::set_quiet(true);
  ::tts::_::report_fail("[loc]", "msg", ::tts::text {"int"});
  TTS_EQUAL(gs.content(), ">  With <T = int>\n");

  gs.clear();
  ::tts::_::set_verbose(true);
  ::tts::_::report_fail("[loc]", "msg", ::tts::text {"int"});
  TTS_EXPECT(gs.content().is_empty());
};

TTS_CASE("Check report_fatal shares the same type hint behavior as report_fail")
{
  ::tts::_::scoped_verbosity scope;

  tts::gathering_sink        gs;
  tts::scoped_sink           sink_scope(gs);

  ::tts::_::set_verbose(false);
  ::tts::_::set_quiet(false);
  ::tts::_::report_fatal("[loc]", "msg", ::tts::text {"int"});
  TTS_EQUAL(gs.content(), ">  With <T = int>\n  [@] [loc] : @@ FATAL @@ : msg\n");

  gs.clear();
  ::tts::_::set_verbose(true);
  ::tts::_::report_fatal("[loc]", "msg", ::tts::text {"int"});
  TTS_EQUAL(gs.content(), "  [@] [loc] : @@ FATAL @@ : msg\n");
};
