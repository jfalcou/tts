//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/options.hpp>
#include <tts/tools/preprocessor.hpp>

namespace tts::_
{
  // Splits the suite in `total` round-robin shards (test at registration index k belongs to
  // shard k % total), so a slow binary can be spread across parallel CI workers without
  // splitting source files. See --shard in doc/cli.hpp.
  struct shard_spec
  {
    bool         active = false;
    unsigned int index  = 0;
    unsigned int total  = 1;

    bool         selects(std::size_t position) const
    {
      return !active || (position % total == index);
    }

    std::size_t count(std::size_t suite_size) const
    {
      if(!active) return suite_size;
      if(suite_size <= index) return 0;
      return (suite_size - index - 1) / total + 1;
    }
  };

  // Parses --shard=i/n. `ok` is set to false if the flag is present but malformed or out of
  // range (n == 0 or i >= n); the returned shard_spec is then meaningless and must be ignored.
  TTS_DISABLE_WARNING_PUSH
  TTS_DISABLE_WARNING_CRT_SECURE
  inline shard_spec parse_shard(bool& ok)
  {
    ok              = true;
    ::tts::text raw = ::tts::arguments().value<::tts::text>("--shard");
    if(raw.is_empty()) return {};

    unsigned int i = 0;
    unsigned int n = 0;
    if(sscanf(raw.data(), "%u/%u", &i, &n) != 2 || n == 0 || i >= n)
    {
      ok = false;
      return {};
    }

    return {true, i, n};
  }
  TTS_DISABLE_WARNING_POP
}
