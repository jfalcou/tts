//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>
#include <array>

TTS_CASE("Check env_var_name derives TTS_<NAME> from a long-form flag")
{
  TTS_EQUAL(tts::_::env_var_name("--shard"), "TTS_SHARD");
  TTS_EQUAL(tts::_::env_var_name("--allow-empty"), "TTS_ALLOW_EMPTY");
};

TTS_CASE("Check env_var_name rejects short flags and null")
{
  TTS_EXPECT(tts::_::env_var_name("-v").is_empty());
  TTS_EXPECT(tts::_::env_var_name(nullptr).is_empty());
};

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

TTS_DISABLE_WARNING_PUSH
TTS_DISABLE_WARNING_CRT_SECURE

// putenv() doesn't copy its argument, it stores the pointer directly in the environment -
// passing it a pointer to an automatic (stack) variable is undefined behavior once that
// variable goes out of scope (CERT POS34-C). Every call below is safe because it's given a
// string literal, which has static storage duration for the whole program - never copy this
// pattern with a dynamically-built buffer without also giving it static/longer storage.

TTS_CASE("Check options falls back to a TTS_<FLAG> environment variable when no CLI flag matches")
{
  putenv(const_cast<char*>("TTS_QUX_TEST_FLAG=1")); // NOSONAR

  std::array<char const*, 1> argv {"prog"};
  tts::options               opts {1, argv.data()};

  TTS_EXPECT(opts("--qux-test-flag"));
  TTS_EXPECT_NOT(opts("--some-other-flag"));
};

TTS_CASE("Check a CLI flag takes precedence over its environment variable")
{
  putenv(const_cast<char*>("TTS_ANOTHER_TEST_FLAG=fromenv")); // NOSONAR

  std::array<char const*, 2> argv {"prog", "--another-test-flag=fromcli"};
  tts::options               opts {2, argv.data()};

  TTS_EQUAL(opts.value<tts::text>("--another-test-flag"), "fromcli");
};

TTS_CASE("Check a value-taking option reads its environment variable")
{
  putenv(const_cast<char*>("TTS_YET_ANOTHER_TEST_FLAG=7")); // NOSONAR

  std::array<char const*, 1> argv {"prog"};
  tts::options               opts {1, argv.data()};

  TTS_EQUAL(opts.value<int>("--yet-another-test-flag"), 7);
};

TTS_DISABLE_WARNING_POP
