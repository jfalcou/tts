//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/detail/termcolor.hpp>
#include <tts/detail/color.hpp>
#include <tts/detail/args.hpp>
#include <tts/api.hpp>

namespace tts::detail
{
  TTS_API std::ostream &grey(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::grey;
    return stream;
  }

  TTS_API std::ostream &red(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::red;
    return stream;
  }

  TTS_API std::ostream &green(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::green;
    return stream;
  }

  TTS_API std::ostream &yellow(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::yellow;
    return stream;
  }

  TTS_API std::ostream &blue(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::blue;
    return stream;
  }

  TTS_API std::ostream &magenta(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::magenta;
    return stream;
  }

  TTS_API std::ostream &cyan(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::cyan;
    return stream;
  }

  TTS_API std::ostream &white(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::white << termcolor::bold;
    return stream;
  }

  TTS_API std::ostream &reset(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::reset;
    return stream;
  }

  TTS_API std::ostream &bold(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::bold;
    return stream;
  }

  TTS_API std::ostream &italic(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::italic;
    return stream;
  }

  TTS_API std::ostream &underline(std::ostream &stream)
  {
    if (args.has_color())  stream << termcolor::underline;
    return stream;
  }
}
