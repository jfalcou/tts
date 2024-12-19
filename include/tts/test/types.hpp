//======================================================================================================================
//! @file
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/engine/info.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/tools/typename.hpp>
#include <tts/tools/types.hpp>

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
    TTS_PASS( "Type: '%s' is '%s'."                                                                 \
            , TTS_STRING(TTS_REMOVE_PARENS(TYPE)), TTS_STRING(TTS_REMOVE_PARENS(REF))               \
            );                                                                                      \
    return ::tts::_::logger{false};                                                                 \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Type: '%s' is not the same as '%s' because '%.*s' is not '%.*s'."                    \
            , TTS_STRING(TTS_REMOVE_PARENS(TYPE)), TTS_STRING(TTS_REMOVE_PARENS(REF))               \
            , ::tts::typename_<TTS_T>.size(), ::tts::typename_<TTS_T>.data()                        \
            , ::tts::typename_<TTS_R>.size(), ::tts::typename_<TTS_R>.data()                        \
            );                                                                                      \
    return ::tts::_::logger{};                                                                      \
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
    TTS_PASS( "Expression: '%s' is of type '%s'."                                                   \
            , TTS_STRING(TTS_REMOVE_PARENS(EXPR)), TTS_STRING(TTS_REMOVE_PARENS(TYPE))              \
            );                                                                                      \
    return ::tts::_::logger{false};                                                                 \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expression: '%s' is not of type '%s' because '%.*s' is not '%.*s'."                  \
            , TTS_STRING(TTS_REMOVE_PARENS(EXPR)), TTS_STRING(TTS_REMOVE_PARENS(TYPE))              \
            , ::tts::typename_<TTS_T>.size(), ::tts::typename_<TTS_T>.data()                        \
            , ::tts::typename_<TTS_R>.size(), ::tts::typename_<TTS_R>.data()                        \
            );                                                                                      \
    return ::tts::_::logger{};                                                                      \
  }                                                                                                 \
}(::tts::type<decltype(TTS_REMOVE_PARENS(EXPR))>{}, ::tts::type<TTS_REMOVE_PARENS(TYPE)>{})         \
/**/

#define TTS_EXPECT_COMPILES_IMPL(EXPR, ...)                                                             \
TTS_DISABLE_WARNING_PUSH                                                                                \
TTS_DISABLE_WARNING_SHADOW                                                                              \
[&]( TTS_ARG(__VA_ARGS__) )                                                                             \
{                                                                                                       \
  if constexpr( requires TTS_REMOVE_PARENS(EXPR) )                                                      \
  {                                                                                                     \
    TTS_PASS("Expression: '%s' compiles as expected.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));            \
    return ::tts::_::logger{false};                                                                     \
  }                                                                                                     \
  else                                                                                                  \
  {                                                                                                     \
    TTS_FAIL ( "Expression: '%s' does not compile as expected.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));  \
    return ::tts::_::logger{};                                                                          \
  }                                                                                                     \
TTS_DISABLE_WARNING_POP                                                                                 \
}(__VA_ARGS__)                                                                                          \
/**/

//======================================================================================================================
/**
  @def TTS_EXPECT_COMPILES
  @brief Checks if an `Expression` based on a list of `Symbols` will compile properly in a SFINAE context.

  @param Symbols Variadic lists of symbols used in the tests
  @param Expression Brace-enclosed expression to validate.

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

#define TTS_EXPECT_NOT_COMPILES_IMPL(EXPR, ...)                                                         \
TTS_DISABLE_WARNING_PUSH                                                                                \
TTS_DISABLE_WARNING_SHADOW                                                                              \
[&]( TTS_ARG(__VA_ARGS__) )                                                                             \
{                                                                                                       \
  if constexpr( !(requires TTS_REMOVE_PARENS(EXPR)) )                                                   \
  {                                                                                                     \
    TTS_PASS("Expression: '%s' does not compile as expected.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));    \
    return ::tts::_::logger{false};                                                                     \
  }                                                                                                     \
  else                                                                                                  \
  {                                                                                                     \
    TTS_FAIL ( "Expression: '%s' compiles unexpectedly.", TTS_STRING(TTS_REMOVE_PARENS(EXPR)));         \
    return ::tts::_::logger{};                                                                          \
  }                                                                                                     \
TTS_DISABLE_WARNING_POP                                                                                 \
}(__VA_ARGS__)                                                                                          \
/**/

//======================================================================================================================
/**
  @def TTS_EXPECT_NOT_COMPILES
  @brief Checks if an `Expression` based on a list of `Symbols` will not compile properly in a SFINAE context.

  @param Symbols Variadic lists of symbols used in the tests
  @param Expression Brace-enclosed expression to validate.

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
