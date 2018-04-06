//==================================================================================================
/*!
  @file

  Defines test macros for test on types

  @copyright 2018 Joel FALCOU


  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

**/
//==================================================================================================
#ifndef TTS_TESTS_TYPES_HPP_INCLUDED
#define TTS_TESTS_TYPES_HPP_INCLUDED

#include <tts/detail/pp_helpers.hpp>
#include <tts/detail/pp_helpers.hpp>
#include <tts/detail/type_id.hpp>
#include <tts/tests/basic.hpp>
#include <type_traits>

/*!
  @ingroup group-unit
  @brief Check for type equality

  For a given types @c T and  @c Type, consider the test successful if and only if
  @c std::is_same<T,Type> evaluates to @true_.

  @par Usage:

  @snippet test/unit/types.cpp type_is

  @param T    First type to compare
  @param Type Second type to compare
**/
#define TTS_TYPE_IS(T, Type)                                                                        \
do                                                                                                  \
{                                                                                                   \
  volatile bool tts_var_b = std::is_same< TTS_REMOVE_PARENS(Type)                                 \
                                          , TTS_REMOVE_PARENS(T)                                    \
                                          >::value;                                                 \
  if( tts_var_b )                                                                                 \
    TTS_PASS (   "Expecting " << TTS_STRING(TTS_REMOVE_PARENS(T))                                   \
                <<  " == " << tts::type_id<TTS_REMOVE_PARENS(Type)>()                               \
                );                                                                                  \
  else                                                                                              \
    TTS_FAIL (   "Expecting " << TTS_STRING(TTS_REMOVE_PARENS(T))                                   \
                <<  " == " << tts::type_id<TTS_REMOVE_PARENS(Type)>()                               \
                <<  " found " << tts::type_id<TTS_REMOVE_PARENS(T)>()                               \
                <<  " instead"                                                                      \
                );                                                                                  \
} while( ::tts::detail::is_false() )                                                                \
/**/

/*!
  @ingroup group-unit
  @brief Check for type equality

  For a given expression @c Expression and a given type @c Type, consider the test successful if
  and only if @c std::is_same<decltype(Expression),Type> evaluates to @true_ .

  @par Usage:

  @snippet test/unit/types.cpp expr_is

  @param Expression Expression to test type
  @param Type       Second type to compare
**/
#define TTS_EXPR_IS(Expression, Type)                                                               \
TTS_TYPE_IS(decltype( TTS_REMOVE_PARENS(Expression)), Type)                                         \
/**/

#endif
