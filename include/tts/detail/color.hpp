//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAILS_COLOR_HPP_INCLUDED
#define TTS_DETAILS_COLOR_HPP_INCLUDED

#include <tts/api.hpp>
#include <iosfwd>

namespace tts::detail
{
  TTS_API std::ostream &reset(std::ostream &stream);
  TTS_API std::ostream &bold(std::ostream &stream);
  TTS_API std::ostream &italic(std::ostream &stream);
  TTS_API std::ostream &underline(std::ostream &stream);

  TTS_API std::ostream &grey(std::ostream &stream);
  TTS_API std::ostream &red(std::ostream &stream);
  TTS_API std::ostream &green(std::ostream &stream);
  TTS_API std::ostream &yellow(std::ostream &stream);
  TTS_API std::ostream &blue(std::ostream &stream);
  TTS_API std::ostream &magenta(std::ostream &stream);
  TTS_API std::ostream &cyan(std::ostream &stream);
}

#endif
