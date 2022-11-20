//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <tts/test/info.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/engine/logger.hpp>

#define TTS_THROW_IMPL(EXPR, EXCEPTION, FAILURE)                                                    \
[&]()                                                                                               \
{                                                                                                   \
  bool tts_caught = false;                                                                          \
                                                                                                    \
  try                 { EXPR; }                                                                     \
  catch(EXCEPTION&  ) { tts_caught = true; }                                                        \
  catch(...)          { }                                                                           \
                                                                                                    \
  if(tts_caught)                                                                                    \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: " << TTS_STRING(EXPR) << " failed to throw " << TTS_STRING(EXCEPTION) );   \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}()
/**/

#define TTS_THROW(EXPR, EXCEPTION, ...)     TTS_THROW_ ## __VA_ARGS__ ( EXPR, EXCEPTION )
#define TTS_THROW_(EXPR, EXCEPTION)         TTS_THROW_IMPL(EXPR, EXCEPTION,TTS_FAIL)
#define TTS_THROW_REQUIRED(EXPR, EXCEPTION) TTS_THROW_IMPL(EXPR, EXCEPTION,TTS_FATAL)

#define TTS_NO_THROW_IMPL(EXPR,FAILURE)                                                             \
[&]()                                                                                               \
{                                                                                                   \
  bool tts_caught = false;                                                                          \
                                                                                                    \
  try        { EXPR; }                                                                              \
  catch(...) { tts_caught = true; }                                                                 \
                                                                                                    \
  if(!tts_caught)                                                                                   \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::logger{false};                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: "  << TTS_STRING(EXPR) << " throws unexpectedly." );                       \
    return ::tts::logger{};                                                                         \
  }                                                                                                 \
}()
/**/

#define TTS_NO_THROW(EXPR, ...)     TTS_NO_THROW_ ## __VA_ARGS__ ( EXPR )
#define TTS_NO_THROW_(EXPR)         TTS_NO_THROW_IMPL(EXPR,TTS_FAIL)
#define TTS_NO_THROW_REQUIRED(EXPR) TTS_NO_THROW_IMPL(EXPR,TTS_FATAL)
