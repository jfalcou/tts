//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#ifndef TTS_TESTS_PRECISION_HPP_INCLUDED
#define TTS_TESTS_PRECISION_HPP_INCLUDED

#include <tts/detail/absdist.hpp>
#include <tts/detail/comparator.hpp>
#include <tts/detail/infos.hpp>
#include <tts/detail/precision.hpp>
#include <tts/detail/reldist.hpp>
#include <tts/detail/ulpdist.hpp>

//==================================================================================================
/**
  * @brief Checks if two values are within a given ULP distance
  *
  * **Required header:** `#include <tts/tests/precision.hpp>`
  *
  * **Example:**
  *
  * @snippet precision/ulp.cpp ulp-equal
  *
  * @note
  * ULP comparison is, by default, handled by the ``tts::ulpdist`` overload. In the case one
  * needs to perform a custom ULP comparison, see the
  * <a href="../extension.html#custom-ulp-comparison">Custom ULP Comparison</a> section.
  *
  *  @param LHS Expression to compare
  *  @param RHS Expression to compare
  *  @param ULP Maximum number of ULPs between the two values to compare
**/
//==================================================================================================
#define TTS_ULP_EQUAL(LHS, RHS, ULP)                                                               \
  do                                                                                               \
  {                                                                                                \
    auto eval_a = (LHS);                                                                           \
    auto eval_b = (RHS);                                                                           \
    ::tts::detail::check_precision(runtime,                                                        \
                                   {__FILE__, __LINE__},                                           \
                                   ::tts::ulpdist(eval_a, eval_b),                                 \
                                   ULP,                                                            \
                                   eval_a, eval_b,                                                 \
                                   TTS_STRING(LHS),                                                \
                                   TTS_STRING(RHS),                                                \
                                   "ULP");                                                         \
  } while(::tts::detail::is_false())                                                               \
/**/

//==================================================================================================
/**
  * @brief Checks if two values are equal at 0 ULP.
  *
  * **Required header:** `#include <tts/tests/precision.hpp>`
  *
  * Checks if the ULP distance between `LHS` and `RHS` is exactly equal to 0. Contrary to
  * @ref TTS_EQUAL, this test handles comparison with infinites or NaN values.
  *
  * **Example:**
  *
  * @snippet precision/ieee.cpp ieee-equal
  *
  * @note
  * ULP comparison is, by default, handled by the ``tts::ulpdist`` overload. In the case one
  * needs to perform a custom ULP comparison, see the
  * <a href="../extension.html#custom-ulp-comparison">Custom ULP Comparison</a> section.
  *
  *  @param LHS Expression to compare
  *  @param RHS Expression to compare
**/
//==================================================================================================
#define TTS_IEEE_EQUAL(LHS, RHS)      TTS_ULP_EQUAL(LHS, RHS, 0.)

//==================================================================================================
/**
  * @brief Checks if two values are within a given percentage
  *
  * **Required header:** `#include <tts/tests/precision.hpp>`
  *
  * Checks if the relative distance between `LHS` and `RHS` is less or equal to `P` percent.
  *
  * **Example:**
  *
  * @snippet precision/relative.cpp relative-equal
  *
  * @note
  * Relative comparison is, by default, handled by the ``tts::reldist`` overload. In the case one
  * needs to perform a custom relative distance comparison, see the
  * <a href="../extension.html#custom-relative-comparison">Custom Relative Comparison</a> section.
  *
  *  @param LHS Expression to compare
  *  @param RHS Expression to compare
  *  @param P   Maximum error percentage between the two values to compare
**/
//==================================================================================================
#define TTS_RELATIVE_EQUAL(LHS, RHS, P)                                                            \
  do                                                                                               \
  {                                                                                                \
    auto eval_a = (LHS);                                                                           \
    auto eval_b = (RHS);                                                                           \
    ::tts::detail::check_precision(runtime,                                                        \
                                   {__FILE__, __LINE__},                                           \
                                   ::tts::reldist(eval_a, eval_b),                                 \
                                   P,                                                              \
                                   eval_a, eval_b,                                                 \
                                   TTS_STRING(LHS),                                                \
                                   TTS_STRING(RHS),                                                \
                                   "%");                                                           \
  } while(::tts::detail::is_false())                                                               \
/**/


//==================================================================================================
/**
  * @brief Checks if the absolute distance between two values is under a given threshold
  *
  * **Required header:** `#include <tts/tests/precision.hpp>`
  *
  * Checks if the absolute distance between `LHS` and `RHS` is less or equal to `N`.
  *
  * **Example:**
  *
  * @snippet precision/relative.cpp relative-equal
  *
  * @note
  * Absolute comparison is, by default, handled by the ``tts::absdist`` overload. In the case one
  * needs to perform a custom absolute distance comparison, see the
  * <a href="../extension.html#custom-absolute-comparison">Custom Absolute Comparison</a> section.
  *
  *  @param LHS Expression to compare
  *  @param RHS Expression to compare
  *  @param N   Maximum absolute error between the two values to compare
**/
//==================================================================================================
#define TTS_ABSOLUTE_EQUAL(LHS, RHS, N)                                                            \
  do                                                                                               \
  {                                                                                                \
    auto eval_a = (LHS);                                                                           \
    auto eval_b = (RHS);                                                                           \
    ::tts::detail::check_precision(runtime,                                                        \
                                   {__FILE__, __LINE__},                                           \
                                   ::tts::absdist(eval_a, eval_b),                                 \
                                   N,                                                              \
                                   eval_a, eval_b,                                                 \
                                   TTS_STRING(LHS),                                                \
                                   TTS_STRING(RHS),                                                \
                                   " unit");                                                       \
  } while(::tts::detail::is_false())                                                               \
/**/

#endif
