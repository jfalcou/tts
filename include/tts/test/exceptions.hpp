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
#include <tts/engine/check.hpp>

namespace tts::_
{
  inline logger check_throw(bool caught, check_site const& site)
  {
    if(caught)
    {
      if(check_passed())
        check_report(site, ::tts::text {"Expected: %s throws %s.", site.lhs, site.rhs});
      ::tts::global_runtime.pass(); // counted twice, as in the published totals
      return logger {false};
    }

    return check_failed(site, ::tts::text {"Expected: %s failed to throw %s.", site.lhs, site.rhs});
  }

  inline logger check_no_throw(bool caught, check_site const& site)
  {
    if(!caught)
    {
      if(check_passed()) check_report(site, ::tts::text {"Expected: %s does not throw.", site.lhs});
      ::tts::global_runtime.pass(); // counted twice, as in the published totals
      return logger {false};
    }

    return check_failed(site, ::tts::text {"Expected: %s throws unexpectedly.", site.lhs});
  }
}

#define TTS_THROW_IMPL(EXPR, EXCEPTION, FAILURE)                                                   \
  ::tts::_::check_throw(                                                                           \
  [ & ]() -> bool                                                                                  \
  {                                                                                                \
    try                                                                                            \
    {                                                                                              \
      EXPR;                                                                                        \
    }                                                                                              \
    catch(EXCEPTION&)                                                                              \
    {                                                                                              \
      return true;                                                                                 \
    }                                                                                              \
    catch(...)                                                                                     \
    {                                                                                              \
    }                                                                                              \
    return false;                                                                                  \
  }(),                                                                                             \
  TTS_CHECK_SITE(                                                                                  \
  FAILURE, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", TTS_STRING(TTS_REMOVE_PARENS(EXCEPTION)), ""))
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
#define TTS_THROW(EXPR, EXCEPTION, ...)
#else
#define TTS_THROW(EXPR, EXCEPTION, ...) TTS_THROW_##__VA_ARGS__(EXPR, EXCEPTION)
#endif

#define TTS_THROW_(EXPR, EXCEPTION)         TTS_THROW_IMPL(EXPR, EXCEPTION, TTS_FAIL)
#define TTS_THROW_REQUIRED(EXPR, EXCEPTION) TTS_THROW_IMPL(EXPR, EXCEPTION, TTS_FATAL)

#define TTS_NO_THROW_IMPL(EXPR, FAILURE)                                                           \
  ::tts::_::check_no_throw(                                                                        \
  [ & ]() -> bool                                                                                  \
  {                                                                                                \
    try                                                                                            \
    {                                                                                              \
      EXPR;                                                                                        \
    }                                                                                              \
    catch(...)                                                                                     \
    {                                                                                              \
      return true;                                                                                 \
    }                                                                                              \
    return false;                                                                                  \
  }(),                                                                                             \
  TTS_CHECK_SITE(FAILURE, TTS_STRING(TTS_REMOVE_PARENS(EXPR)), "", "", ""))
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
#define TTS_NO_THROW(EXPR, ...)
#else
#define TTS_NO_THROW(EXPR, ...) TTS_NO_THROW_##__VA_ARGS__(EXPR)
#endif

#define TTS_NO_THROW_(EXPR)         TTS_NO_THROW_IMPL(EXPR, TTS_FAIL)
#define TTS_NO_THROW_REQUIRED(EXPR) TTS_NO_THROW_IMPL(EXPR, TTS_FATAL)

//======================================================================================================================
/**
  @}
**/
//======================================================================================================================
