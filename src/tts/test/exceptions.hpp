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
    TTS_PASS( ::tts::green  << TTS_STRING(EXPR) << tts::reset                                       \
                            << " throws: " << ::tts::green                                          \
                            << TTS_STRING(EXCEPTION)                                                \
                            << ::tts::reset << " as expected."                                      \
            );                                                                                      \
    return ::tts::logger{false};                                                                    \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(EXPR)  << tts::reset                      \
                            << " failed to throw " << ::tts::red                                    \
                            << TTS_STRING(EXCEPTION)                                                \
            );                                                                                      \
    return ::tts::logger{::tts::verbose_status};                                                    \
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
    TTS_PASS( ::tts::green  << TTS_STRING(EXPR) << tts::reset                                       \
                            << " does not throw as expected."                                       \
            );                                                                                      \
    return ::tts::logger{false};                                                                    \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::red << TTS_STRING(EXPR)  << tts::reset                        \
                            << " throws unexpectedly."                                              \
            );                                                                                      \
    return ::tts::logger{::tts::verbose_status};                                                    \
  }                                                                                                 \
}()
/**/

#define TTS_NO_THROW(EXPR, ...)     TTS_NO_THROW_ ## __VA_ARGS__ ( EXPR )
#define TTS_NO_THROW_(EXPR)         TTS_NO_THROW_IMPL(EXPR,TTS_FAIL)
#define TTS_NO_THROW_REQUIRED(EXPR) TTS_NO_THROW_IMPL(EXPR,TTS_FATAL)
