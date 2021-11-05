//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/test/info.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/engine/logger.hpp>

#define TTS_TYPE_IS_IMPL(T, TYPE, FAILURE)                                                          \
[&]()                                                                                               \
{                                                                                                   \
  constexpr auto check = std::is_same_v<TTS_REMOVE_PARENS(TYPE), TTS_REMOVE_PARENS(T)>;             \
                                                                                                    \
  if constexpr(check)                                                                               \
  {                                                                                                 \
    TTS_PASS( ::tts::green  << TTS_STRING(TTS_REMOVE_PARENS(T)) << tts::reset                       \
                            << " evaluates as " << ::tts::green                                     \
                            << tts::typename_<TTS_REMOVE_PARENS(TYPE)>                              \
                            << ::tts::reset << " as expected.");                                    \
    return ::tts::logger{false};                                                                    \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE( ::tts::green  << TTS_STRING(TTS_REMOVE_PARENS(T)) << tts::reset                        \
                            << " evaluates as " << ::tts::red                                       \
                            << tts::typename_<TTS_REMOVE_PARENS(T)>                                 \
                            << ::tts::reset << " instead of "                                       \
                            << ::tts::green << tts::typename_<TTS_REMOVE_PARENS(TYPE)>              \
            );                                                                                      \
    return ::tts::logger{::tts::verbose_status};                                                    \
  }                                                                                                 \
}()
/**/

#define TTS_TYPE_IS(TYPE, REF)                                                                      \
[&]()                                                                                               \
{                                                                                                   \
  static_assert ( std::is_same_v<TTS_REMOVE_PARENS(TYPE),TTS_REMOVE_PARENS(REF)>                    \
                , "[TTS] - ** FAILURE** : " TTS_STRING(TTS_REMOVE_PARENS(TYPE))                     \
                  " is not the same as " TTS_STRING(TTS_REMOVE_PARENS(REF)) "."                     \
                );                                                                                  \
  ::tts::global_runtime.pass();                                                                     \
}()
/**/

#define TTS_EXPR_IS(EXPR, TYPE)                                                                     \
[&]()                                                                                               \
{                                                                                                   \
  static_assert ( std::is_same_v<decltype(TTS_REMOVE_PARENS(EXPR)),TTS_REMOVE_PARENS(TYPE)>         \
                , "[TTS] - ** FAILURE** : " TTS_STRING(TTS_REMOVE_PARENS(EXPR))                     \
                  " does not evaluates to an instance of " TTS_STRING(TTS_REMOVE_PARENS(TYPE)) "."  \
                );                                                                                  \
  ::tts::global_runtime.pass();                                                                     \
}()                                                                                                 \
/**/
