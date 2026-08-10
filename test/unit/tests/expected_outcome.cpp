//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>

TTS_CASE("expect_fail/may_fail/expect_invalid tag their ID correctly")
{
  TTS_EQUAL(::tts::expect_fail("x").tag, ::tts::expected_outcome::xfail);
  TTS_EQUAL(::tts::may_fail("x").tag, ::tts::expected_outcome::may_fail);
  TTS_EQUAL(::tts::expect_invalid("x").tag, ::tts::expected_outcome::xinvalid);
};

namespace
{
  ::tts::expected_outcome tag_of(char const* name)
  {
    for(auto const& t: ::tts::_::suite())
      if(::tts::text {t.name} == ::tts::text {name}) return t.tag;
    return ::tts::expected_outcome::pass;
  }
}

TTS_CASE(TTS_XFAIL("xfail-tagged plain case"))
{
  TTS_EXPECT(1 == 2); // genuinely expected to fail - matches its own tag
};

TTS_CASE(TTS_MAYFAIL("mayfail-tagged plain case"))
{
  TTS_EXPECT(1 == 2); // tolerated either way
};

TTS_CASE(TTS_XINVALID("xinvalid-tagged plain case"))
{
// genuinely empty - matches its own tag
};

TTS_CASE_TPL(TTS_XFAIL("xfail-tagged template case"), int, float)
(auto)
{
  TTS_EXPECT(1 == 2);
};

TTS_CASE("Registered tags are visible on the corresponding tts::_::test entries")
{
  TTS_EQUAL(tag_of("xfail-tagged plain case"), ::tts::expected_outcome::xfail);
  TTS_EQUAL(tag_of("mayfail-tagged plain case"), ::tts::expected_outcome::may_fail);
  TTS_EQUAL(tag_of("xinvalid-tagged plain case"), ::tts::expected_outcome::xinvalid);
  TTS_EQUAL(tag_of("xfail-tagged template case"), ::tts::expected_outcome::xfail);
  TTS_EQUAL(tag_of("Registered tags are visible on the corresponding tts::_::test entries"),
            ::tts::expected_outcome::pass);
};

TTS_CASE("report() passes when every case matched its declared tag")
{
  ::tts::gathering_sink silence;
  ::tts::scoped_sink    quiet {silence};

  ::tts::_::env         local;
  local.test_count       = 3;
  local.success_count    = 2;
  local.failure_count    = 1;
  local.unexpected_count = 0; // the one failure matched its own tag (e.g. TTS_XFAIL)

  TTS_EQUAL(local.report(0, 0), 0);
};

TTS_CASE("report() fails when at least one case's outcome didn't match its tag")
{
  ::tts::gathering_sink silence;
  ::tts::scoped_sink    quiet {silence};

  ::tts::_::env         local;
  local.test_count       = 3;
  local.success_count    = 3;
  local.unexpected_count = 1; // e.g. a TTS_XFAIL case that unexpectedly passed

  TTS_EQUAL(local.report(0, 0), 1);
};

TTS_CASE("The legacy two-argument report(fails, invalids) form ignores unexpected_count")
{
  ::tts::gathering_sink silence;
  ::tts::scoped_sink    quiet {silence};

  ::tts::_::env         local;
  local.test_count       = 3;
  local.failure_count    = 2;
  local.invalid_count    = 1;
  local.unexpected_count = 5; // irrelevant on the explicit fails/invalids path

  TTS_EQUAL(local.report(2, 1), 0);
  TTS_EQUAL(local.report(1, 1), 1);
};
