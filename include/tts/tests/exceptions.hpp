//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#ifndef TTS_TESTS_EXCEPTIONS_HPP_INCLUDED
#define TTS_TESTS_EXCEPTIONS_HPP_INCLUDED

#include <tts/detail/exceptions.hpp>
#include <tts/detail/infos.hpp>
#include <tts/detail/pp_helpers.hpp>
#include <typeinfo>

//==================================================================================================
#define TTS_THROW(Expr, Exception)                                                                 \
  do                                                                                               \
  {                                                                                                \
    ::tts::detail::throw_test(                                                                     \
        runtime         , {__FILE__, __LINE__},                                                    \
        TTS_STRING(Expr), TTS_STRING(Exception),                                                   \
        [] { Expr; }    , typeid(Exception).hash_code());                                          \
  } while(::tts::detail::is_false())
/**/

//==================================================================================================
#define TTS_NO_THROW(Expr)                                                                         \
  do                                                                                               \
  {                                                                                                \
    ::tts::detail::nothrow_test(runtime, {__FILE__, __LINE__}, TTS_STRING(Expr), [] { Expr; });    \
  } while(::tts::detail::is_false())
/**/

#endif
