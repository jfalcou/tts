//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_PRECISION_HPP_INCLUDED
#define TTS_TESTS_PRECISION_HPP_INCLUDED

#include <tts/detail/absdist.hpp>
#include <tts/detail/comparator.hpp>
#include <tts/detail/infos.hpp>
#include <tts/detail/precision.hpp>
#include <tts/detail/reldist.hpp>
#include <tts/detail/ulpdist.hpp>

#define TTS_ULP_EQUAL(A, B, X)                                                                     \
  ::tts::detail::check_precision(runtime,                                                          \
                                 {__FILE__, __LINE__},                                             \
                                 ::tts::ulpdist((A), (B)),                                         \
                                 X,                                                                \
                                 TTS_STRING(A),                                                    \
                                 TTS_STRING(B),                                                    \
                                 "ULP") /**/

#define TTS_ALL_ULP_EQUAL(A, B, X)                                                                 \
  do                                                                                               \
  {                                                                                                \
    bool ok = ::tts::detail::has_matching_size(runtime,                                            \
                                               {__FILE__, __LINE__},                               \
                                               TTS_STRING(A),                                      \
                                               TTS_STRING(B),                                      \
                                               ::tts::detail::size(A),                             \
                                               ::tts::detail::size(B));                            \
    if(ok)                                                                                         \
    {                                                                                              \
      auto r =                                                                                     \
          ::tts::detail::all_check(A, B, X, [](auto a, auto b) { return ::tts::ulpdist(a, b); });  \
      ::tts::detail::report_all_errors(                                                            \
          runtime, {__FILE__, __LINE__}, r, X, TTS_STRING(A), TTS_STRING(B), "ULP");               \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

#define TTS_IEEE_EQUAL(A, B) TTS_ULP_EQUAL(A, B, 0.)
#define TTS_ALL_IEEE_EQUAL(A, B) TTS_ALL_ULP_EQUAL(A, B, 0.)

#define TTS_RELATIVE_EQUAL(A, B, X)                                                                \
  ::tts::detail::check_precision(runtime,                                                          \
                                 {__FILE__, __LINE__},                                             \
                                 ::tts::reldist((A), (B)),                                         \
                                 X,                                                                \
                                 TTS_STRING(A),                                                    \
                                 TTS_STRING(B),                                                    \
                                 "%") /**/

#define TTS_ALL_RELATIVE_EQUAL(A, B, X)                                                            \
  do                                                                                               \
  {                                                                                                \
    bool ok = ::tts::detail::has_matching_size(runtime,                                            \
                                               {__FILE__, __LINE__},                               \
                                               TTS_STRING(A),                                      \
                                               TTS_STRING(B),                                      \
                                               ::tts::detail::size(A),                             \
                                               ::tts::detail::size(B));                            \
    if(ok)                                                                                         \
    {                                                                                              \
      auto r =                                                                                     \
          ::tts::detail::all_check(A, B, X, [](auto a, auto b) { return ::tts::reldist(a, b); });  \
      ::tts::detail::report_all_errors(                                                            \
          runtime, {__FILE__, __LINE__}, r, X, TTS_STRING(A), TTS_STRING(B), "%");                 \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

#define TTS_ALL_EQUAL(A, B) TTS_ALL_RELATIVE_EQUAL(A, B, 0)

#define TTS_ABSOLUTE_EQUAL(A, B, X)                                                                \
  ::tts::detail::check_precision(runtime,                                                          \
                                 {__FILE__, __LINE__},                                             \
                                 ::tts::absdist((A), (B)),                                         \
                                 X,                                                                \
                                 TTS_STRING(A),                                                    \
                                 TTS_STRING(B),                                                    \
                                 " unit") /**/

#define TTS_ALL_ABSOLUTE_EQUAL(A, B, X)                                                            \
  do                                                                                               \
  {                                                                                                \
    bool ok = ::tts::detail::has_matching_size(runtime,                                            \
                                               {__FILE__, __LINE__},                               \
                                               TTS_STRING(A),                                      \
                                               TTS_STRING(B),                                      \
                                               ::tts::detail::size(A),                             \
                                               ::tts::detail::size(B));                            \
    if(ok)                                                                                         \
    {                                                                                              \
      auto r =                                                                                     \
          ::tts::detail::all_check(A, B, X, [](auto a, auto b) { return ::tts::absdist(a, b); });  \
      ::tts::detail::report_all_errors(                                                            \
          runtime, {__FILE__, __LINE__}, r, X, TTS_STRING(A), TTS_STRING(B), " unit");             \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

#endif
