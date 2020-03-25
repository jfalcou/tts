//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
*/
//===================================================================================================
#ifndef TTS_TESTS_RELATION_HPP_INCLUDED
#define TTS_TESTS_RELATION_HPP_INCLUDED

#include <tts/detail/comparator.hpp>
#include <tts/detail/pp_helpers.hpp>
#include <tts/detail/infos.hpp>
#include <tts/detail/relations.hpp>

//==================================================================================================
#define TTS_EQUAL(LHS, RHS)                                                                        \
do                                                                                                 \
{                                                                                                  \
  auto eval_a = (LHS);                                                                             \
  auto eval_b = (RHS);                                                                             \
  ::tts::detail::check_relation(runtime,                                                           \
                                {__FILE__, __LINE__},                                              \
                                tts::detail::eq(eval_a, eval_b),                                   \
                                TTS_STRING(LHS),                                                   \
                                TTS_STRING(RHS),                                                   \
                                "==",                                                              \
                                ::tts::detail::to_string(eval_a),                                  \
                                ::tts::detail::to_string(eval_b));                                 \
} while( ::tts::detail::is_false() )                                                               \
/**/

//==================================================================================================
#define TTS_NOT_EQUAL(LHS, RHS)                                                                    \
do                                                                                                 \
{                                                                                                  \
  auto eval_a = (LHS);                                                                             \
  auto eval_b = (RHS);                                                                             \
  ::tts::detail::check_relation(runtime,                                                           \
                                {__FILE__, __LINE__},                                              \
                                tts::detail::neq(eval_a, eval_b),                                  \
                                TTS_STRING(LHS),                                                   \
                                TTS_STRING(RHS),                                                   \
                                "!=",                                                              \
                                ::tts::detail::to_string(eval_a),                                  \
                                ::tts::detail::to_string(eval_b));                                 \
} while( ::tts::detail::is_false() )                                                               \
/**/

//==================================================================================================
#define TTS_LESS(LHS, RHS)                                                                         \
do                                                                                                 \
{                                                                                                  \
  auto eval_a = (LHS);                                                                             \
  auto eval_b = (RHS);                                                                             \
  ::tts::detail::check_relation(runtime,                                                           \
                                {__FILE__, __LINE__},                                              \
                                tts::detail::lt(eval_a, eval_b),                                   \
                                TTS_STRING(LHS),                                                   \
                                TTS_STRING(RHS),                                                   \
                                "<",                                                               \
                                ::tts::detail::to_string(eval_a),                                  \
                                ::tts::detail::to_string(eval_b));                                 \
} while( ::tts::detail::is_false() )                                                               \
/**/

//==================================================================================================
#define TTS_GREATER(LHS, RHS)                                                                      \
do                                                                                                 \
{                                                                                                  \
  auto eval_a = (LHS);                                                                             \
  auto eval_b = (RHS);                                                                             \
  ::tts::detail::check_relation(runtime,                                                           \
                                {__FILE__, __LINE__},                                              \
                                tts::detail::gt(eval_a, eval_b),                                   \
                                TTS_STRING(LHS),                                                   \
                                TTS_STRING(RHS),                                                   \
                                ">",                                                               \
                                ::tts::detail::to_string(eval_a),                                  \
                                ::tts::detail::to_string(eval_b));                                 \
} while( ::tts::detail::is_false() )                                                               \
/**/

//==================================================================================================
#define TTS_LESS_EQUAL(LHS, RHS)                                                                   \
do                                                                                                 \
{                                                                                                  \
  auto eval_a = (LHS);                                                                             \
  auto eval_b = (RHS);                                                                             \
  ::tts::detail::check_relation(runtime,                                                           \
                                {__FILE__, __LINE__},                                              \
                                tts::detail::le(eval_a, eval_b),                                   \
                                TTS_STRING(LHS),                                                   \
                                TTS_STRING(RHS),                                                   \
                                "<=",                                                              \
                                ::tts::detail::to_string(eval_a),                                  \
                                ::tts::detail::to_string(eval_b));                                 \
} while( ::tts::detail::is_false() )                                                               \
/**/

//==================================================================================================
#define TTS_GREATER_EQUAL(LHS, RHS)                                                                \
do                                                                                                 \
{                                                                                                  \
  auto eval_a = (LHS);                                                                             \
  auto eval_b = (RHS);                                                                             \
  ::tts::detail::check_relation(runtime,                                                           \
                                {__FILE__, __LINE__},                                              \
                                tts::detail::ge(eval_a, eval_b),                                   \
                                TTS_STRING(LHS),                                                   \
                                TTS_STRING(RHS),                                                   \
                                ">=",                                                              \
                                ::tts::detail::to_string(eval_a),                                  \
                                ::tts::detail::to_string(eval_b));                                 \
} while( ::tts::detail::is_false() )                                                               \
/**/

#endif
