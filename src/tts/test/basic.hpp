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
[&]()                                                                                               \
{                                                                                                   \
  ::tts::result tts_var_d = TTS_DECOMPOSE(EXPR);                                                    \
  if(tts_var_d)                                                                                     \
  {                                                                                                 \
    TTS_PASS( ::tts::green  << TTS_STRING(EXPR) << tts::reset                                       \
                            << " evaluates as " << ::tts::green                                     \
                            << tts_var_d.lhs << " " << tts_var_d.op << " " << tts_var_d.rhs         \
                            << ::tts::reset << " as expected.");                                    \
    return ::tts::logger{false};                                                                    \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(EXPR)  << tts::reset                      \
                            << " but " << ::tts::red                                                \
                            << tts_var_d.lhs << " " << tts_var_d.op << " " << tts_var_d.rhs         \
                            << ::tts::reset << " occured instead.");                                \
    return ::tts::logger{::tts::verbose_status};                                                    \
  }                                                                                                 \
}()
/**/

#define TTS_CONSTEXPR_EXPECT_IMPL(EXPR,FAILURE)                                                     \
[&]()                                                                                               \
{                                                                                                   \
  ::tts::result tts_var_d = TTS_DECOMPOSE(EXPR);                                                    \
  constexpr auto evaluation = std::bool_constant<(EXPR)>::value;                                    \
  if(evaluation)                                                                                    \
  {                                                                                                 \
    TTS_PASS( ::tts::green  << TTS_STRING(EXPR) << tts::reset                                       \
                            << " evaluates as " << ::tts::green                                     \
                            << tts_var_d.lhs << " " << tts_var_d.op << " " << tts_var_d.rhs         \
                            << ::tts::reset << " at compile-time as expected.");                    \
    return ::tts::logger{false};                                                                    \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(EXPR)  << tts::reset                      \
                            << " but " << ::tts::red                                                \
                            << tts_var_d.lhs << " " << tts_var_d.op << " " << tts_var_d.rhs         \
                            << ::tts::reset << " occured at compile-time instead.");                \
    return ::tts::logger{::tts::verbose_status};                                                    \
  }                                                                                                 \
}()
/**/

#define TTS_EXPECT(EXPR, ...)     TTS_EXPECT_ ## __VA_ARGS__ ( EXPR )
#define TTS_EXPECT_(EXPR)         TTS_EXPECT_IMPL(EXPR,TTS_FAIL)
#define TTS_EXPECT_REQUIRED(EXPR) TTS_EXPECT_IMPL(EXPR,TTS_FATAL)

#define TTS_CONSTEXPR_EXPECT(EXPR, ...)     TTS_CONSTEXPR_EXPECT_ ## __VA_ARGS__ ( EXPR )
#define TTS_CONSTEXPR_EXPECT_(EXPR)         TTS_CONSTEXPR_EXPECT_IMPL(EXPR,TTS_FAIL)
#define TTS_CONSTEXPR_EXPECT_REQUIRED(EXPR) TTS_CONSTEXPR_EXPECT_IMPL(EXPR,TTS_FATAL)

#define TTS_EXPECT_NOT_IMPL(EXPR,FAILURE)                                                           \
[&]()                                                                                               \
{                                                                                                   \
  ::tts::result tts_var_d = TTS_DECOMPOSE(EXPR);                                                    \
  if(tts_var_d)                                                                                     \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(EXPR) << tts::reset                       \
                            << " to not evaluate to " << ::tts::red                                 \
                            << tts_var_d.lhs << " " << tts_var_d.op << " " << tts_var_d.rhs         \
                            << ::tts::reset << " but occured anyway."                               \
                            );                                                                      \
    return ::tts::logger{::tts::verbose_status};                                                    \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    TTS_PASS ( ::tts::green << TTS_STRING(EXPR) << tts::reset                                       \
                            << " does not evaluate to " << ::tts::green                             \
                            << tts_var_d.lhs << " " << tts_var_d.op << " " << tts_var_d.rhs         \
                            << ::tts::reset << " as expected.");                                    \
    return ::tts::logger{false};                                                                    \
  }                                                                                                 \
}()
/**/

#define TTS_CONSTEXPR_EXPECT_NOT_IMPL(EXPR,FAILURE)                                                 \
[&]()                                                                                               \
{                                                                                                   \
  ::tts::result tts_var_d = TTS_DECOMPOSE(EXPR);                                                    \
  constexpr auto evaluation = std::bool_constant<(EXPR)>::value;                                    \
  if(evaluation)                                                                                    \
  {                                                                                                 \
    FAILURE ( "Expected: "  << ::tts::green << TTS_STRING(EXPR) << tts::reset                       \
                            << " to not evaluate to " << ::tts::red                                 \
                            << tts_var_d.lhs << " " << tts_var_d.op << " " << tts_var_d.rhs         \
                            << ::tts::reset << " at compile-time but occured anyway."               \
                            );                                                                      \
    return ::tts::logger{::tts::verbose_status};                                                    \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    TTS_PASS ( ::tts::green << TTS_STRING(EXPR) << tts::reset                                       \
                            << " does not evaluate to " << ::tts::green                             \
                            << tts_var_d.lhs << " " << tts_var_d.op << " " << tts_var_d.rhs         \
                            << ::tts::reset << " at compile-time as expected.");                    \
    return ::tts::logger{false};                                                                    \
  }                                                                                                 \
}()
/**/

#define TTS_EXPECT_NOT(EXPR, ...)     TTS_EXPECT_NOT_ ## __VA_ARGS__ ( EXPR )
#define TTS_EXPECT_NOT_(EXPR)         TTS_EXPECT_NOT_IMPL((EXPR),TTS_FAIL)
#define TTS_EXPECT_NOT_REQUIRED(EXPR) TTS_EXPECT_NOT_IMPL((EXPR),TTS_FATAL)

#define TTS_CONSTEXPR_EXPECT_NOT(EXPR, ...)     TTS_CONSTEXPR_EXPECT_NOT_ ## __VA_ARGS__ ( EXPR )
#define TTS_CONSTEXPR_EXPECT_NOT_(EXPR)         TTS_CONSTEXPR_EXPECT_NOT_IMPL((EXPR),TTS_FAIL)
#define TTS_CONSTEXPR_EXPECT_NOT_REQUIRED(EXPR) TTS_CONSTEXPR_EXPECT_NOT_IMPL((EXPR),TTS_FATAL)
