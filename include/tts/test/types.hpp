//======================================================================================================================
//! @file
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/test/info.hpp>
#include <tts/tools/types.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/tools/typename.hpp>

//======================================================================================================================
/**
  @def TTS_TYPE_IS
  @brief Checks if two types satisfy `std::is_same_v<Type,Target> == true`.

  @param TYPE Type to compare.
  @param REF  Expected type.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <type_traits>

  TTS_CASE( "Check that types can be tested for equivalence" )
  {
    TTS_TYPE_IS( std::add_pointer<float const>::type, float const* );
  };
  @endcode
**/
//======================================================================================================================
#define TTS_TYPE_IS(TYPE, REF, ...)     TTS_TYPE_IS_ ## __VA_ARGS__ (TYPE, REF)
#define TTS_TYPE_IS_(TYPE, REF)         TTS_TYPE_IS_IMPL(TYPE, REF,TTS_FAIL)
#define TTS_TYPE_IS_REQUIRED(TYPE, REF) TTS_TYPE_IS_IMPL(TYPE, REF,TTS_FATAL)

#define TTS_TYPE_IS_IMPL(TYPE, REF, FAILURE)                                                        \
[&]<typename TTS_T, typename TTS_R>(::tts::type<TTS_T>, ::tts::type<TTS_R>)                         \
{                                                                                                   \
  if constexpr( std::is_same_v<TTS_T,TTS_R> )                                                       \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::detail::logger{false};                              \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Type: "  << TTS_STRING(TTS_REMOVE_PARENS(TYPE)) << " is not the same as "            \
                        << TTS_STRING(TTS_REMOVE_PARENS(REF))  << " because "                       \
                        << ::tts::typename_<TTS_T> << " is not " << ::tts::typename_<TTS_R>         \
            );                                                                                      \
    return ::tts::detail::logger{};                                                                 \
  }                                                                                                 \
}(::tts::type<TTS_REMOVE_PARENS(TYPE)>{}, ::tts::type<TTS_REMOVE_PARENS(REF)>{})                    \
/**/

//======================================================================================================================
/**
  @def TTS_EXPR_IS
  @brief Checks if an Expression evaluates to a value of a given Type.

  @param EXPR Expression to evaluate.
  @param TYPE Expected type.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check that expression types can be tested for equality" )
  {
    double d;

    TTS_EXPR_IS( &d + 5        , double*   );
    TTS_EXPR_IS( std::move(d)  , double&&  );
    TTS_EXPR_IS( std::swap(d,d), void      );
  };
  @endcode
**/
//======================================================================================================================
#define TTS_EXPR_IS(EXPR, TYPE, ...)     TTS_EXPR_IS_ ## __VA_ARGS__ (EXPR, TYPE)
#define TTS_EXPR_IS_(EXPR, TYPE)         TTS_EXPR_IS_IMPL(EXPR, TYPE,TTS_FAIL)
#define TTS_EXPR_IS_REQUIRED(EXPR, TYPE) TTS_EXPR_IS_IMPL(EXPR, TYPE,TTS_FATAL)

#define TTS_EXPR_IS_IMPL(EXPR, TYPE, FAILURE)                                                       \
[&]<typename TTS_T, typename TTS_R>(::tts::type<TTS_T>, ::tts::type<TTS_R>)                         \
{                                                                                                   \
  if constexpr( std::is_same_v<TTS_T,TTS_R> )                                                       \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::detail::logger{false};                              \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE (   "Type: "  << TTS_STRING(TTS_REMOVE_PARENS(EXPR))  << " is not the same as "         \
                          << TTS_STRING(TTS_REMOVE_PARENS(TYPE)) << " because "                     \
                          << ::tts::typename_<TTS_T> << " is not " << ::tts::typename_<TTS_R>       \
            );                                                                                      \
    return ::tts::detail::logger{};                                                                 \
  }                                                                                                 \
}(::tts::type<decltype(TTS_REMOVE_PARENS(EXPR))>{}, ::tts::type<TTS_REMOVE_PARENS(TYPE)>{})         \
/**/

#define TTS_EXPECT_COMPILES_IMPL(EXPR, ...)                                                         \
[&]( TTS_ARG(__VA_ARGS__) )                                                                         \
{                                                                                                   \
  if constexpr( requires TTS_REMOVE_PARENS(EXPR) )                                                  \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::detail::logger{false};                              \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    TTS_FAIL(     "Expression: " << TTS_STRING(TTS_REMOVE_PARENS(EXPR))                             \
              <<  " does not compile as expected."                                                  \
            );                                                                                      \
    return ::tts::detail::logger{};                                                                 \
  }                                                                                                 \
}(__VA_ARGS__)                                                                                      \
/**/

//======================================================================================================================
/**
  @def TTS_EXPECT_COMPILES
  @brief Checks if an `Expression` based on a list of `Symbols` will compile properly in a SFINAE context.

  @param Symbols Variadic lists of symbols used in the tests
  @param Expression Brace-enclosed expression to validate.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check that expression can compile properly" )
  {
    double d, e;

    TTS_EXPECT_COMPILES(d, e, { d += 4. * e; } );
  };
  @endcode
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPECT_COMPILES(Symbols..., Expression, ...)
#else
#define TTS_EXPECT_COMPILES(...) TTS_VAL(TTS_EXPECT_COMPILES_IMPL TTS_REVERSE(__VA_ARGS__) )
#endif

#define TTS_EXPECT_NOT_COMPILES_IMPL(EXPR, ...)                                                     \
[&]( TTS_ARG(__VA_ARGS__) )                                                                         \
{                                                                                                   \
  if constexpr( !(requires TTS_REMOVE_PARENS(EXPR)) )                                               \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::detail::logger{false};                              \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    TTS_FAIL("Expression: " << TTS_STRING(TTS_REMOVE_PARENS(EXPR)) << " compiles unexpectedly." );  \
    return ::tts::detail::logger{};                                                                 \
  }                                                                                                 \
}(__VA_ARGS__)                                                                                      \
/**/

//======================================================================================================================
/**
  @def TTS_EXPECT_NOT_COMPILES
  @brief Checks if an `Expression` based on a list of `Symbols` will not compile properly in a SFINAE context.

  @param Symbols Variadic lists of symbols used in the tests
  @param Expression Brace-enclosed expression to validate.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check that expression can compile properly" )
  {
    double d, e;

    TTS_EXPECT_NOT_COMPILES(d, e, { d.foo(e); } );
  };
  @endcode
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_EXPECT_NOT_COMPILES(Symbols..., Expression, ...)
#else
#define TTS_EXPECT_NOT_COMPILES(...) TTS_VAL(TTS_EXPECT_NOT_COMPILES_IMPL TTS_REVERSE(__VA_ARGS__))
#endif
