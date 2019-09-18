//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//=====================================================================================================
#ifndef TTS_API_HPP_INCLUDED
#define TTS_API_HPP_INCLUDED

#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
  #ifdef TTS_API_EXPORTS
    #ifdef __GNUC__
      #define TTS_API __attribute__ ((dllexport))
    #else
      #define TTS_API __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define TTS_API __attribute__ ((dllimport))
    #else
      #define TTS_API __declspec(dllimport)
    #endif
  #endif
  #define TTS_INTERNAL
#else
  #define TTS_API       __attribute__ ((visibility ("default")))
  #define TTS_INTERNAL  __attribute__ ((visibility ("hidden")))
#endif

#endif
