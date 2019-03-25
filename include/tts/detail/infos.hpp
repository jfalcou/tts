//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_INFO_HPP_INCLUDED
#define TTS_DETAIL_INFO_HPP_INCLUDED

#include <tts/engine/env.hpp>
#include <cstddef>
#include <string>
#include <iostream>

namespace tts::detail
{
  // Simple wrapper for __FILE__/__LINE__
  struct location
  {
    std::string file;
    std::size_t line;
  };

  std::ostream &operator<<(std::ostream &os, location const &l);

  std::ostream& pass(tts::env& runtime, location const& l);
  std::ostream& fail(tts::env& runtime, location const& l);
  std::ostream& invalid(tts::env& runtime, location const& l);
}

#endif
