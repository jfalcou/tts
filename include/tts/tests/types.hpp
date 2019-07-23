//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_TYPES_HPP_INCLUDED
#define TTS_TESTS_TYPES_HPP_INCLUDED

#include <tts/detail/pp_helpers.hpp>
#include <tts/detail/type_id.hpp>
#include <tts/tests/basic.hpp>
#include <type_traits>

/*!
  @ingroup group-unit
  @brief Check for type equality
  For a given types @c T and  @c Type, consider the test successful if and only if
  @c std::is_same<T,Type> evaluates to @true_.
  @param Type Second type to compare
**/
#define TTS_TYPE_IS(T, Type)                                                                       \
  do                                                                                               \
  {                                                                                                \
    constexpr auto check = std::is_same_v<TTS_REMOVE_PARENS(Type), TTS_REMOVE_PARENS(T)>;          \
    if constexpr(check)                                                                            \
    {                                                                                              \
      TTS_PASS("Expecting " << ::tts::detail::white_(TTS_STRING(TTS_REMOVE_PARENS(T))) << " == "   \
                            << ::tts::detail::green_(tts::type_id<TTS_REMOVE_PARENS(Type)>()));    \
    }                                                                                              \
                                                                                                   \
    if constexpr(!check)                                                                           \
    {                                                                                              \
      TTS_FAIL("Expecting " << ::tts::detail::white_(tts::type_id<TTS_REMOVE_PARENS(Type)>())      \
                            << " found "                                                           \
                            << ::tts::detail::red_(tts::type_id<TTS_REMOVE_PARENS(T)>())           \
          << " instead");                                                                          \
    }                                                                                              \
  } while(::tts::detail::is_false()) /**/

/*!
  @ingroup group-unit
  @brief Check for type equality
  For a given expression @c Expression and a given type @c Type, consider the test successful if
  and only if @c std::is_same<decltype(Expression),Type> evaluates to @true_ .
**/
#define TTS_EXPR_IS(Expression, Type)                                                              \
  TTS_TYPE_IS(decltype(TTS_REMOVE_PARENS(Expression)), Type)                                       \
  /**/

#endif
