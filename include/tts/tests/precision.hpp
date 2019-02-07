//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_PRECISION_HPP_INCLUDED
#define TTS_TESTS_PRECISION_HPP_INCLUDED

#include <tts/detail/decompose.hpp>
#include <tts/tests/basic.hpp>
#include <tts/tests/impl/absolute.hpp>
#include <tts/tests/impl/relative.hpp>
#include <tts/tests/impl/ulp.hpp>

/*!
  @ingroup group-unit

  @brief Check for equality within ULP distance

  Evaluates @c A and @c B and checks if their respective value are withing @c X ULPs of
  each other.

  @par Example:

  @snippet test/unit/ulp.cpp ulp

  @param A First expression to compare
  @param B Second expression to compare
  @param X ULP distance threshold
**/
#define TTS_ULP_EQUAL(A, B, X)                                                                     \
  do                                                                                               \
  {                                                                                                \
    auto tts_var_r = ::tts::ulpdist((A), (B));                                                     \
    auto tts_var_d = TTS_DECOMPOSE((A) == (B));                                                    \
    if(tts_var_r <= (X))                                                                           \
      TTS_PASS("Expecting: " << ::tts::detail::white_(tts_var_d.lhs) << " == "                     \
                             << ::tts::detail::white_(tts_var_d.rhs) << " within "                 \
                             << ::tts::detail::white_(X) << " ULPs");                              \
    else                                                                                           \
      TTS_FAIL("Expecting: " << ::tts::detail::white_(tts_var_d.lhs) << " == "                     \
                             << ::tts::detail::white_(tts_var_d.rhs) << " within "                 \
                             << ::tts::detail::white_(X) << " ULPs but found: "                    \
                             << ::tts::detail::red_(tts_var_r)                                     \
                             << " ULPs instead.");                                                 \
  } while(::tts::detail::is_false()) /**/

/*!
  @ingroup group-unit

  @brief Check for equality within IEEE rules

  Evaluates @c A and @c B and checks if their respective value(s) are equal at 0 ULPs or
  are both NaNs.

  @par Example:

  @snippet test/unit/ulp.cpp ieee

  @param A First expression to compare
  @param B Second expression to compare
**/
#define TTS_IEEE_EQUAL(A, B) TTS_ULP_EQUAL(A, B, 0.)

/*!
  @ingroup group-unit

  @brief Check for equality within ULP distance in ranges

  Evaluates @c A and @c B and checks if their respective contents are withing @c X ULPs of
  each other. If @c A or @c B are single values, the other must be a container of size 1.

  @par Example:

  @snippet test/unit/all_ulp.cpp all_ulp

  @param A First range expression to compare
  @param B Second range expression to compare
  @param X ULP distance threshold
**/
#define TTS_ALL_ULP_EQUAL(A, B, X)                                                                 \
  do                                                                                               \
  {                                                                                                \
    auto tts_var_r = TTS_DECOMPOSE((A) == ::tts::ulp(B, X));                                       \
    if(tts_var_r)                                                                                  \
      TTS_PASS("Expecting: " << ::tts::detail::white_(TTS_STRING(A)) << " == "                     \
                             << ::tts::detail::white_(TTS_STRING(B)) << " within "                 \
                             << ::tts::detail::white_(X) << " ULPs");                              \
    else                                                                                           \
      TTS_FAIL("Expecting: " << ::tts::detail::white_(TTS_STRING(A)) << " == "                     \
                             << ::tts::detail::white_(TTS_STRING(B)) << " within "                 \
                             << ::tts::detail::white_(X) << " ULPs "                               \
                             << "but found: " << tts_var_r.rhs << " instead.");                    \
  } while(::tts::detail::is_false()) /**/

/*!
  @ingroup group-unit

  @brief Check for equality within IEEE rules in ranges

  Evaluates @c A and @c B and checks if their respective contents are equal at 0 ULPs or
  are both NaNs. If @c A or @c B are single values, the other must be a container of size 1.

  @par Example:

  @snippet test/unit/ulp.cpp ieee

  @param A First range expression to compare
  @param B Second range expression to compare
**/
#define TTS_ALL_IEEE_EQUAL(A, B) TTS_ALL_ULP_EQUAL(A, B, 0.)

/*!
  @ingroup group-unit

  @brief Check for equality within a relative distance

  Evaluates @c A and @c B and checks if their respective value(s) are within a relative tolerance
  of @c X percent of each other.

  @par Example:

  @snippet test/unit/relative.cpp relative

  @param A First expression to compare
  @param B Second expression to compare
  @param X Relative tolerance
**/
#define TTS_RELATIVE_EQUAL(A, B, X)                                                                \
  do                                                                                               \
  {                                                                                                \
    auto tts_var_r = ::tts::reldist((A), (B));                                                     \
    auto tts_var_d = TTS_DECOMPOSE((A) == (B));                                                    \
    if(tts_var_r <= (X / 100.))                                                                    \
      TTS_PASS("Expecting: " << ::tts::detail::white_(tts_var_d.lhs) << " == "                     \
                             << ::tts::detail::white_(tts_var_d.rhs) << " within "                 \
                             << ::tts::detail::white_(X) << " %");                                 \
    else                                                                                           \
      TTS_FAIL("Expecting: " << ::tts::detail::white_(tts_var_d.lhs) << " == "                     \
                             << ::tts::detail::white_(tts_var_d.rhs) << " within "                 \
                             << ::tts::detail::white_(X) << " % but found: "                       \
                             << ::tts::detail::red_(100 * tts_var_r)                               \
                             << " % instead.");                                                    \
  } while(::tts::detail::is_false()) /**/

/*!
  @ingroup group-unit

  @brief Check for equality within a relative distance in ranges

  Evaluates @c A and @c B and checks if their respective contents are within a relative tolerance
  of @c X percent of each other. If @c A or @c B are single values, the other must be a container
  of size 1.

  @par Example:

  @snippet test/unit/all_relative.cpp all_relative

  @param A First expression to compare
  @param B Second expression to compare
  @param X Relative tolerance
**/
#define TTS_ALL_RELATIVE_EQUAL(A, B, X)                                                            \
  do                                                                                               \
  {                                                                                                \
    auto tts_var_r = TTS_DECOMPOSE((A) == ::tts::relative(B, X));                                  \
    if(tts_var_r)                                                                                  \
      TTS_PASS("Expecting: " << ::tts::detail::white_(TTS_STRING(A)) << " == "                     \
                             << ::tts::detail::white_(TTS_STRING(B)) << " within "                 \
                             << ::tts::detail::white_(X) << " % ");                                \
    else                                                                                           \
      TTS_FAIL("Expecting: " << ::tts::detail::white_(TTS_STRING(A)) << " == "                     \
                             << ::tts::detail::white_(TTS_STRING(B)) << " within "                 \
                             << ::tts::detail::white_(X) << " % "                                  \
                             << "but found: " << tts_var_r.rhs << " instead.");                    \
  } while(::tts::detail::is_false()) /**/

/*!
  @ingroup group-unit

  @brief Check for strict equality in ranges

  Evaluates @c A and @c B and checks if their respective contents are strictly equals.
  If @c A or @c B are single values, the other must be a container of size 1.

  @par Example:

  @snippet test/unit/all_relative.cpp all_relative

  @param A First expression to compare
  @param B Second expression to compare
**/
#define TTS_ALL_EQUAL(A, B) TTS_ALL_RELATIVE_EQUAL(A, B, 0)

/*!
  @ingroup group-unit

  @brief Check for equality within a absolute distance

  Evaluates @c A and @c B and checks if their respective value(s) are within a absolute tolerance
  of @c X.

  @par Example:

  @snippet test/unit/absolute.cpp absolute

  @param A First expression to compare
  @param B Second expression to compare
  @param X Absolute tolerance
**/
#define TTS_ABSOLUTE_EQUAL(A, B, X)                                                                \
  do                                                                                               \
  {                                                                                                \
    auto tts_var_r = ::tts::absdist((A), (B));                                                     \
    auto tts_var_d = TTS_DECOMPOSE((A) == (B));                                                    \
    if(tts_var_r <= (X))                                                                           \
      TTS_PASS("Expecting: " << ::tts::detail::white_(tts_var_d.lhs) << " == "                     \
                             << ::tts::detail::white_(tts_var_d.rhs) << " within ~"                \
                             << ::tts::detail::white_(X));                                         \
    else                                                                                           \
      TTS_FAIL("Expecting: " << ::tts::detail::white_(tts_var_d.lhs) << " == "                     \
                             << ::tts::detail::white_(tts_var_d.rhs) << " within ~"                \
                             << ::tts::detail::white_(X) << " but was found ~"                     \
                             << ::tts::detail::red_(tts_var_r)                                     \
                             << " instead.");                                                      \
  } while(::tts::detail::is_false()) /**/

/*!
  @ingroup group-unit

  @brief Check for equality within a absolute distance in ranges

  Evaluates @c A and @c B and checks if their respective contents are within a
  absolute tolerance of @c X. If @c A or @c B are single values, the other must
  be a container of size 1.

  @par Example:

  @snippet test/unit/all_absolute.cpp all_absolute

  @param A First expression to compare
  @param B Second expression to compare
  @param X Absolute tolerance
**/
#define TTS_ALL_ABSOLUTE_EQUAL(A, B, X)                                                            \
  do                                                                                               \
  {                                                                                                \
    auto tts_var_r = TTS_DECOMPOSE((A) == ::tts::absolute(B, X));                                  \
    if(tts_var_r)                                                                                  \
      TTS_PASS("Expecting: " << ::tts::detail::white_(TTS_STRING(A)) << " == "                     \
                             << ::tts::detail::white_(TTS_STRING(B)) << " within ~"                \
                             << ::tts::detail::white_(X))          ;                               \
    else                                                                                           \
      TTS_FAIL("Expecting: " << ::tts::detail::white_(TTS_STRING(A)) << " == "                     \
                             << ::tts::detail::white_(TTS_STRING(B)) << " within ~"                \
                             << ::tts::detail::white_(X)                                           \
                             << " but found: " << tts_var_r.rhs << " instead.");                   \
  } while(::tts::detail::is_false()) /**/

#endif
