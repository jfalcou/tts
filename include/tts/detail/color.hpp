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

#include <tts/detail/args.hpp>
#include <tts/detail/rt_helpers.hpp>
#include <iostream>
#include <iomanip>

#if __has_include(<windows.h>)
#define NOMINMAX
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define TTS_USE_CONSOLE_ATTRIBUTES
#endif

namespace tts::detail
{
#if defined(TTS_USE_CONSOLE_ATTRIBUTES)
  enum Color { GRAY = 7, DARKGRAY = 8, GREEN = 10, RED = 12, MAGENTA = 13
             , YELLOW = 14, WHITE = 15, DEFAULT = 7
             };
#else
  enum Color  { GRAY = 37, DARKGRAY = 90, GREEN = 32, RED = 31, YELLOW = 33, MAGENTA = 35
              , WHITE = 97, DEFAULT = 39
              };
#endif

  template<typename Color> struct colored_data
  {
    std::string data;
    Color       color;
  };

  template<int Code> struct color_markup
  {
    template<typename T> inline std::basic_ostream<T>& setup(std::basic_ostream<T>& os) const
    {
      if(&os == &std::cout && args.has_color())
      {
#if defined(TTS_USE_CONSOLE_ATTRIBUTES)
        auto h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, Code);
#else
        os << "\033[" << Code << "m";
#endif
      }

      return os;
    }

    template<typename T> colored_data<color_markup<Code>> operator()(T const& t) const
    {
      return {to_string(t),*this};
    }
  };

  template<int Code> inline std::ostream& operator<<(std::ostream& os, const color_markup<Code>& cm)
  {
    return cm.setup(os);
  }

  inline color_markup<Color::RED>       const red_      = {};
  inline color_markup<Color::GREEN>     const green_    = {};
  inline color_markup<Color::WHITE>     const white_    = {};
  inline color_markup<Color::MAGENTA>   const magenta_  = {};
  inline color_markup<Color::GRAY>      const gray_     = {};
  inline color_markup<Color::YELLOW>    const yellow_   = {};
  inline color_markup<Color::DEFAULT>   const default_  = {};
  inline color_markup<Color::DARKGRAY>  const darkgray_ = {};

  template<typename C>
  inline std::ostream& operator<<(std::ostream& os, const colored_data<C>& cd)
  {
    os << cd.color << cd.data << default_;
    return os;
  }
}

#endif
