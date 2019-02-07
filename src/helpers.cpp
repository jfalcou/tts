//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <cstdlib>
#include <string>
#include <tts/detail/rt_helpers.hpp>
#include <tts/detail/color.hpp>

namespace tts::detail
{
  // Externally extensible to_string converter
  std::string to_string(std::nullptr_t) { return "nullptr"; }
  std::string to_string(bool v) { return v ? "true" : "false"; }
  std::string to_string(std::string const &v) { return v; }
  std::string to_string(char const *v) { return std::string(v); }
  std::string to_string(char v) { return std::string(1, v); }

  // null iostream
  int null_buffer_t::overflow(int c) { return c; }
  null_buffer_t null_buffer = {};
  std::ostream  null_stream(&null_buffer);

  // Simple wrapper for __FILE__/__LINE__
  std::ostream &operator<<(std::ostream &os, location const &l)
  {
    return os << darkgray_(l.file) << ":" << darkgray_(l.line);
  }

  location at(std::string const &f, std::size_t l) { return {f, l}; }
}
