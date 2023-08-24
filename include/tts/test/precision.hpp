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
#include <tts/tools/preprocessor.hpp>
#include <tts/engine/precision.hpp>
#include <tts/engine/logger.hpp>

#define TTS_PRECISION_IMPL(LHS, RHS, N, UNIT, FUNC, PREC,FAILURE)                                   \
[&](auto lhs, auto rhs)                                                                             \
{                                                                                                   \
  auto r = FUNC (lhs,rhs);                                                                          \
                                                                                                    \
  if(r <= N)                                                                                        \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::detail::logger{false};                              \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: " << TTS_STRING(LHS) << " == " << TTS_STRING(RHS)                          \
                            << " but "                                                              \
                            << ::tts::as_string(lhs) << " == " << ::tts::as_string(rhs)             \
                            << " within " << std::setprecision(PREC) << std::fixed                  \
                            << r << std::defaultfloat                                               \
                            << " " << UNIT << " when "                                              \
                            << std::setprecision(PREC) <<  std::fixed                               \
                            << N << std::defaultfloat                                               \
                            << " " << UNIT << " was expected."                                      \
            );                                                                                      \
    return ::tts::detail::logger{};                                                                 \
  }                                                                                                 \
}(LHS,RHS)                                                                                          \
/**/

#define TTS_PRECISION(L,R,N,U,F,P,...)      TTS_PRECISION_ ## __VA_ARGS__ (L,R,N,U,F,P)
#define TTS_PRECISION_(L,R,N,U,F,P)         TTS_PRECISION_IMPL(L,R,N,U,F,P,TTS_FAIL)
#define TTS_PRECISION_REQUIRED(L,R,N,U,F,P) TTS_PRECISION_IMPL(L,R,N,U,F,P,TTS_FATAL)

//======================================================================================================================
/**
  @def TTS_ABSOLUTE_EQUAL
  @brief Checks if the absolute distance between values is less or equal to a threshold

  This comparison is performed by using the proper tts::absolute_distance overload.

  @param L, R Expressions to compare.
  @param N    Maximum absolute distance accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE("Absolute distance")
  {
    TTS_ABSOLUTE_EQUAL(-2.  , 2.f  , 4.);
    TTS_ABSOLUTE_EQUAL('A'  , 80LL , 15, REQUIRED);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_ABSOLUTE_EQUAL(L,R,N,...) TTS_PRECISION(L,R,N,"unit", ::tts::absolute_distance, 8, __VA_ARGS__ )

//======================================================================================================================
/**
  @def TTS_RELATIVE_EQUAL
  @brief Checks if values are within a given relative distance expressed as a percentage.

  This comparison is performed by using the proper tts::relative_distance overload.

  @param L, R Expressions to compare.
  @param N    Maximum relative percentage accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE("Relative distance")
  {
    TTS_RELATIVE_EQUAL(42.f , 42.f  , 0     );
    TTS_RELATIVE_EQUAL('A'  , 80LL  , 18.75 );
    TTS_RELATIVE_EQUAL(1.   , 2.f   , 100.  );

    TTS_RELATIVE_EQUAL(1    , 10    , 900., REQUIRED  );
  };
  @endcode
**/
//======================================================================================================================
#define TTS_RELATIVE_EQUAL(L,R,N,...) TTS_PRECISION(L,R,N,"%"   , ::tts::relative_distance, 8, __VA_ARGS__ )

//======================================================================================================================
/**
  @def TTS_ULP_EQUAL
  @brief Checks if two values are within a given ULP distance

  This comparison is performed by using the proper tts::ulp_distance overload.

  @param L, R Expressions to compare.
  @param N    Maximum ULPs accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "ULP distance")
  {
    float a{1};
    float eps  = std::numeric_limits<float>::epsilon();
    float qnan = std::numeric_limits<float>::quiet_NaN();
    float inf  = std::numeric_limits<float>::infinity();
    float minf = -inf;

    TTS_ULP_EQUAL(a    , a     , 0.  );

    TTS_ULP_EQUAL(a    , qnan  , inf );
    TTS_ULP_EQUAL(qnan , qnan  , 0.  );

    TTS_ULP_EQUAL(a    , inf   , inf );
    TTS_ULP_EQUAL(inf  , inf   , inf );

    TTS_ULP_EQUAL(a, a-eps   , 1   );
    TTS_ULP_EQUAL(a, a+eps   , 0.5 );
    TTS_ULP_EQUAL(a, a+3*eps , 1.5, REQUIRED );
  };
  @endcode
**/
//======================================================================================================================
#define TTS_ULP_EQUAL(L,R,N,...)      TTS_PRECISION(L,R,N,"ULP" , ::tts::ulp_distance     , 2, __VA_ARGS__ )


#define TTS_DO_IEEE_EQUAL_IMPL(LHS, RHS, FAILURE)                                                   \
[&](auto lhs, auto rhs)                                                                             \
{                                                                                                   \
  if(::tts::is_ieee_equal(lhs,rhs))                                                                 \
  {                                                                                                 \
    ::tts::global_runtime.pass(); return ::tts::detail::logger{false};                              \
  }                                                                                                 \
  else                                                                                              \
  {                                                                                                 \
    FAILURE ( "Expected: " << TTS_STRING(LHS) << " == " << TTS_STRING(RHS)                          \
                            << " but "                                                              \
                            << ::tts::as_string(lhs) << " != " << ::tts::as_string(rhs)             \
            );                                                                                      \
    return ::tts::detail::logger{};                                                                 \
  }                                                                                                 \
}(LHS,RHS)                                                                                          \
/**/

#define TTS_DO_IEEE_EQUAL(L,R,...)    TTS_DO_IEEE_EQUAL_ ## __VA_ARGS__ (L,R)
#define TTS_DO_IEEE_EQUAL_(L,R)         TTS_DO_IEEE_EQUAL_IMPL(L,R,TTS_FAIL)
#define TTS_DO_IEEE_EQUAL_REQUIRED(L,R) TTS_DO_IEEE_EQUAL_IMPL(L,R,TTS_FATAL)

//======================================================================================================================
/**
  @def TTS_IEEE_EQUAL
  @brief Checks if two values are exactly within a 0 ULP.

  This also allow for infinites and NaNs to be compared equal if both values are the
  same infinites or are both $NaN$. This comparison is performed by using the proper tts::ulp_distance overload.

  @param L, R Expressions to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "ULP distance")
  {
    float x = std::numeric_limits<float>::quiet_NaN();
    TTS_IEEE_EQUAL(x,x);

    TTS_IEEE_EQUAL(1.f, 1.f);
    TTS_IEEE_EQUAL(2. , 2. );
    TTS_IEEE_EQUAL(65 , 'A');
  };
  @endcode
**/
//======================================================================================================================
#define TTS_IEEE_EQUAL(L,R,...)       TTS_DO_IEEE_EQUAL(L, R, __VA_ARGS__ )
