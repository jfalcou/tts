//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#if defined(_WIN32) || defined(_WIN64)
#define TTS_COLOR_WINDOWS
#endif

#if defined(TTS_COLOR_WINDOWS)
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <cstdio>
#include <iostream>

namespace tts::detail
{
  inline bool color_status = true;

  inline std::ostream & modifiy_stream(std::ostream &stream, int foreground)
  {
    if(color_status)
    {
#if defined(TTS_COLOR_WINDOWS)
      static WORD defaultAttributes = 0;

      HANDLE hTerminal = INVALID_HANDLE_VALUE;
            if (&stream == &std::cout)  hTerminal = GetStdHandle(STD_OUTPUT_HANDLE);
      else  if (&stream == &std::cerr)  hTerminal = GetStdHandle(STD_ERROR_HANDLE);

      if (!defaultAttributes)
      {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (!GetConsoleScreenBufferInfo(hTerminal, &info)) return stream;
        defaultAttributes = info.wAttributes;
      }

      if (foreground > 7) return stream

      if (foreground == -1)
      {
        SetConsoleTextAttribute(hTerminal, defaultAttributes);
        return stream;
      }

      CONSOLE_SCREEN_BUFFER_INFO info;
      if (!GetConsoleScreenBufferInfo(hTerminal, &info)) return stream;

      info.wAttributes &= ~(info.wAttributes & 0x0F);
      info.wAttributes |= static_cast<WORD>(FOREGROUND_INTENSITY | foreground);

      SetConsoleTextAttribute(hTerminal, info.wAttributes);
      return stream;
#else
    constexpr char const* modifier[] =
    {
      "\033[00m", /* -1 reset */
      "\033[30m", /* 0 = grey   */ "\033[34m", /* 1 = blue  */ "\033[32m", /* 2 = green   */
      "\033[36m", /* 3 = cyan   */ "\033[31m", /* 4 = red   */ "\033[35m", /* 5 = magenta */
      "\033[33m", /* 6 = yellow */ "\033[37m", /* 7 = white */ "\033[1m" , /* 8 = bold    */
    };

    return stream << modifier[foreground+1];
#endif
    }
    else
    {
      return stream;
    }
  }
}

#undef TTS_COLOR_WINDOWS

namespace tts
{
  //================================================================================================
  // Console colors using ANSI sequence or WIN32 SetConsoleTextAttribute
  //================================================================================================
  inline std::ostream& reset  (std::ostream &stream) { return detail::modifiy_stream(stream, -1); }
  inline std::ostream& grey   (std::ostream &stream) { return detail::modifiy_stream(stream,  0); }
  inline std::ostream& blue   (std::ostream &stream) { return detail::modifiy_stream(stream,  1); }
  inline std::ostream& green  (std::ostream &stream) { return detail::modifiy_stream(stream,  2); }
  inline std::ostream& cyan   (std::ostream &stream) { return detail::modifiy_stream(stream,  3); }
  inline std::ostream& red    (std::ostream &stream) { return detail::modifiy_stream(stream,  4); }
  inline std::ostream& magenta(std::ostream &stream) { return detail::modifiy_stream(stream,  5); }
  inline std::ostream& yellow (std::ostream &stream) { return detail::modifiy_stream(stream,  6); }
  inline std::ostream& white  (std::ostream &stream) { return detail::modifiy_stream(stream,  7); }
  inline std::ostream& bold   (std::ostream &stream) { return detail::modifiy_stream(stream,  8); }
}
