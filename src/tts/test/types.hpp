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

#define TTS_TYPE_IS(TYPE, REF)                                                                      \
{                                                                                                   \
  static_assert ( std::is_same_v<TTS_REMOVE_PARENS(TYPE),TTS_REMOVE_PARENS(REF)>                    \
                , "[TTS] - ** FAILURE** : " TTS_STRING(TTS_REMOVE_PARENS(TYPE))                     \
                  " is not the same as " TTS_STRING(TTS_REMOVE_PARENS(REF)) "."                     \
                );                                                                                  \
  ::tts::global_runtime.pass();                                                                     \
}
/**/

#define TTS_EXPR_IS(EXPR, TYPE)                                                                     \
{                                                                                                   \
  static_assert ( std::is_same_v<decltype(TTS_REMOVE_PARENS(EXPR)),TTS_REMOVE_PARENS(TYPE)>         \
                , "[TTS] - ** FAILURE** : " TTS_STRING(TTS_REMOVE_PARENS(EXPR))                     \
                  " does not evaluates to an instance of " TTS_STRING(TTS_REMOVE_PARENS(TYPE)) "."  \
                );                                                                                  \
  ::tts::global_runtime.pass();                                                                     \
}
/**/
