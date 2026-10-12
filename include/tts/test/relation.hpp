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
#include <tts/tools/typename.hpp>

namespace tts::_
{
  template<typename A, typename B>
  logger relation_failed(A const& a, B const& b, check_site const& site, char const* kind)
  {
    return check_failed(site,
                        ::tts::text {"%s: %s %s %s is false because %s %s %s.",
                                     kind,
                                     site.lhs,
                                     site.op,
                                     site.rhs,
                                     ::tts::as_text(a).data(),
                                     site.negated,
                                     ::tts::as_text(b).data()});
  }

  template<typename Relation, typename A, typename B>
  logger check_relation(Relation relation, A const& a, B const& b, check_site const& site)
  {
    if(relation(a, b))
    {
      if(check_passed())
        check_report(site,
                     ::tts::text {"Expression: %s %s %s is true.", site.lhs, site.op, site.rhs});
      return logger {false};
    }

    return relation_failed(a, b, site, "Expression");
  }

  template<typename A, typename B> logger types_differ(check_site const& site)
  {
    return check_failed(site,
                        ::tts::text {"'%s %s %s' is false because '%.*s' is not '%.*s'.",
                                     site.lhs,
                                     site.op,
                                     site.rhs,
                                     ::tts::typename_<A>.size(),
                                     ::tts::typename_<A>.data(),
                                     ::tts::typename_<B>.size(),
                                     ::tts::typename_<B>.data()});
  }

  template<typename Relation, typename A, typename B>
  logger check_typed_relation(Relation relation, A const& a, B const& b, check_site const& site)
  {
    if constexpr(!std::same_as<A, B>) return types_differ<A, B>(site);
    else return check_relation(relation, a, b, site);
  }

  template<bool Holds, typename A, typename B>
  logger check_constant_relation(A const& a, B const& b, check_site const& site)
  {
    if constexpr(Holds)
    {
      if(check_passed())
        check_report(
        site, ::tts::text {"Constant expression: %s %s %s is true.", site.lhs, site.op, site.rhs});
      return logger {false};
    }
    else
    {
      return relation_failed(a, b, site, "Constant expression");
    }
  }

  template<bool Holds, typename A, typename B>
  logger check_typed_constant_relation(A const& a, B const& b, check_site const& site)
  {
    if constexpr(!std::same_as<A, B>)
    {
      // No additional information on differing types, as in the published output.
      types_differ<A, B>(site);
      return logger {false};
    }
    else
    {
      return check_constant_relation<Holds>(a, b, site);
    }
  }
}

#define TTS_RELATION(A, B, OP, T, F, ...)     TTS_RELATION_##__VA_ARGS__(A, B, OP, T, F)
#define TTS_RELATION_(A, B, OP, T, F)         TTS_RELATION_IMPL(A, B, OP, T, F, TTS_FAIL)
#define TTS_RELATION_REQUIRED(A, B, OP, T, F) TTS_RELATION_IMPL(A, B, OP, T, F, TTS_FATAL)

#define TTS_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                                 \
  ::tts::_::check_relation(                                                                        \
  ::tts::_::OP, A, B, TTS_CHECK_SITE(FAILURE, TTS_STRING(A), T, TTS_STRING(B), F)) /**/

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
  @brief Performs equality comparison between the bits of two expressions

  This comparison goes through the `bit_equal` member of tts::comparison, a `memcmp` over the whole
  object unless the type specializes it.

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs inequality comparison between the bits of two expressions

  This comparison goes through the `bit_equal` member of tts::comparison, a `memcmp` over the whole
  object unless the type specializes it.

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs equality comparison between two expressions

  This comparison is performed by using the proper `operator==` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.


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
  @brief Performs inequality comparison between two expressions

  This comparison is performed by using the proper `operator==` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs less-than comparison between two expressions

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs greater-than comparison between two expressions

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs less-or-equal-than comparison between two expressions

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs greater-or-equal-than comparison between two expressions

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  ::tts::_::check_typed_relation(                                                                  \
  ::tts::_::OP, A, B, TTS_CHECK_SITE(FAILURE, TTS_STRING(A), T, TTS_STRING(B), F)) /**/

//======================================================================================================================
/**
  @def TTS_TYPED_EQUAL
  @brief Performs type and equality comparison between two expressions

  This test behaves as @ref TTS_EQUAL but also verify that both arguments's types are the same.

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs type and equality comparison between two expressions

  This test behaves as @ref TTS_NOT_EQUAL but also verify that both arguments's types are the same.

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs type and equality comparison between two expressions

  This test behaves as @ref TTS_LESS but also verify that both arguments's types are the same.

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs type and equality comparison between two expressions

  This test behaves as @ref TTS_GREATER but also verify that both arguments's types are the same.

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs type and equality comparison between two expressions

  This test behaves as @ref TTS_LESS_EQUAL but also verify that both arguments's types are the same.

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs type and equality comparison between two expressions

  This test behaves as @ref TTS_GREATER_EQUAL but also verify that both arguments's types are the
same.

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  A, B, TTS_CHECK_SITE(FAILURE, TTS_STRING(A), T, TTS_STRING(B), F)) /**/

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_EQUAL
  @brief Performs equality comparison between two expressions at compile-time

  This comparison is performed by using the proper `operator==` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs inequality comparison between two expressions at compile-time

  This comparison is performed by using the proper `operator==` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs less-than comparison between two expressions at compile-time.

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs greater-than comparison between two expressions at compile-time.

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs less-or-equal-than comparison between two expressions at compile-time

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  @brief Performs greater-or-equal-than comparison between two expressions at compile-time

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

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
  A, B, TTS_CHECK_SITE(FAILURE, TTS_STRING(A), T, TTS_STRING(B), F)) /**/

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
