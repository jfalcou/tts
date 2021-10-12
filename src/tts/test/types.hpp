//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/test/info.hpp>
#include <tts/test/decomposer.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/engine/logger.hpp>

#define TTS_TYPE_IS_IMPL(T, TYPE, FAILURE)                                                          \
::tts::logger{}.check                                                                               \
( ::tts::result{}                                                                                   \
, [](auto const&) { return std::is_same_v<TTS_REMOVE_PARENS(TYPE), TTS_REMOVE_PARENS(T)>; }         \
, [](auto const& res)                                                                               \
  {                                                                                                 \
    TTS_PASS( ::tts::green  << TTS_STRING(TTS_REMOVE_PARENS(T)) << tts::reset                       \
                            << " evaluates as " << ::tts::green                                     \
                            << tts::typename_<TTS_REMOVE_PARENS(TYPE)>                              \
                            << ::tts::reset << " as expected.");                                    \
    return false;                                                                                   \
  }                                                                                                 \
, [](auto const& res)                                                                               \
  {                                                                                                 \
    FAILURE( ::tts::green  << TTS_STRING(TTS_REMOVE_PARENS(T)) << tts::reset                        \
                            << " evaluates as " << ::tts::red                                       \
                            << tts::typename_<TTS_REMOVE_PARENS(T)>                                 \
                            << ::tts::reset << " instead of "                                       \
                            << ::tts::green << tts::typename_<TTS_REMOVE_PARENS(TYPE)>              \
            );                                                                                      \
    return ::tts::verbose_status;                                                                   \
  }                                                                                                 \
)                                                                                                   \
/**/

#define TTS_TYPE_IS(T, TYPE, ...)     TTS_TYPE_IS_ ## __VA_ARGS__ ( T, TYPE )
#define TTS_TYPE_IS_(T, TYPE)         TTS_TYPE_IS_IMPL(T, TYPE,TTS_FAIL)
#define TTS_TYPE_IS_REQUIRED(T, TYPE) TTS_TYPE_IS_IMPL(T, TYPE,TTS_FATAL)

#define TTS_EXPR_IS(EXPR, TYPE, ...)  TTS_TYPE_IS(decltype(TTS_REMOVE_PARENS(EXPR)), TYPE, __VA_ARGS__)
