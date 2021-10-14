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

#define TTS_EXPECT_IMPL(EXPR,FAILURE)                                                               \
::tts::logger{}.check                                                                               \
( TTS_DECOMPOSE(EXPR)                                                                               \
, [](tts::result const& res)                                                                        \
  {                                                                                                 \
    TTS_PASS( ::tts::green  << TTS_STRING(EXPR) << tts::reset                                       \
                            << " evaluates as " << ::tts::green                                     \
                            << res.lhs << " " << res.op << " " << res.rhs                           \
                            << ::tts::reset << " as expected.");                                    \
    return false;                                                                                   \
  }                                                                                                 \
, [](tts::result const& res)                                                                        \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(EXPR)  << tts::reset                      \
                            << " but " << ::tts::red                                                \
                            << res.lhs << " " << res.op << " " << res.rhs                           \
                            << ::tts::reset << " occured instead.");                                \
    return ::tts::verbose_status;                                                                   \
  }                                                                                                 \
)                                                                                                   \
/**/

#define TTS_CONSTEXPR_EXPECT_IMPL(EXPR,FAILURE)                                                     \
::tts::logger{}.check                                                                               \
( ((void)(std::bool_constant<(EXPR)>::value), TTS_DECOMPOSE(EXPR))                                  \
, [](::tts::result const& res)                                                                      \
  {                                                                                                 \
    TTS_PASS( ::tts::green  << TTS_STRING(EXPR) << tts::reset                                       \
                            << " evaluates as " << ::tts::green                                     \
                            << res.lhs << " " << res.op << " " << res.rhs                           \
                            << ::tts::reset << " at compile-time as expected.");                    \
    return false;                                                                                   \
  }                                                                                                 \
, [](tts::result const& res)                                                                        \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(EXPR)  << tts::reset                      \
                            << " but " << ::tts::red                                                \
                            << res.lhs << " " << res.op << " " << res.rhs                           \
                            << ::tts::reset << " occured at compile-time instead.");                \
    return ::tts::verbose_status;                                                                   \
  }                                                                                                 \
)                                                                                                   \
/**/

#define TTS_EXPECT(EXPR, ...)     TTS_EXPECT_ ## __VA_ARGS__ ( EXPR )
#define TTS_EXPECT_(EXPR)         TTS_EXPECT_IMPL(EXPR,TTS_FAIL)
#define TTS_EXPECT_REQUIRED(EXPR) TTS_EXPECT_IMPL(EXPR,TTS_FATAL)

#define TTS_CONSTEXPR_EXPECT(EXPR, ...)     TTS_CONSTEXPR_EXPECT_ ## __VA_ARGS__ ( EXPR )
#define TTS_CONSTEXPR_EXPECT_(EXPR)         TTS_CONSTEXPR_EXPECT_IMPL(EXPR,TTS_FAIL)
#define TTS_CONSTEXPR_EXPECT_REQUIRED(EXPR) TTS_CONSTEXPR_EXPECT_IMPL(EXPR,TTS_FATAL)

#define TTS_EXPECT_NOT_IMPL(EXPR,FAILURE)                                                           \
::tts::logger{}.check                                                                               \
( TTS_DECOMPOSE(EXPR)                                                                               \
, [](::tts::result res)                                                                             \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(EXPR) << tts::reset                       \
                            << " to not evaluate to " << ::tts::red                                 \
                            << res.lhs << " " << res.op << " " << res.rhs                           \
                            << ::tts::reset << " but occured anyway."                               \
                            );                                                                      \
    return ::tts::verbose_status;                                                                   \
  }                                                                                                 \
, [](::tts::result res)                                                                             \
  {                                                                                                 \
    TTS_PASS ( ::tts::green << TTS_STRING(EXPR) << tts::reset                                       \
                            << " does not evaluate to " << ::tts::green                             \
                            << res.lhs << " " << res.op << " " << res.rhs                           \
                            << ::tts::reset << " as expected.");                                    \
    return false;                                                                                   \
  }                                                                                                 \
)                                                                                                   \
/**/

#define TTS_CONSTEXPR_EXPECT_NOT_IMPL(EXPR,FAILURE)                                                 \
::tts::logger{}.check                                                                               \
( ((void)(std::bool_constant<(EXPR)>::value), TTS_DECOMPOSE(EXPR))                                  \
, [](tts::result const& res)                                                                        \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(EXPR) << tts::reset                       \
                            << " to not evaluate to " << ::tts::red                                 \
                            << res.lhs << " " << res.op << " " << res.rhs                           \
                            << ::tts::reset << " at compile-time but occured anyway."               \
                            );                                                                      \
    return ::tts::verbose_status;                                                                   \
  }                                                                                                 \
, [](tts::result const& res)                                                                        \
  {                                                                                                 \
    TTS_PASS ( ::tts::green << TTS_STRING(EXPR) << tts::reset                                       \
                            << " does not evaluate to " << ::tts::green                             \
                            << res.lhs << " " << res.op << " " << res.rhs                           \
                            << ::tts::reset << " at compile-time as expected.");                    \
    return false;                                                                                   \
  }                                                                                                 \
)                                                                                                   \
/**/

#define TTS_EXPECT_NOT(EXPR, ...)     TTS_EXPECT_NOT_ ## __VA_ARGS__ ( EXPR )
#define TTS_EXPECT_NOT_(EXPR)         TTS_EXPECT_NOT_IMPL((EXPR),TTS_FAIL)
#define TTS_EXPECT_NOT_REQUIRED(EXPR) TTS_EXPECT_NOT_IMPL((EXPR),TTS_FATAL)

#define TTS_CONSTEXPR_EXPECT_NOT(EXPR, ...)     TTS_CONSTEXPR_EXPECT_NOT_ ## __VA_ARGS__ ( EXPR )
#define TTS_CONSTEXPR_EXPECT_NOT_(EXPR)         TTS_CONSTEXPR_EXPECT_NOT_IMPL((EXPR),TTS_FAIL)
#define TTS_CONSTEXPR_EXPECT_NOT_REQUIRED(EXPR) TTS_CONSTEXPR_EXPECT_NOT_IMPL((EXPR),TTS_FATAL)
