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

#define TTS_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                 \
  if(::tts::_::OP(local_tts_a, local_tts_b))                                                       \
  {                                                                                                \
    TTS_PASS("Expression: %s %s %s is true.", TTS_STRING(A), T, TTS_STRING(B));                    \
    return ::tts::_::logger {false};                                                               \
  }                                                                                                \
  else                                                                                             \
  {                                                                                                \
    FAILURE("Expression: %s %s %s is false because %s %s %s.",                                     \
            TTS_STRING(A),                                                                         \
            T,                                                                                     \
            TTS_STRING(B),                                                                         \
            ::tts::as_text(local_tts_a).data(),                                                    \
            F,                                                                                     \
            ::tts::as_text(local_tts_b).data());                                                   \
    return ::tts::_::logger {};                                                                    \
  }                                                                                                \
  /**/

#define TTS_CEXPR_RELATION_BASE(A, B, OP, T, F, FAILURE)                                           \
  constexpr auto local_tts_expr = ::tts::_::OP(A, B);                                              \
  if constexpr(local_tts_expr)                                                                     \
  {                                                                                                \
    TTS_PASS("Constant expression: %s %s %s is true.", TTS_STRING(A), T, TTS_STRING(B));           \
    ::tts::global_logger_status = false;                                                           \
  }                                                                                                \
  else                                                                                             \
  {                                                                                                \
    FAILURE("Constant expression: %s %s %s is false because %s %s %s.",                            \
            TTS_STRING(A),                                                                         \
            T,                                                                                     \
            TTS_STRING(B),                                                                         \
            ::tts::as_text(A).data(),                                                              \
            F,                                                                                     \
            ::tts::as_text(B).data());                                                             \
    ::tts::global_logger_status = true;                                                            \
  }                                                                                                \
  /**/

#define TTS_RELATION(A, B, OP, T, F, ...)     TTS_RELATION_##__VA_ARGS__(A, B, OP, T, F)
#define TTS_RELATION_(A, B, OP, T, F)         TTS_RELATION_IMPL(A, B, OP, T, F, TTS_FAIL)
#define TTS_RELATION_REQUIRED(A, B, OP, T, F) TTS_RELATION_IMPL(A, B, OP, T, F, TTS_FATAL)

#define TTS_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                                 \
  [ & ](auto const& local_tts_a, auto const& local_tts_b)                                          \
  { TTS_RELATION_BASE(A, B, OP, T, F, FAILURE) }(A, B) /**/

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

  This comparison is performed by using memcmp on the underlying representation of both expressions.

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

  This comparison is performed by using memcmp on the underlying representation of both expressions.

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
#define TTS_GREATER_EQUAL(LHS, RHS, ...) TTS_RELATION(LHS, RHS, ge, ">=", "<=", __VA_ARGS__)
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
  [ & ](auto&& local_tts_a, auto&& local_tts_b)                                                    \
  {                                                                                                \
    using type_a = std::remove_cvref_t<decltype(local_tts_a)>;                                     \
    using type_b = std::remove_cvref_t<decltype(local_tts_b)>;                                     \
                                                                                                   \
    if(!std::same_as<type_a, type_b>)                                                              \
    {                                                                                              \
      FAILURE("'%s %s %s' is false because '%.*s' is not '%.*s'.",                                 \
              TTS_STRING(A),                                                                       \
              T,                                                                                   \
              TTS_STRING(B),                                                                       \
              ::tts::typename_<type_a>.size(),                                                     \
              ::tts::typename_<type_a>.data(),                                                     \
              ::tts::typename_<type_b>.size(),                                                     \
              ::tts::typename_<type_b>.data());                                                    \
      return ::tts::_::logger {};                                                                  \
    }                                                                                              \
    else { TTS_RELATION_BASE(A, B, OP, T, F, FAILURE) }                                            \
  }(A, B) /**/

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
  TTS_TYPED_RELATION(LHS, RHS, ge, ">=", "<=", __VA_ARGS__)
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
  ::tts::global_logger_status = false;                                                             \
  do {                                                                                             \
    TTS_CEXPR_RELATION_BASE(A, B, OP, T, F, FAILURE)                                               \
  } while(0);                                                                                      \
  ::tts::_::logger { ::tts::global_logger_status }                                                 \
/**/

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
  TTS_CEXPR_RELATION(LHS, RHS, ge, ">=", "<=", __VA_ARGS__)
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
  ::tts::global_logger_status = false;                                                             \
  do {                                                                                             \
    using type_a = std::remove_cvref_t<decltype(A)>;                                               \
    using type_b = std::remove_cvref_t<decltype(B)>;                                               \
                                                                                                   \
    if(!std::same_as<type_a, type_b>)                                                              \
    {                                                                                              \
      FAILURE("'%s %s %s' is false because '%.*s' is not '%.*s'.",                                 \
              TTS_STRING(A),                                                                       \
              T,                                                                                   \
              TTS_STRING(B),                                                                       \
              ::tts::typename_<type_a>.size(),                                                     \
              ::tts::typename_<type_a>.data(),                                                     \
              ::tts::typename_<type_b>.size(),                                                     \
              ::tts::typename_<type_b>.data());                                                    \
      ::tts::global_logger_status = false;                                                         \
    }                                                                                              \
    else { TTS_CEXPR_RELATION_BASE(A, B, OP, T, F, FAILURE) }                                      \
  } while(0);                                                                                      \
  ::tts::_::logger { ::tts::global_logger_status }                                                 \
  /**/

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
  TTS_TYPED_CEXPR_RELATION(LHS, RHS, ge, ">=", "<=", __VA_ARGS__)

//======================================================================================================================
/// @}
//======================================================================================================================
