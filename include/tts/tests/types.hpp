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
/**
  * @brief Check for type equivalence
  *
  * **Required header:** `#include <tts/tests/types.hpp>`
  *
  * Checks if two types satisfy `std::is_same_v<T1,T2> == true`.
  *
  * **Example:**
  *
  * @snippet types.cpp type_is
  *
  *  @param T1 Type to compare
  *  @param T2 Type to compare
**/
//==================================================================================================
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
  } while(::tts::detail::is_false())
/**/

//==================================================================================================
/**
  * @brief Check for the type of an expression
  *
  * **Required header:** `#include <tts/tests/types.hpp>`
  *
  * Checks if an expression evaluates to a given type.
  *
  * **Example:**
  *
  * @snippet types.cpp expr_is
  *
  *  @param Expression  Expression to evaluates
  *  @param Type        Expected expression type
**/
//==================================================================================================
#define TTS_EXPR_IS(Expression, Type)                                                              \
  TTS_TYPE_IS(decltype(TTS_REMOVE_PARENS(Expression)), Type)
/**/

#endif
