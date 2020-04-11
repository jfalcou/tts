//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#ifndef TTS_TESTS_TYPES_HPP_INCLUDED
#define TTS_TESTS_TYPES_HPP_INCLUDED

#include <tts/detail/pp_helpers.hpp>
#include <tts/detail/type_id.hpp>
#include <tts/tests/basic.hpp>
#include <type_traits>

//==================================================================================================
#define TTS_TYPE_IS(T, Type)                                                                       \
  do                                                                                               \
  {                                                                                                \
    constexpr auto check = std::is_same_v<TTS_REMOVE_PARENS(Type), TTS_REMOVE_PARENS(T)>;          \
    if constexpr(check)                                                                            \
    {                                                                                              \
      TTS_PASS("Expecting " << TTS_STRING(TTS_REMOVE_PARENS(T)) << " to be "                       \
                            << ::tts::detail::green << tts::type_id<TTS_REMOVE_PARENS(Type)>()     \
                            << ::tts::detail::reset                                                \
              );                                                                                   \
    }                                                                                              \
                                                                                                   \
    if constexpr(!check)                                                                           \
    {                                                                                              \
      TTS_FAIL("Expecting " << TTS_STRING(TTS_REMOVE_PARENS(T)) << " to be "                       \
                            << tts::type_id<TTS_REMOVE_PARENS(Type)>() << " but found "            \
                            << ::tts::detail::red << tts::type_id<TTS_REMOVE_PARENS(T)>()          \
                            << ::tts::detail::reset << " instead"                                  \
              );                                                                                   \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

//==================================================================================================
#define TTS_EXPR_IS(Expression, Type)                                                              \
  TTS_TYPE_IS(decltype(TTS_REMOVE_PARENS(Expression)), Type)
/**/

#endif
