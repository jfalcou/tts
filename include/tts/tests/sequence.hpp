//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#ifndef TTS_TESTS_SEQUENCE_HPP_INCLUDED
#define TTS_TESTS_SEQUENCE_HPP_INCLUDED

#include <tts/tests/precision.hpp>
#include <tts/detail/infos.hpp>

//==================================================================================================
#define TTS_ALL_ULP_EQUAL(LHS, RHS, ULP)                                                           \
  do                                                                                               \
  {                                                                                                \
    bool ok = ::tts::detail::has_matching_size(runtime,                                            \
                                               {__FILE__, __LINE__},                               \
                                               TTS_STRING(LHS),                                    \
                                               TTS_STRING(RHS),                                    \
                                               ::tts::detail::size(LHS),                           \
                                               ::tts::detail::size(RHS));                          \
    if(ok)                                                                                         \
    {                                                                                              \
      auto r =                                                                                     \
      ::tts::detail::all_check(LHS, RHS, ULP, [](auto a, auto b) { return ::tts::ulpdist(a, b); });\
      ::tts::detail::report_all_errors(                                                            \
          runtime, {__FILE__, __LINE__}, r, ULP, TTS_STRING(LHS), TTS_STRING(RHS), "ULP");         \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

//==================================================================================================
#define TTS_ALL_IEEE_EQUAL(LHS, RHS)  TTS_ALL_ULP_EQUAL(LHS, RHS, 0.)

//==================================================================================================
#define TTS_ALL_RELATIVE_EQUAL(LHS, RHS, P)                                                        \
  do                                                                                               \
  {                                                                                                \
    bool ok = ::tts::detail::has_matching_size(runtime,                                            \
                                               {__FILE__, __LINE__},                               \
                                               TTS_STRING(LHS),                                    \
                                               TTS_STRING(RHS),                                    \
                                               ::tts::detail::size(LHS),                           \
                                               ::tts::detail::size(RHS));                          \
    if(ok)                                                                                         \
    {                                                                                              \
      auto r =                                                                                     \
      ::tts::detail::all_check(LHS, RHS, P, [](auto a, auto b) { return ::tts::reldist(a, b); });  \
      ::tts::detail::report_all_errors(                                                            \
          runtime, {__FILE__, __LINE__}, r, P, TTS_STRING(LHS), TTS_STRING(RHS), "%");             \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

//==================================================================================================
#define TTS_ALL_EQUAL(LHS, RHS) TTS_ALL_RELATIVE_EQUAL(LHS, RHS, 0)

//==================================================================================================
#define TTS_ALL_ABSOLUTE_EQUAL(LHS, RHS, X)                                                        \
  do                                                                                               \
  {                                                                                                \
    bool ok = ::tts::detail::has_matching_size(runtime,                                            \
                                               {__FILE__, __LINE__},                               \
                                               TTS_STRING(LHS),                                    \
                                               TTS_STRING(RHS),                                    \
                                               ::tts::detail::size(LHS),                           \
                                               ::tts::detail::size(RHS));                          \
    if(ok)                                                                                         \
    {                                                                                              \
      auto r =                                                                                     \
      ::tts::detail::all_check(LHS, RHS, X, [](auto a, auto b) { return ::tts::absdist(a, b); });  \
      ::tts::detail::report_all_errors(                                                            \
          runtime, {__FILE__, __LINE__}, r, X, TTS_STRING(LHS), TTS_STRING(RHS), " unit");         \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

#endif
