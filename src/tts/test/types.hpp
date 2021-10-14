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

namespace tts
{
  template<typename Type, typename Expected>
  struct type_result
  {
    std::string expr, type, expected;
    explicit operator bool() const { return std::is_same_v<Type, Expected>; }
  };
}

#define TTS_TYPE_IS_IMPL(T, TYPE, FAILURE)                                                          \
::tts::logger{}.check                                                                               \
( ::tts::type_result<TTS_REMOVE_PARENS(TYPE), TTS_REMOVE_PARENS(T)>                                 \
  { TTS_STRING(TTS_REMOVE_PARENS(T))                                                                \
  , std::string{tts::typename_<TTS_REMOVE_PARENS(T)>}                                               \
  , std::string{tts::typename_<TTS_REMOVE_PARENS(TYPE)>}                                            \
  }                                                                                                 \
, [](auto const& res)                                                                               \
  {                                                                                                 \
    TTS_PASS( ::tts::green  << res.expr << tts::reset                                               \
                            << " evaluates as " << ::tts::green << res.type                         \
                            << ::tts::reset << " as expected.");                                    \
    return false;                                                                                   \
  }                                                                                                 \
, [](auto const& res)                                                                               \
  {                                                                                                 \
    FAILURE( ::tts::green  << res.expr << tts::reset                                                \
                            << " evaluates as " << ::tts::red << res.type                           \
                            << ::tts::reset << " instead of "                                       \
                            << ::tts::green << res.expected                                         \
            );                                                                                      \
    return ::tts::verbose_status;                                                                   \
  }                                                                                                 \
)                                                                                                   \
/**/

#define TTS_TYPE_IS(T, TYPE, ...)     TTS_TYPE_IS_ ## __VA_ARGS__ ( T, TYPE )
#define TTS_TYPE_IS_(T, TYPE)         TTS_TYPE_IS_IMPL(T, TYPE,TTS_FAIL)
#define TTS_TYPE_IS_REQUIRED(T, TYPE) TTS_TYPE_IS_IMPL(T, TYPE,TTS_FATAL)

#define TTS_EXPR_IS(EXPR, TYPE, ...)  TTS_TYPE_IS(decltype(TTS_REMOVE_PARENS(EXPR)), TYPE, __VA_ARGS__)
