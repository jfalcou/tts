//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/preprocessor.hpp>
#include <tts/engine/info.hpp>
#include <tts/engine/logger.hpp>

#define TTS_THROW_IMPL(EXPR, EXCEPTION, FAILURE)                                                   \
  [ & ]()                                                                                          \
  {                                                                                                \
    bool tts_caught = false;                                                                       \
                                                                                                   \
    try                                                                                            \
    {                                                                                              \
      EXPR;                                                                                        \
    }                                                                                              \
    catch(EXCEPTION &)                                                                             \
    {                                                                                              \
      tts_caught = true;                                                                           \
    }                                                                                              \
    catch(...)                                                                                     \
    {                                                                                              \
    }                                                                                              \
                                                                                                   \
    if(tts_caught)                                                                                 \
    {                                                                                              \
      TTS_PASS("Expected: %s throws %s.",                                                          \
               TTS_STRING(TTS_REMOVE_PARENS(EXPR)),                                                \
               TTS_STRING(TTS_REMOVE_PARENS(EXCEPTION)));                                          \
      ::tts::global_runtime.pass();                                                                \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expected: %s failed to throw %s.",                                                  \
              TTS_STRING(TTS_REMOVE_PARENS(EXPR)),                                                 \
              TTS_STRING(TTS_REMOVE_PARENS(EXCEPTION)));                                           \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }()
/**/

//======================================================================================================================
/**
  @defgroup test-exceptions Exception Tests Macros
  @brief Macros for performing checks related to exceptions handling.
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @def TTS_THROW
  @brief Checks if a given expression throws an exception of a given type

  @param EXPR       Expression to evaluate.
  @param EXCEPTION  Expected exception type to be thrown.
  @param ...        Optional tag. If equals to `REQUIRED`, this test will stop the program if it
fails.

  @groupheader{Example}
  @snippet doc/throw.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_THROW(EXPR, EXCEPTION, ...)
#else
#  define TTS_THROW(EXPR, EXCEPTION, ...) TTS_THROW_##__VA_ARGS__(EXPR, EXCEPTION)
#endif

#define TTS_THROW_(EXPR, EXCEPTION) TTS_THROW_IMPL(EXPR, EXCEPTION, TTS_FAIL)
#define TTS_THROW_REQUIRED(EXPR, EXCEPTION) TTS_THROW_IMPL(EXPR, EXCEPTION, TTS_FATAL)

#define TTS_NO_THROW_IMPL(EXPR, FAILURE)                                                           \
  [ & ]()                                                                                          \
  {                                                                                                \
    bool tts_caught = false;                                                                       \
                                                                                                   \
    try                                                                                            \
    {                                                                                              \
      EXPR;                                                                                        \
    }                                                                                              \
    catch(...)                                                                                     \
    {                                                                                              \
      tts_caught = true;                                                                           \
    }                                                                                              \
                                                                                                   \
    if(!tts_caught)                                                                                \
    {                                                                                              \
      TTS_PASS("Expected: %s does not throw.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));               \
      ::tts::global_runtime.pass();                                                                \
      return ::tts::_::logger {false};                                                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      FAILURE("Expected: %s throws unexpectedly.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));           \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
  }()
/**/

//======================================================================================================================
/**
  @def TTS_NO_THROW
  @brief Checks if a given expression throws no exception

  @param EXPR       Expression to evaluate.
  @param ...        Optional tag. If equals to `REQUIRED`, this test will stop the program if it
fails.

  @groupheader{Example}
  @snippet doc/no_throw.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_NO_THROW(EXPR, ...)
#else
#  define TTS_NO_THROW(EXPR, ...) TTS_NO_THROW_##__VA_ARGS__(EXPR)
#endif

#define TTS_NO_THROW_(EXPR) TTS_NO_THROW_IMPL(EXPR, TTS_FAIL)
#define TTS_NO_THROW_REQUIRED(EXPR) TTS_NO_THROW_IMPL(EXPR, TTS_FATAL)

//======================================================================================================================
/**
  @}
**/
//======================================================================================================================
