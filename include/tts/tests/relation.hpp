//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//===================================================================================================
#ifndef TTS_TESTS_RELATION_HPP_INCLUDED
#define TTS_TESTS_RELATION_HPP_INCLUDED

#include <tts/detail/comparator.hpp>
#include <tts/detail/pp_helpers.hpp>
#include <tts/detail/relations.hpp>

#define TTS_EQUAL(A, B)                                                                            \
  ::tts::detail::check_relation(runtime,                                                           \
                                tts::detail::eq((A), (B)),                                         \
                                TTS_STRING(A),                                                     \
                                TTS_STRING(B),                                                     \
                                "==",                                                              \
                                ::tts::detail::to_string(A),                                       \
                                ::tts::detail::to_string(B)) /**/

#define TTS_NOT_EQUAL(A, B)                                                                        \
  ::tts::detail::check_relation(runtime,                                                           \
                                tts::detail::neq((A), (B)),                                        \
                                TTS_STRING(A),                                                     \
                                TTS_STRING(B),                                                     \
                                "!=",                                                              \
                                ::tts::detail::to_string(A),                                       \
                                ::tts::detail::to_string(B)) /**/

#define TTS_LESS(A, B)                                                                             \
  ::tts::detail::check_relation(runtime,                                                           \
                                tts::detail::lt((A), (B)),                                         \
                                TTS_STRING(A),                                                     \
                                TTS_STRING(B),                                                     \
                                "<",                                                               \
                                ::tts::detail::to_string(A),                                       \
                                ::tts::detail::to_string(B)) /**/

#define TTS_GREATER(A, B)                                                                          \
  ::tts::detail::check_relation(runtime,                                                           \
                                tts::detail::gt((A), (B)),                                         \
                                TTS_STRING(A),                                                     \
                                TTS_STRING(B),                                                     \
                                ">",                                                               \
                                ::tts::detail::to_string(A),                                       \
                                ::tts::detail::to_string(B)) /**/

#define TTS_LESS_EQUAL(A, B)                                                                       \
  ::tts::detail::check_relation(runtime,                                                           \
                                tts::detail::le((A), (B)),                                         \
                                TTS_STRING(A),                                                     \
                                TTS_STRING(B),                                                     \
                                "<=",                                                              \
                                ::tts::detail::to_string(A),                                       \
                                ::tts::detail::to_string(B)) /**/

#define TTS_GREATER_EQUAL(A, B)                                                                    \
  ::tts::detail::check_relation(runtime,                                                           \
                                tts::detail::ge((A), (B)),                                         \
                                TTS_STRING(A),                                                     \
                                TTS_STRING(B),                                                     \
                                ">=",                                                              \
                                ::tts::detail::to_string(A),                                       \
                                ::tts::detail::to_string(B)) /**/

#endif
