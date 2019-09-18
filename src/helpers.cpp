//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/detail/rt_helpers.hpp>
#include <tts/api.hpp>
#include <cstdlib>
#include <iomanip>
#include <string>

namespace tts::detail
{
  // Externally extensible to_string converter
  TTS_API std::string to_string(std::nullptr_t) { return "nullptr"; }
  TTS_API std::string to_string(bool v) { return v ? "true" : "false"; }
  TTS_API std::string to_string(std::string const &v) { return v; }
  TTS_API std::string to_string(char const *v) { return std::string(v); }
  TTS_API std::string to_string(char v) { return std::string(1, v); }

  // null iostream
  int null_buffer_t::overflow(int c) { return c; }
  null_buffer_t null_buffer = {};
  std::ostream  null_stream(&null_buffer);
}
