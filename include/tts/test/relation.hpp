//======================================================================================================================
//! @file
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/tools/comparators.hpp>
#include <tts/tools/as_text.hpp>
#include <tts/engine/info.hpp>

#define TTS_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                \
if( ::tts::detail::OP(local_tts_a,local_tts_b) )                                                  \
{                                                                                                 \
    TTS_PASS( "'%s %s %s' is true.", TTS_STRING(A), T, TTS_STRING(B) );                           \
    return ::tts::_::logger{false};                                                               \
}                                                                                                 \
else                                                                                              \
{                                                                                                 \
  FAILURE ( "'%s %s %s' is false because '%s %s %s'."                                             \
          , TTS_STRING(A), T, TTS_STRING(B)                                                       \
          , ::tts::as_text(local_tts_a).data(), F, ::tts::as_text(local_tts_b).data()             \
          );                                                                                      \
  return ::tts::_::logger{};                                                                      \
}                                                                                                 \
/**/

#define TTS_CEXPR_RELATION_BASE( A, B, OP, T, F, FAILURE)                                         \
constexpr auto local_tts_expr = ::tts::detail::OP(A,B);                                           \
if constexpr( local_tts_expr )                                                                    \
{                                                                                                 \
  TTS_PASS( "Constant expression: '%s %s %s' is true.", TTS_STRING(A), T, TTS_STRING(B) );        \
    return ::tts::_::logger{false};                                                               \
}                                                                                                 \
else                                                                                              \
{                                                                                                 \
  FAILURE ( "Constant expression '%s %s %s' is false because '%s %s %s'."                         \
          , TTS_STRING(A), T, TTS_STRING(B)                                                       \
          , ::tts::as_text(A).data(), F, ::tts::as_text(B).data()                                 \
          );                                                                                      \
  return ::tts::_::logger{};                                                                      \
}                                                                                                 \
/**/

#define TTS_RELATION(A, B, OP, T, F, ...)     TTS_RELATION_ ## __VA_ARGS__ (A,B,OP,T,F)
#define TTS_RELATION_(A, B, OP, T, F)         TTS_RELATION_IMPL(A,B,OP,T,F,TTS_FAIL)
#define TTS_RELATION_REQUIRED(A, B, OP, T, F) TTS_RELATION_IMPL(A,B,OP,T,F,TTS_FATAL)

#define TTS_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                                  \
[&](auto&& local_tts_a, auto&& local_tts_b)                                                         \
{                                                                                                   \
  TTS_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                        \
}(A,B)                                                                                              \
/**/

//======================================================================================================================
/**
  @def TTS_EQUAL
  @brief Performs equality comparison between two expressions

  This comparison is performed by using the proper `operator==` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <vector>

  TTS_CASE( "Check correctness of equality tests" )
  {
    TTS_EQUAL(45,45.f);

    std::vector<float> a{1,2,3}, b{1,2,3};
    TTS_EQUAL(a,b);

    // This test would cause the program to stop with a failure.
    // TTS_EQUAL(12, 21, REQUIRED);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_EQUAL(LHS, RHS, ...)          TTS_RELATION(LHS,RHS, eq , "==" , "!=" , __VA_ARGS__)

//======================================================================================================================
/**
  @def TTS_NOT_EQUAL
  @brief Performs inequality comparison between two expressions

  This comparison is performed by using the proper `operator==` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <vector>

  TTS_CASE( "Check correctness of inequality tests" )
  {
    TTS_NOT_EQUAL(42,69.f);

    std::vector<float> a{1,2,3}, b{7,9};
    TTS_NOT_EQUAL(a,b);

    // This test would cause the program to stop with a failure
    // TTS_NOT_EQUAL('A', 65, REQUIRED);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_NOT_EQUAL(LHS, RHS, ...)      TTS_RELATION(LHS,RHS, neq, "!=" , "==" , __VA_ARGS__)

//======================================================================================================================
/**
  @def TTS_LESS
  @brief Performs less-than comparison between two expressions

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <vector>

  TTS_CASE( "Check correctness of less-than comparison tests" )
  {
    TTS_LESS(42., 69);

    std::vector<float> a{1,2,3}, b{12,3};
    TTS_LESS(a,b);

    // This test would cause the program to stop with a failure
    // TTS_LESS(69, 42, REQUIRED);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_LESS(LHS, RHS, ...)           TTS_RELATION(LHS,RHS, lt , "<"  , ">=" , __VA_ARGS__)

//======================================================================================================================
/**
  @def TTS_GREATER
  @brief Performs greater-than comparison between two expressions

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <vector>

  TTS_CASE( "Check correctness of less-than comparison tests" )
  {
    TTS_GREATER(69., 42ULL);

    std::vector<float> a{13,5}, b{2,4,6};
    TTS_GREATER(a,b);

    // This test will cause the program to stop with a failure
    TTS_GREATER(1., 10., REQUIRED);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_GREATER(LHS, RHS, ...)        TTS_RELATION(LHS,RHS, gt , ">"  , "<=" , __VA_ARGS__)

//======================================================================================================================
/**
  @def TTS_LESS_EQUAL
  @brief Performs less-or-equal-than comparison between two expressions

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <vector>

  TTS_CASE( "Check correctness of less-than comparison tests" )
  {
    TTS_LESS_EQUAL(42., 69);
    TTS_LESS_EQUAL(69., 69);

    std::vector<float> a{1,2,3}, b{7,9};
    TTS_LESS_EQUAL(a,b);
    TTS_LESS_EQUAL(a,a);

    // This test would cause the program to stop with a failure
    // TTS_LESS_EQUAL(10., 1., REQUIRED);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_LESS_EQUAL(LHS, RHS, ...)     TTS_RELATION(LHS,RHS, le , "<=" , ">"  , __VA_ARGS__)

//======================================================================================================================
/**
  @def TTS_GREATER_EQUAL
  @brief Performs greater-or-equal-than comparison between two expressions

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <vector>

  TTS_CASE( "Check correctness of less-than comparison tests" )
  {
    TTS_GREATER_EQUAL(69LL, 42.f);
    TTS_GREATER_EQUAL(69., 69);

    std::vector<float> a{12,3}, b{7,9,11};
    TTS_GREATER_EQUAL(a,b);
    TTS_GREATER_EQUAL(a,a);

    // This test would cause the program to stop with a failure
    // TTS_GREATER_EQUAL(1., 10., REQUIRED);
  };

  @endcode
**/
//======================================================================================================================
#define TTS_GREATER_EQUAL(LHS, RHS, ...)  TTS_RELATION(LHS,RHS, ge , ">=" , "<=" , __VA_ARGS__)

#define TTS_CEXPR_RELATION(A, B, OP, T, F, ...)     TTS_CEXPR_RELATION_ ## __VA_ARGS__ (A,B,OP,T,F)
#define TTS_CEXPR_RELATION_(A, B, OP, T, F)         TTS_CEXPR_RELATION_IMPL(A,B,OP,T,F,TTS_FAIL)
#define TTS_CEXPR_RELATION_REQUIRED(A, B, OP, T, F) TTS_CEXPR_RELATION_IMPL(A,B,OP,T,F,TTS_FATAL)

#define TTS_CEXPR_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                            \
do                                                                                                  \
{                                                                                                   \
  TTS_CEXPR_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                  \
}while(0);                                                                                          \
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

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check correctness of constexpr equality tests" )
  {
    constexpr float a = 45.f;
    constexpr int   b = 45;

    TTS_CONSTEXPR_EQUAL(a, b);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_CONSTEXPR_EQUAL(LHS, RHS, ...)          TTS_CEXPR_RELATION(LHS,RHS, eq , "==" , "!=", __VA_ARGS__)

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_NOT_EQUAL
  @brief Performs inequality comparison between two expressions at compile-time

  This comparison is performed by using the proper `operator==` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check correctness of constexpr equality tests" )
  {
    constexpr float a = 4.5f;
    constexpr int   b = 45;

    TTS_CONSTEXPR_NOT_EQUAL(a, b);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)      TTS_CEXPR_RELATION(LHS,RHS, neq, "!=" , "==", __VA_ARGS__)

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_LESS
  @brief Performs less-than comparison between two expressions at compile-time.

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check correctness of less-than comparison tests" )
  {
    TTS_CONSTEXPR_LESS(42LL, 69.f);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_CONSTEXPR_LESS(LHS, RHS, ...)           TTS_CEXPR_RELATION(LHS,RHS, lt , "<"  , ">=", __VA_ARGS__)

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_GREATER
  @brief Performs greater-than comparison between two expressions at compile-time.

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check correctness of greater-than comparison tests" )
  {
    TTS_CONSTEXPR_GREATER(69LL, 42.f);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_CONSTEXPR_GREATER(LHS, RHS, ...)        TTS_CEXPR_RELATION(LHS,RHS, gt , ">"  , "<=", __VA_ARGS__)

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_LESS_EQUAL
  @brief Performs less-or-equal-than comparison between two expressions at compile-time

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check correctness of less-equal comparison tests" )
  {
    TTS_CONSTEXPR_LESS_EQUAL(42LL, 69.f);
    TTS_CONSTEXPR_LESS_EQUAL('A' , 65. );
  };
  @endcode
**/
//======================================================================================================================
#define TTS_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)     TTS_CEXPR_RELATION(LHS,RHS, le , "<=" , ">" , __VA_ARGS__)

//======================================================================================================================
/**
  @def TTS_CONSTEXPR_GREATER_EQUAL
  @brief Performs greater-or-equal-than comparison between two expressions at compile-time

  This comparison is performed by using the proper `operator<` overload or by a
  [custom comparison](@ref customize-comparison).

  @param LHS, RHS  Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check correctness of greater-equal comparison tests" )
  {
    TTS_CONSTEXPR_GREATER_EQUAL(69LL, 42.f);
    TTS_CONSTEXPR_GREATER_EQUAL(69., 69);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...)  TTS_CEXPR_RELATION(LHS,RHS, ge , ">=" , "<=", __VA_ARGS__)

#define TTS_TYPED_RELATION(A, B, OP, T, F, ...)     TTS_TYPED_RELATION_ ## __VA_ARGS__ (A,B,OP,T,F)
#define TTS_TYPED_RELATION_(A, B, OP, T, F)         TTS_TYPED_RELATION_IMPL(A,B,OP,T,F,TTS_FAIL)
#define TTS_TYPED_RELATION_REQUIRED(A, B, OP, T, F) TTS_TYPED_RELATION_IMPL(A,B,OP,T,F,TTS_FATAL)

#define TTS_TYPED_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                            \
[&](auto&& local_tts_a, auto&& local_tts_b)                                                         \
{                                                                                                   \
  using type_a = std::remove_cvref_t<decltype(local_tts_a)>;                                        \
  using type_b = std::remove_cvref_t<decltype(local_tts_b)>;                                        \
                                                                                                    \
  if ( !std::same_as<type_a, type_b> )                                                              \
  {                                                                                                 \
    FAILURE ( "'%s %s %s' is false because '%.*s' is not '%.*s'."                                   \
            , TTS_STRING(A), T, TTS_STRING(B)                                                       \
            , ::tts::typename_<type_a>.size(), ::tts::typename_<type_a>.data()                      \
            , ::tts::typename_<type_b>.size(), ::tts::typename_<type_b>.data()                      \
            );                                                                                      \
    return ::tts::_::logger{};                                                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    TTS_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                      \
  }                                                                                                 \
}(A,B)                                                                                              \
/**/

#define TTS_TYPED_EQUAL(LHS, RHS, ...)          TTS_TYPED_RELATION(LHS,RHS, eq , "==" , "!=" , __VA_ARGS__)
#define TTS_TYPED_NOT_EQUAL(LHS, RHS, ...)      TTS_TYPED_RELATION(LHS,RHS, neq, "!=" , "==" , __VA_ARGS__)
#define TTS_TYPED_LESS(LHS, RHS, ...)           TTS_TYPED_RELATION(LHS,RHS, lt , "<"  , ">=" , __VA_ARGS__)
#define TTS_TYPED_GREATER(LHS, RHS, ...)        TTS_TYPED_RELATION(LHS,RHS, gt , ">"  , "<=" , __VA_ARGS__)
#define TTS_TYPED_LESS_EQUAL(LHS, RHS, ...)     TTS_TYPED_RELATION(LHS,RHS, le , "<=" , ">"  , __VA_ARGS__)
#define TTS_TYPED_GREATER_EQUAL(LHS, RHS, ...)  TTS_TYPED_RELATION(LHS,RHS, ge , ">=" , "<=" , __VA_ARGS__)

#define TTS_TYPED_CEXPR_RELATION(A, B, OP, T, F, ...)     TTS_TYPED_CEXPR_RELATION_ ## __VA_ARGS__ (A,B,OP,T,F)
#define TTS_TYPED_CEXPR_RELATION_(A, B, OP, T, F)         TTS_TYPED_CEXPR_RELATION_IMPL(A,B,OP,T,F,TTS_FAIL)
#define TTS_TYPED_CEXPR_RELATION_REQUIRED(A, B, OP, T, F) TTS_TYPED_CEXPR_RELATION_IMPL(A,B,OP,T,F,TTS_FATAL)

#define TTS_TYPED_CEXPR_RELATION_IMPL(A, B, OP, T, F, FAILURE)                                      \
do                                                                                                  \
{                                                                                                   \
  using type_a = std::remove_cvref_t<decltype(A)>;                                                  \
  using type_b = std::remove_cvref_t<decltype(B)>;                                                  \
                                                                                                    \
  if ( !std::same_as<type_a, type_b> )                                                              \
  {                                                                                                 \
    FAILURE ( "'%s %s %s' is false because '%.*s' is not '%.*s'."                                   \
            , TTS_STRING(A), T, TTS_STRING(B)                                                       \
            , ::tts::typename_<type_a>.size(), ::tts::typename_<type_a>.data()                      \
            , ::tts::typename_<type_b>.size(), ::tts::typename_<type_b>.data()                      \
            );                                                                                      \
    return ::tts::_::logger{};                                                                      \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    TTS_CEXPR_RELATION_BASE(A, B, OP, T, F, FAILURE)                                                \
  }                                                                                                 \
} while(0)                                                                                          \
/**/

#define TTS_TYPED_CONSTEXPR_EQUAL(LHS, RHS, ...)          TTS_TYPED_CEXPR_RELATION(LHS,RHS, eq , "==" , "!=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)      TTS_TYPED_CEXPR_RELATION(LHS,RHS, neq, "!=" , "==", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_LESS(LHS, RHS, ...)           TTS_TYPED_CEXPR_RELATION(LHS,RHS, lt , "<"  , ">=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_GREATER(LHS, RHS, ...)        TTS_TYPED_CEXPR_RELATION(LHS,RHS, gt , ">"  , "<=", __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)     TTS_TYPED_CEXPR_RELATION(LHS,RHS, le , "<=" , ">" , __VA_ARGS__)
#define TTS_TYPED_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...)  TTS_TYPED_CEXPR_RELATION(LHS,RHS, ge , ">=" , "<=", __VA_ARGS__)
