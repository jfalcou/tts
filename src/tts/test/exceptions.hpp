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

#define TTS_THROW_IMPL(EXPR, EXCEPTION, FAILURE)                                                    \
::tts::logger{}.check                                                                               \
( ::tts::result{}                                                                                   \
, [&](auto const&) { try { EXPR; } catch(EXCEPTION&) { return true; } catch(...) {} return false; } \
, [](auto const& res)                                                                               \
  {                                                                                                 \
    TTS_PASS( ::tts::green  << TTS_STRING(EXPR) << tts::reset                                       \
                            << " throws: " << ::tts::green                                          \
                            << TTS_STRING(EXCEPTION)                                                \
                            << ::tts::reset << " as expected."                                      \
            );                                                                                      \
    return false;                                                                                   \
  }                                                                                                 \
, [](auto const& res)                                                                               \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(EXPR)  << tts::reset                      \
                            << " failed to throw " << ::tts::red                                    \
                            << TTS_STRING(EXCEPTION)                                                \
            );                                                                                      \
    return ::tts::verbose_status;                                                                   \
  }                                                                                                 \
)                                                                                                   \
/**/

#define TTS_THROW(EXPR, EXCEPTION, ...)     TTS_THROW_ ## __VA_ARGS__ ( EXPR, EXCEPTION )
#define TTS_THROW_(EXPR, EXCEPTION)         TTS_THROW_IMPL(EXPR, EXCEPTION,TTS_FAIL)
#define TTS_THROW_REQUIRED(EXPR, EXCEPTION) TTS_THROW_IMPL(EXPR, EXCEPTION,TTS_FATAL)

#define TTS_NO_THROW_IMPL(EXPR,FAILURE)                                                             \
::tts::logger{}.check                                                                               \
( ::tts::result{}                                                                                   \
, [&](auto const&) { try { EXPR; } catch(...) { return false; } return true; }                      \
, [](auto const& res)                                                                               \
  {                                                                                                 \
    TTS_PASS( ::tts::green  << TTS_STRING(EXPR) << tts::reset                                       \
                            << " does not throw as expected."                                       \
            );                                                                                      \
    return false;                                                                                   \
  }                                                                                                 \
, [](auto const& res)                                                                               \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::red << TTS_STRING(EXPR)  << tts::reset                        \
                            << " throws unexpectedly."                                              \
            );                                                                                      \
    return ::tts::verbose_status;                                                                   \
  }                                                                                                 \
)                                                                                                   \
/**/

#define TTS_NO_THROW(EXPR, ...)     TTS_NO_THROW_ ## __VA_ARGS__ ( EXPR )
#define TTS_NO_THROW_(EXPR)         TTS_NO_THROW_IMPL(EXPR,TTS_FAIL)
#define TTS_NO_THROW_REQUIRED(EXPR) TTS_NO_THROW_IMPL(EXPR,TTS_FATAL)
