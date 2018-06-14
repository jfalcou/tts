//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_EXCEPTIONS_HPP_INCLUDED
#define TTS_TESTS_EXCEPTIONS_HPP_INCLUDED

#include <tts/detail/pp_helpers.hpp>
#include <tts/tests/basic.hpp>

/*!
  @ingroup group-unit
  Test if an expression @c X throws an exception of type @c T.

  @par Usage:

  @snippet test/unit/exceptions.cpp throw

  where @c foo is defined as :

  @snippet test/unit/exceptions.cpp throwing_foo

  @param X Expression to test
  @param T Exception type expected to be thrown
**/
#define TTS_THROW( X, T )                                                                           \
do                                                                                                  \
{                                                                                                   \
  bool caught = false;                                                                              \
  try                            { (void)(TTS_REMOVE_PARENS(X)); }                                  \
  catch( TTS_REMOVE_PARENS(T)& ) { caught = true; }                                                 \
                                                                                                    \
  if(caught)                                                                                        \
    TTS_PASS( TTS_STRING(X) <<  " throws " <<  TTS_STRING(T) );                                     \
  else                                                                                              \
    TTS_FAIL( TTS_STRING(X) <<  " does not throw " <<  TTS_STRING(T) );                             \
} while( ::tts::detail::is_false() )                                                                \
/**/

/*!
  @ingroup group-unit
  Test if an expression @c X does not throw any exception.

  @par Usage:

  @snippet test/unit/exceptions.cpp no_throw

  where @c foo is defined as :

  @snippet test/unit/exceptions.cpp throwing_foo

  @param X Expression to test
**/
#define TTS_NO_THROW( X )                                                                           \
do                                                                                                  \
{                                                                                                   \
  bool caught = false;                                                                              \
  try          { (void)(TTS_REMOVE_PARENS(X)); }                                                    \
  catch( ... ) { caught = true; }                                                                   \
                                                                                                    \
  if(caught)                                                                                        \
    TTS_FAIL( TTS_STRING(X) << " throws while not expected to." );                                  \
  else                                                                                              \
    TTS_PASS( TTS_STRING(X) << " doesn't throw." );                                                 \
} while( ::tts::detail::is_false() )                                                                \
/**/

#endif
