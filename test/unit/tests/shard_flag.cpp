//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION shard_flag_main
#include <tts/tts.hpp>
#include <array>

TTS_CASE("shard_spec partitions round-robin and stays balanced")
{
  tts::_::shard_spec    s0 {true, 0, 3};
  tts::_::shard_spec    s1 {true, 1, 3};
  tts::_::shard_spec    s2 {true, 2, 3};

  constexpr std::size_t n  = 17;
  std::size_t           c0 = 0;
  std::size_t           c1 = 0;
  std::size_t           c2 = 0;

  for(std::size_t k = 0; k < n; ++k)
  {
    // Every index belongs to exactly one of the three shards.
    TTS_EQUAL(static_cast<int>(s0.selects(k)) + static_cast<int>(s1.selects(k)) +
              static_cast<int>(s2.selects(k)),
              1);

    if(s0.selects(k)) ++c0;
    if(s1.selects(k)) ++c1;
    if(s2.selects(k)) ++c2;
  }

  TTS_EQUAL(c0, s0.count(n));
  TTS_EQUAL(c1, s1.count(n));
  TTS_EQUAL(c2, s2.count(n));
  TTS_EQUAL(c0 + c1 + c2, n);
};

namespace
{
  // Intentionally mutable: set from within the test bodies below to prove they ran.
  bool first_dummy_ran  = false; // NOSONAR
  bool second_dummy_ran = false; // NOSONAR
}

TTS_CASE("First dummy test so shard_flag_main has something to (not) filter")
{
  first_dummy_ran = true;
  TTS_EXPECT(1 == 1);
};

TTS_CASE("Second dummy test so shard_flag_main has something to (not) filter")
{
  second_dummy_ran = true;
  TTS_EXPECT(1 == 1);
};

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);

  bool ok = true;

  // A shard landing on zero tests must not fail, same as --allow-empty (nothing has run yet
  // at this point in a fresh process, so test_count is guaranteed to still be 0 here).
  {
    std::array<char const*, 2> empty_shard_argv {argv[ 0 ], "--shard=0/2"};
    ::tts::_::current_arguments = ::tts::options {2, empty_shard_argv.data()};

    ok                          = ok && (::tts::global_runtime.test_count == 0);
    ok                          = ok && (::tts::report(0, 0) == 0);
  }

  // Valid --shard= is parsed correctly.
  {
    std::array<char const*, 2> valid_argv {argv[ 0 ], "--shard=1/4"};
    ::tts::_::current_arguments   = ::tts::options {2, valid_argv.data()};

    bool                 parse_ok = false;
    ::tts::_::shard_spec spec     = ::tts::_::parse_shard(parse_ok);
    ok = ok && parse_ok && spec.active && (spec.index == 1) && (spec.total == 4);
  }

  // Malformed or out-of-range --shard= values are rejected.
  for(char const* bad: {"--shard=abc", "--shard=2/2", "--shard=1/0", "--shard=/3", "--shard=1/"})
  {
    std::array<char const*, 2> bad_argv {argv[ 0 ], bad};
    ::tts::_::current_arguments = ::tts::options {2, bad_argv.data()};

    bool parse_ok               = true;
    ::tts::_::parse_shard(parse_ok);
    ok = ok && !parse_ok;
  }

  // No --shard at all: parsing succeeds and yields an inactive shard.
  {
    std::array<char const*, 1> no_shard_argv {argv[ 0 ]};
    ::tts::_::current_arguments   = ::tts::options {1, no_shard_argv.data()};

    bool                 parse_ok = false;
    ::tts::_::shard_spec spec     = ::tts::_::parse_shard(parse_ok);
    ok                            = ok && parse_ok && !spec.active;
  }

  // --shard is a no-op for a custom driver: every registered test still runs, not just the
  // round-robin subset --shard=0/2 would otherwise select.
  {
    std::array<char const*, 2> shard_argv {argv[ 0 ], "--shard=0/2"};
    ::tts::_::current_arguments = ::tts::options {2, shard_argv.data()};

    shard_flag_main(argc, argv);
    ok = ok && first_dummy_ran && second_dummy_ran;
  }

  return ok ? 0 : 1;
}
