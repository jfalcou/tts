//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/comparators.hpp>
#include <tts/tools/as_text.hpp>
#include <tts/engine/info.hpp>
#include <tts/engine/check.hpp>
#include <tts/engine/context.hpp>
#include <tts/tools/typename.hpp>

namespace tts::_
{
  template<typename A, typename B>
  text relation_message(A const& a, B const& b, check_site const& site, char const* kind)
  {
    return ::tts::text {"%s: %s %s %s is false because %s %s %s.",
                        kind,
                        site.lhs,
                        site.op,
                        site.rhs,
                        ::tts::as_text(a).data(),
                        site.negated,
                        ::tts::as_text(b).data()};
  }

  template<typename A, typename B> text types_differ_message(check_site const& site)
  {
    return ::tts::text {"'%s %s %s' is false because '%.*s' is not '%.*s'.",
                        site.lhs,
                        site.op,
                        site.rhs,
                        ::tts::typename_<A>.size(),
                        ::tts::typename_<A>.data(),
                        ::tts::typename_<B>.size(),
                        ::tts::typename_<B>.data()};
  }

  template<typename Context, typename Relation, typename A, typename B>
  constexpr auto
  check_relation(Context& ctx, Relation relation, A const& a, B const& b, check_site const& site)
  {
    ctx.operands(a, b);
    if(relation(a, b))
      return ctx.pass(
      site,
      [ &site ]
      { return ::tts::text {"Expression: %s %s %s is true.", site.lhs, site.op, site.rhs}; });

    return ctx.fail(site, [ &a, &b, &site ] { return relation_message(a, b, site, "Expression"); });
  }

  template<typename A, typename B, typename Context>
  constexpr auto types_differ(Context& ctx, check_site const& site)
  {
    return ctx.fail(site, [ &site ] { return types_differ_message<A, B>(site); });
  }

  template<typename Context, typename Relation, typename A, typename B>
  constexpr auto check_typed_relation(Context&          ctx,
                                      Relation          relation,
                                      A const&          a,
                                      B const&          b,
                                      check_site const& site)
  {
    if constexpr(!std::same_as<A, B>) return types_differ<A, B>(ctx, site);
    else return check_relation(ctx, relation, a, b, site);
  }

  template<bool Holds, typename Context, typename A, typename B>
  constexpr auto
  check_constant_relation(Context& ctx, A const& a, B const& b, check_site const& site)
  {
    ctx.operands(a, b);
    if constexpr(Holds)
      return ctx.pass(site,
                      [ &site ]
                      {
                        return ::tts::text {
                        "Constant expression: %s %s %s is true.", site.lhs, site.op, site.rhs};
                      });
    else
      return ctx.fail(
      site, [ &a, &b, &site ] { return relation_message(a, b, site, "Constant expression"); });
  }

  template<bool Holds, typename Context, typename A, typename B>
  constexpr auto
  check_typed_constant_relation(Context& ctx, A const& a, B const& b, check_site const& site)
  {
    if constexpr(!std::same_as<A, B>) return types_differ<A, B>(ctx, site);
    else return check_constant_relation<Holds>(ctx, a, b, site);
  }
}

#define TTS_RELATION(A, B, OP, T, F, ...)     TTS_RELATION_##__VA_ARGS__(A, B, OP, T, F)
#define TTS_RELATION_(A, B, OP, T, F)         TTS_RELATION_IMPL(A, B, OP, T, F, TTS_FAIL)
#define TTS_RELATION_REQUIRED(A, B, OP, T, F) TTS_RELATION_IMPL(A, B, OP, T, F, TTS_FATAL)

#define TTS_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                                 \
  ::tts::_::check_relation(local_tts_ctx(),                                                        \
                           ::tts::_::OP,                                                           \
                           A,                                                                      \
                           B,                                                                      \
                           TTS_CHECK_SITE(FAILURE, TTS_STRING(A), T, TTS_STRING(B), F)) /**/

//======================================================================================================================
/**
  @defgroup test-relation Relation Tests Macros
  @brief Macros for performing relation checks.
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @name Runtime Relation Checks
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @def TTS_BIT_EQUAL
  @brief Checks that two expressions have the same bits.

  Compares with the `bit_equal` member of tts::comparison, a `memcmp` over the whole object unless
  the type specializes it.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/bit_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_BIT_EQUAL(LHS, RHS, ...)
#else
#define TTS_BIT_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, bit_eq, "==", "!=", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_BIT_NOT_EQUAL
  @brief Checks that two expressions have different bits.

  Compares with the `bit_equal` member of tts::comparison, a `memcmp` over the whole object unless
  the type specializes it.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/bit_not_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_BIT_NOT_EQUAL(LHS, RHS, ...)
#else
#define TTS_BIT_NOT_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, bit_neq, "!=", "==", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_EQUAL
  @brief Checks that two expressions are equal.

  Compares with `operator==`, or with the [custom comparison](@ref customize-comparison) of the two
  types.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EQUAL(LHS, RHS, ...)
#else
#define TTS_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, eq, "==", "!=", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_NOT_EQUAL
  @brief Checks that two expressions are not equal.

  Compares with `operator==`, or with the [custom comparison](@ref customize-comparison) of the two
  types.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/not_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_NOT_EQUAL(LHS, RHS, ...)
#else
#define TTS_NOT_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, neq, "!=", "==", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_LESS
  @brief Checks that `LHS` is less than `RHS`.

  Compares with `operator<`, or with the [custom comparison](@ref customize-comparison) of the two
  types.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/less.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_LESS(LHS, RHS, ...)
#else
#define TTS_LESS(LHS, RHS, ...) TTS_RELATION(LHS, RHS, lt, "<", ">=", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_GREATER
  @brief Checks that `LHS` is greater than `RHS`.

  Holds when `LHS` is neither less than nor equal to `RHS`. Compares with `operator<` and
  `operator==`, or with the [custom comparison](@ref customize-comparison) of the two types.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/greater.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_GREATER(LHS, RHS, ...)
#else
#define TTS_GREATER(LHS, RHS, ...) TTS_RELATION(LHS, RHS, gt, ">", "<=", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_LESS_EQUAL
  @brief Checks that `LHS` is less than or equal to `RHS`.

  Compares with `operator<` and `operator==`, or with the
  [custom comparison](@ref customize-comparison) of the two types.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/less_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_LESS_EQUAL(LHS, RHS, ...)
#else
#define TTS_LESS_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, le, "<=", ">", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_GREATER_EQUAL
  @brief Checks that `LHS` is greater than or equal to `RHS`.

  Holds when `LHS` is not less than `RHS`. Compares with `operator<`, or with the
  [custom comparison](@ref customize-comparison) of the two types.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/greater_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_GREATER_EQUAL(LHS, RHS, ...)
#else
#define TTS_GREATER_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, ge, ">=", "<", __VA_ARGS__)
#endif

//======================================================================================================================
/// @}
//======================================================================================================================

//======================================================================================================================
/**
  @name Runtime Typed Relation Checks
  @{
**/
//======================================================================================================================

#define TTS_TYPED_RELATION(A, B, OP, T, F, ...) TTS_TYPED_RELATION_##__VA_ARGS__(A, B, OP, T, F)
#define TTS_TYPED_RELATION_(A, B, OP, T, F)     TTS_TYPED_RELATION_IMPL(A, B, OP, T, F, TTS_FAIL)
#define TTS_TYPED_RELATION_REQUIRED(A, B, OP, T, F)                                                \
  TTS_TYPED_RELATION_IMPL(A, B, OP, T, F, TTS_FATAL)

#define TTS_TYPED_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                           \
  ::tts::_::check_typed_relation(local_tts_ctx(),                                                  \
                                 ::tts::_::OP,                                                     \
                                 A,                                                                \
                                 B,                                                                \
                                 TTS_CHECK_SITE(FAILURE, TTS_STRING(A), T, TTS_STRING(B), F)) /**/

//======================================================================================================================
/**
  @def TTS_TYPED_EQUAL
  @brief Checks that two expressions are of the same type and that they are equal.

  Behaves as @ref TTS_EQUAL, and also fails when `LHS` and `RHS` differ in type.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/typed_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_EQUAL(LHS, RHS, ...)
#else
#define TTS_TYPED_EQUAL(LHS, RHS, ...) TTS_TYPED_RELATION(LHS, RHS, eq, "==", "!=", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_TYPED_NOT_EQUAL
  @brief Checks that two expressions are of the same type and that they are not equal.

  Behaves as @ref TTS_NOT_EQUAL, and also fails when `LHS` and `RHS` differ in type.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/typed_not_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_NOT_EQUAL(LHS, RHS, ...)
#else
#define TTS_TYPED_NOT_EQUAL(LHS, RHS, ...)                                                         \
  TTS_TYPED_RELATION(LHS, RHS, neq, "!=", "==", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_TYPED_LESS
  @brief Checks that two expressions are of the same type and that `LHS` is less than `RHS`.

  Behaves as @ref TTS_LESS, and also fails when `LHS` and `RHS` differ in type.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/typed_less.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_LESS(LHS, RHS, ...)
#else
#define TTS_TYPED_LESS(LHS, RHS, ...) TTS_TYPED_RELATION(LHS, RHS, lt, "<", ">=", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_TYPED_GREATER
  @brief Checks that two expressions are of the same type and that `LHS` is greater than `RHS`.

  Behaves as @ref TTS_GREATER, and also fails when `LHS` and `RHS` differ in type.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/typed_greater.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_GREATER(LHS, RHS, ...)
#else
#define TTS_TYPED_GREATER(LHS, RHS, ...) TTS_TYPED_RELATION(LHS, RHS, gt, ">", "<=", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_TYPED_LESS_EQUAL
  @brief Checks that two expressions are of the same type and that `LHS` is less than or equal to
  `RHS`.

  Behaves as @ref TTS_LESS_EQUAL, and also fails when `LHS` and `RHS` differ in type.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/typed_less_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_LESS_EQUAL(LHS, RHS, ...)
#else
#define TTS_TYPED_LESS_EQUAL(LHS, RHS, ...) TTS_TYPED_RELATION(LHS, RHS, le, "<=", ">", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_TYPED_GREATER_EQUAL
  @brief Checks that two expressions are of the same type and that `LHS` is greater than or equal to
  `RHS`.

  Behaves as @ref TTS_GREATER_EQUAL, and also fails when `LHS` and `RHS` differ in type.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/typed_greater_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TYPED_GREATER_EQUAL(LHS, RHS, ...)
#else
#define TTS_TYPED_GREATER_EQUAL(LHS, RHS, ...)                                                     \
  TTS_TYPED_RELATION(LHS, RHS, ge, ">=", "<", __VA_ARGS__)
#endif

//======================================================================================================================
/// @}
//======================================================================================================================

//======================================================================================================================
/**
  @name Constexpr Relation Checks
  @{
**/
//======================================================================================================================

#define TTS_CEXPR_RELATION(A, B, OP, T, F, ...) TTS_CEXPR_RELATION_##__VA_ARGS__(A, B, OP, T, F)
#define TTS_CEXPR_RELATION_(A, B, OP, T, F)     TTS_CEXPR_RELATION_IMPL(A, B, OP, T, F, TTS_FAIL)
#define TTS_CEXPR_RELATION_REQUIRED(A, B, OP, T, F)                                                \
  TTS_CEXPR_RELATION_IMPL(A, B, OP, T, F, TTS_FATAL)

#define TTS_CEXPR_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                           \
  ::tts::_::check_constant_relation<(::tts::_::OP(A, B))>(                                         \
  local_tts_ctx(), A, B, TTS_CHECK_SITE(FAILURE, TTS_STRING(A), T, TTS_STRING(B), F)) /**/

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_EQUAL
  @brief Checks at compile time that two constant expressions are equal.

  Compares with `operator==`, or with the [custom comparison](@ref customize-comparison) of the two
  types.

  Evaluates the expression at compile time, so it must be a constant expression, or the test does
  not compile. Reports the check at run time with the others, as a pass or a failure.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/cexpr_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_EQUAL(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_EQUAL(LHS, RHS, ...) TTS_CEXPR_RELATION(LHS, RHS, eq, "==", "!=", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_NOT_EQUAL
  @brief Checks at compile time that two constant expressions are not equal.

  Compares with `operator==`, or with the [custom comparison](@ref customize-comparison) of the two
  types.

  Evaluates the expression at compile time, so it must be a constant expression, or the test does
  not compile. Reports the check at run time with the others, as a pass or a failure.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/cexpr_not_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)                                                     \
  TTS_CEXPR_RELATION(LHS, RHS, neq, "!=", "==", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_LESS
  @brief Checks at compile time that `LHS` is less than `RHS`.

  Compares with `operator<`, or with the [custom comparison](@ref customize-comparison) of the two
  types.

  Evaluates the expression at compile time, so it must be a constant expression, or the test does
  not compile. Reports the check at run time with the others, as a pass or a failure.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/cexpr_less.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_LESS(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_LESS(LHS, RHS, ...) TTS_CEXPR_RELATION(LHS, RHS, lt, "<", ">=", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_GREATER
  @brief Checks at compile time that `LHS` is greater than `RHS`.

  Holds when `LHS` is neither less than nor equal to `RHS`. Compares with `operator<` and
  `operator==`, or with the [custom comparison](@ref customize-comparison) of the two types.

  Evaluates the expression at compile time, so it must be a constant expression, or the test does
  not compile. Reports the check at run time with the others, as a pass or a failure.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/cexpr_greater.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_GREATER(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_GREATER(LHS, RHS, ...)                                                       \
  TTS_CEXPR_RELATION(LHS, RHS, gt, ">", "<=", __VA_ARGS__)
#endif
//======================================================================================================================

/**
  @def TTS_CONSTEXPR_LESS_EQUAL
  @brief Checks at compile time that `LHS` is less than or equal to `RHS`.

  Compares with `operator<` and `operator==`, or with the
  [custom comparison](@ref customize-comparison) of the two types.

  Evaluates the expression at compile time, so it must be a constant expression, or the test does
  not compile. Reports the check at run time with the others, as a pass or a failure.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/cexpr_less_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)                                                    \
  TTS_CEXPR_RELATION(LHS, RHS, le, "<=", ">", __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_GREATER_EQUAL
  @brief Checks at compile time that `LHS` is greater than or equal to `RHS`.

  Holds when `LHS` is not less than `RHS`. Compares with `operator<`, or with the
  [custom comparison](@ref customize-comparison) of the two types.

  Evaluates the expression at compile time, so it must be a constant expression, or the test does
  not compile. Reports the check at run time with the others, as a pass or a failure.

  @param LHS, RHS Expressions to compare.
  @param ...      `REQUIRED` ends the run when the check fails.

  @groupheader{Example}
  @snippet doc/cexpr_greater_equal.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...)
#else
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...)                                                 \
  TTS_CEXPR_RELATION(LHS, RHS, ge, ">=", "<", __VA_ARGS__)
#endif

//======================================================================================================================
/// @}
//======================================================================================================================

#define TTS_TYPED_CEXPR_RELATION(A, B, OP, T, F, ...)                                              \
  TTS_TYPED_CEXPR_RELATION_##__VA_ARGS__(A, B, OP, T, F)
#define TTS_TYPED_CEXPR_RELATION_(A, B, OP, T, F)                                                  \
  TTS_TYPED_CEXPR_RELATION_IMPL(A, B, OP, T, F, TTS_FAIL)
#define TTS_TYPED_CEXPR_RELATION_REQUIRED(A, B, OP, T, F)                                          \
  TTS_TYPED_CEXPR_RELATION_IMPL(A, B, OP, T, F, TTS_FATAL)

#define TTS_TYPED_CEXPR_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                     \
  ::tts::_::check_typed_constant_relation<(::tts::_::OP(A, B))>(                                   \
  local_tts_ctx(), A, B, TTS_CHECK_SITE(FAILURE, TTS_STRING(A), T, TTS_STRING(B), F)) /**/

#define TTS_TYPED_CONSTEXPR_EQUAL(LHS, RHS, ...)                                                   \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, eq, "==", "!=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)                                               \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, neq, "!=", "==", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_LESS(LHS, RHS, ...)                                                    \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, lt, "<", ">=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_GREATER(LHS, RHS, ...)                                                 \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, gt, ">", "<=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)                                              \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, le, "<=", ">", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...)                                           \
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, ge, ">=", "<", __VA_ARGS__)

//======================================================================================================================
/// @}
//======================================================================================================================
