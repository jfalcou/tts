//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE("Check default constructed option is invalid")
{
  tts::_::option o;
  TTS_EXPECT_NOT(o.is_valid());
};

TTS_CASE("Check option parses a bare CLI flag (no value)")
{
  tts::_::option o {"--verbose"};
  TTS_EXPECT(o.is_valid());
  TTS_EXPECT(o.has_flag("--verbose"));
  TTS_EXPECT_NOT(o.has_flag("--quiet"));
};

TTS_CASE("Check option parses a CLI flag with a value")
{
  tts::_::option o {"--seed=42"};
  TTS_EXPECT(o.is_valid());
  TTS_EXPECT(o.has_flag("--seed"));
  TTS_EQUAL(o.get<int>(), 42);
  TTS_EQUAL(o.get<double>(), 42.);
};

TTS_CASE("Check option built from a separate name/value pair (env var style)")
{
  tts::_::option o {"--shard", "1/4"};
  TTS_EXPECT(o.is_valid());
  TTS_EXPECT(o.has_flag("--shard"));
  TTS_EXPECT_NOT(o.has_flag("--seed"));
  TTS_EQUAL(o.get<tts::text>(), "1/4");
};

TTS_CASE("Check option::get from a separate name/value pair parses integrals and floats")
{
  tts::_::option i {"--block", "4096"};
  TTS_EQUAL(i.get<int>(), 4096);

  tts::_::option f {"--ulpmax", "2.5"};
  TTS_EQUAL(f.get<double>(), 2.5);
};
