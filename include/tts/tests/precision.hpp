//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#ifndef TTS_TESTS_PRECISION_HPP_INCLUDED
#define TTS_TESTS_PRECISION_HPP_INCLUDED

#include <tts/detail/absdist.hpp>
#include <tts/detail/comparator.hpp>
#include <tts/detail/infos.hpp>
#include <tts/detail/precision.hpp>
#include <tts/detail/reldist.hpp>
#include <tts/detail/ulpdist.hpp>

//==================================================================================================
#define TTS_ULP_EQUAL(LHS, RHS, ULP)                                                               \
  do                                                                                               \
  {                                                                                                \
    auto eval_a = (LHS);                                                                           \
    auto eval_b = (RHS);                                                                           \
    ::tts::detail::check_precision(runtime,                                                        \
                                   {__FILE__, __LINE__},                                           \
                                   ::tts::ulpdist(eval_a, eval_b),                                 \
                                   ULP,                                                            \
                                   ::tts::detail::to_string(eval_a),                               \
                                   ::tts::detail::to_string(eval_b),                               \
                                   TTS_STRING(LHS),                                                \
                                   TTS_STRING(RHS),                                                \
                                   "ULP");                                                         \
  } while(::tts::detail::is_false())                                                               \
/**/

//==================================================================================================
#define TTS_IEEE_EQUAL(LHS, RHS)      TTS_ULP_EQUAL(LHS, RHS, 0.)

//==================================================================================================
#define TTS_RELATIVE_EQUAL(LHS, RHS, P)                                                            \
  do                                                                                               \
  {                                                                                                \
    auto eval_a = (LHS);                                                                           \
    auto eval_b = (RHS);                                                                           \
    ::tts::detail::check_precision(runtime,                                                        \
                                   {__FILE__, __LINE__},                                           \
                                   ::tts::reldist(eval_a, eval_b),                                 \
                                   P,                                                              \
                                   ::tts::detail::to_string(eval_a),                               \
                                   ::tts::detail::to_string(eval_b),                               \
                                   TTS_STRING(LHS),                                                \
                                   TTS_STRING(RHS),                                                \
                                   "%");                                                           \
  } while(::tts::detail::is_false())                                                               \
/**/

//==================================================================================================
#define TTS_ABSOLUTE_EQUAL(LHS, RHS, N)                                                            \
  do                                                                                               \
  {                                                                                                \
    auto eval_a = (LHS);                                                                           \
    auto eval_b = (RHS);                                                                           \
    ::tts::detail::check_precision(runtime,                                                        \
                                   {__FILE__, __LINE__},                                           \
                                   ::tts::absdist(eval_a, eval_b),                                 \
                                   N,                                                              \
                                   ::tts::detail::to_string(eval_a),                               \
                                   ::tts::detail::to_string(eval_b),                               \
                                   TTS_STRING(LHS),                                                \
                                   TTS_STRING(RHS),                                                \
                                   " unit");                                                       \
  } while(::tts::detail::is_false())                                                               \
/**/

#endif
