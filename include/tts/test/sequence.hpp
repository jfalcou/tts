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
#include <type_traits>

namespace tts::detail
{
  template<typename T, typename U> struct failure
  {
    std::size_t index;
    T original;
    U other;
  };
}

#define TTS_ALL_IMPL(SEQ1,SEQ2,OP,N,UNIT,FAILURE)                                                   \
[](auto const& local_tts_a, auto const& local_tts_b)                                                \
{                                                                                                   \
  if( std::size(local_tts_b) != std::size(local_tts_a) )                                            \
  {                                                                                                 \
    FAILURE ( "Expected: "  << TTS_STRING(SEQ1) << " == " << TTS_STRING(SEQ2)                       \
                            << " but sizes does not match: "                                        \
                            << "size(" TTS_STRING(SEQ1) ") = " << std::size(local_tts_a)            \
                            << " while size(" TTS_STRING(SEQ2) ") = " << std::size(local_tts_b)     \
            );                                                                                      \
    return ::tts::detail::logger{};                                                                 \
  }                                                                                                 \
                                                                                                    \
  auto ba = std::begin(local_tts_a);                                                                \
  auto bb = std::begin(local_tts_b);                                                                \
  auto ea = std::end(local_tts_a);                                                                  \
                                                                                                    \
  std::vector < ::tts::detail::failure< std::remove_cvref_t<decltype(*ba)>                          \
                                      , std::remove_cvref_t<decltype(*bb)>                          \
                                      >                                                             \
              > failures;                                                                           \
  std::size_t i = 0;                                                                                \
                                                                                                    \
  while(ba != ea)                                                                                   \
  {                                                                                                 \
    if( OP(*ba,*bb) > N )  failures.push_back({i++,*ba,*bb});                                       \
    ba++;                                                                                           \
    bb++;                                                                                           \
  }                                                                                                 \
                                                                                                    \
  if( !failures.empty( ) )                                                                          \
  {                                                                                                 \
    FAILURE ( "Expected: "  << TTS_STRING(SEQ1) << " == " << TTS_STRING(SEQ2)                       \
                            << " but values differ by more than " << N << " "<< UNIT                \
            );                                                                                      \
                                                                                                    \
    for(auto f : failures)                                                                          \
      std::cout << "    @[" << f.index << "] : " << f.original << " and " << f.other                \
                << " differ by " << OP(f.original,f.other) << " " << UNIT << "\n";                  \
                                                                                                    \
    std::cout << "\n";                                                                              \
    return ::tts::detail::logger{};                                                                 \
  }                                                                                                 \
                                                                                                    \
  ::tts::global_runtime.pass();                                                                     \
  return ::tts::detail::logger{false};                                                              \
}(SEQ1, SEQ2)                                                                                       \
/**/

#define TTS_ALL(L,R,F,N,U, ...)     TTS_ALL_ ## __VA_ARGS__ (L,R,F,N,U)
#define TTS_ALL_(L,R,F,N,U)         TTS_ALL_IMPL(L,R,F,N,U,TTS_FAIL)
#define TTS_ALL_REQUIRED(L,R,F,N,U) TTS_ALL_IMPL(L,R,F,N,U,TTS_FATAL)

//======================================================================================================================
/**
  @def TTS_ALL_ABSOLUTE_EQUAL
  @brief Checks if all elements of two sequences are within a given absolute distance  and that their sizes are equal.

  This comparison is performed by using the proper tts::absolute_distance overload.

  @param L, R Sequences to compare.
  @param N    Maximum absolute distance accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <vector>
  #include <list>

  TTS_CASE("Absolute distance over sequences")
  {
    std::vector v{1.f,2.f,3.f,-1.f};
    std::list   w{1.f,7.f,3.f,-5.f};

    TTS_ALL_ABSOLUTE_EQUAL(v,w,5);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_ALL_ABSOLUTE_EQUAL(L,R,N,...) TTS_ALL(L,R, ::tts::absolute_distance,N,"unit", __VA_ARGS__ )

//======================================================================================================================
/**
  @def TTS_ALL_RELATIVE_EQUAL
  @brief Checks if all elements of two sequences are within a given relative percentage and that their sizes are equal.

  This comparison is performed by using the proper tts::relative_distance overload.

  @param L, R Sequences to compare.
  @param N    Maximum relative percentage accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <vector>
  #include <list>

  TTS_CASE("Relative distance over sequences")
  {
    std::vector v{1.f,2.f,3.f,-5.f};
    std::list   w{1.f,2.f,3.1f,-5.f};

    TTS_ALL_RELATIVE_EQUAL(v,w,3.33);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_ALL_RELATIVE_EQUAL(L,R,N,...) TTS_ALL(L,R, ::tts::relative_distance,N,"%"   , __VA_ARGS__ )

//======================================================================================================================
/**
  @def TTS_ALL_ULP_EQUAL
  @brief Checks if all elements of two sequences are within a given ULP distance  and that their sizes are equal.

  This comparison is performed by using the proper tts::ulp_distance overload.

  @param L, R Sequences to compare.
  @param N    Maximum ULPs accepted between `L` and `R`.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <vector>
  #include <list>

  TTS_CASE( "ULP distance over sequences")
  {
    std::vector v{1.f       , 2.f, 3.f, -5.f};
    std::list   w{1.f+1e-7f , 2.f, 3.f, -5.f};

    TTS_ALL_ULP_EQUAL(v,w,0.5);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_ALL_ULP_EQUAL(L,R,N,...)      TTS_ALL(L,R, ::tts::ulp_distance     ,N,"ULP" , __VA_ARGS__ )

//======================================================================================================================
/**
  @def TTS_ALL_IEEE_EQUAL
  @brief Checks if all elements of two sequences are within 0 ULP and that their sizes are equal.

  This also allow for infinites and NaNs to be compared equal if both values are the
  same infinites or are both $NaN$. This comparison is performed by using the proper tts::ulp_distance overload.

  @param L, R Sequences to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <vector>
  #include <list>

  TTS_CASE( "IEEE distance over sequences")
  {
    float x = std::numeric_limits<float>::quiet_NaN();
    std::vector v{1.f, 2.f, 3.f, x};
    std::list   w{1.f, 2.f, 3.f, x};

    TTS_ALL_IEEE_EQUAL(v,w);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_ALL_IEEE_EQUAL(L,R,...)     TTS_ALL_ULP_EQUAL(L,R,0, __VA_ARGS__)

//======================================================================================================================
/**
  @def TTS_ALL_EQUAL
  @brief Checks if all elements of two sequences are all equal and that their sizes are equal.

  This comparison is performed by using the proper tts::absolute_distance overload.

  @param L, R Sequences to compare.
  @param ...  Optional tag. If equals to `REQUIRED`, this test will stop the program if it fails.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <vector>
  #include <list>

  TTS_CASE("Equality over sequences")
  {
    std::vector v{1.f,2.f,3.f,-5.f};
    std::list   w{1.f,2.f,3.f,-5.f};

    TTS_ALL_EQUAL(v,w);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_ALL_EQUAL(L,R,...)            TTS_ALL_ABSOLUTE_EQUAL(L,R, 0 __VA_ARGS__ )
