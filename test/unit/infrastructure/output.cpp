//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE("Check is_verbose and is_quiet reflect the current verbosity state")
{
  bool const previous_verbose         = ::tts::_::current_verbosity.verbose;
  bool const previous_quiet           = ::tts::_::current_verbosity.quiet;

  ::tts::_::current_verbosity.verbose = false;
  ::tts::_::current_verbosity.quiet   = false;
  TTS_EXPECT_NOT(tts::is_verbose());
  TTS_EXPECT_NOT(tts::is_quiet());

  ::tts::_::current_verbosity.verbose = true;
  TTS_EXPECT(tts::is_verbose());
  TTS_EXPECT_NOT(tts::is_quiet());

  ::tts::_::current_verbosity.quiet = true;
  TTS_EXPECT(tts::is_verbose());
  TTS_EXPECT(tts::is_quiet());

  ::tts::_::current_verbosity.verbose = previous_verbose;
  ::tts::_::current_verbosity.quiet   = previous_quiet;
};

TTS_CASE("Check is_detailed is true only when verbose is set and quiet is not")
{
  bool const previous_verbose         = ::tts::_::current_verbosity.verbose;
  bool const previous_quiet           = ::tts::_::current_verbosity.quiet;

  ::tts::_::current_verbosity.verbose = false;
  ::tts::_::current_verbosity.quiet   = false;
  TTS_EXPECT_NOT(tts::is_detailed());

  ::tts::_::current_verbosity.verbose = true;
  ::tts::_::current_verbosity.quiet   = false;
  TTS_EXPECT(tts::is_detailed());

  ::tts::_::current_verbosity.verbose = true;
  ::tts::_::current_verbosity.quiet   = true;
  TTS_EXPECT_NOT(tts::is_detailed());

  ::tts::_::current_verbosity.verbose = false;
  ::tts::_::current_verbosity.quiet   = true;
  TTS_EXPECT_NOT(tts::is_detailed());

  ::tts::_::current_verbosity.verbose = previous_verbose;
  ::tts::_::current_verbosity.quiet   = previous_quiet;
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

TTS_CASE("Check report_fail only shows the failing type outside verbose mode")
{
  bool const          previous_verbose = ::tts::_::current_verbosity.verbose;
  bool const          previous_quiet   = ::tts::_::current_verbosity.quiet;

  tts::gathering_sink gs;
  auto&               out      = tts::output();
  auto&               previous = out.sink();
  out.sink(gs);

  // Quiet hides the "[X] ... FAILURE ..." line so only the type hint remains, isolating it.
  ::tts::_::current_verbosity.verbose = false;
  ::tts::_::current_verbosity.quiet   = true;
  ::tts::_::report_fail("[loc]", "msg", ::tts::text {"int"});
  TTS_EQUAL(gs.content(), ">  With <T = int>\n");

  gs.clear();
  ::tts::_::current_verbosity.verbose = true;
  ::tts::_::report_fail("[loc]", "msg", ::tts::text {"int"});
  TTS_EXPECT(gs.content().is_empty());

  out.sink(previous);
  ::tts::_::current_verbosity.verbose = previous_verbose;
  ::tts::_::current_verbosity.quiet   = previous_quiet;
};

TTS_CASE("Check report_fatal shares the same type hint behavior as report_fail")
{
  bool const          previous_verbose = ::tts::_::current_verbosity.verbose;
  bool const          previous_quiet   = ::tts::_::current_verbosity.quiet;

  tts::gathering_sink gs;
  auto&               out      = tts::output();
  auto&               previous = out.sink();
  out.sink(gs);

  ::tts::_::current_verbosity.verbose = false;
  ::tts::_::current_verbosity.quiet   = false;
  ::tts::_::report_fatal("[loc]", "msg", ::tts::text {"int"});
  TTS_EQUAL(gs.content(), ">  With <T = int>\n  [@] [loc] : @@ FATAL @@ : msg\n");

  gs.clear();
  ::tts::_::current_verbosity.verbose = true;
  ::tts::_::report_fatal("[loc]", "msg", ::tts::text {"int"});
  TTS_EQUAL(gs.content(), "  [@] [loc] : @@ FATAL @@ : msg\n");

  out.sink(previous);
  ::tts::_::current_verbosity.verbose = previous_verbose;
  ::tts::_::current_verbosity.quiet   = previous_quiet;
};
