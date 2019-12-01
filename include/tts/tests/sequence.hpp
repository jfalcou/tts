//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#ifndef TTS_TESTS_SEQUENCE_HPP_INCLUDED
#define TTS_TESTS_SEQUENCE_HPP_INCLUDED

#include <tts/tests/precision.hpp>
#include <tts/detail/infos.hpp>

//==================================================================================================
/**
  * @brief ULP distance check for sequences
  *
  * **Required header:** `#include <tts/tests/sequence.hpp>`
  *
  * Checks if all values of two containers are within a given ULP distance and if both arguments
  * have the same size.
  *
  * **Example:**
  *
  * @snippet sequence/ulp.cpp all-ulp-equal
  *
  *  @param LHS Expression to compare
  *  @param RHS Expression to compare
  *  @param ULP Maximum number of ULPs between all the values to compare
**/
//==================================================================================================
#define TTS_ALL_ULP_EQUAL(LHS, RHS, ULP)                                                           \
  do                                                                                               \
  {                                                                                                \
    bool ok = ::tts::detail::has_matching_size(runtime,                                            \
                                               {__FILE__, __LINE__},                               \
                                               TTS_STRING(LHS),                                    \
                                               TTS_STRING(RHS),                                    \
                                               ::tts::detail::size(LHS),                           \
                                               ::tts::detail::size(RHS));                          \
    if(ok)                                                                                         \
    {                                                                                              \
      auto r =                                                                                     \
      ::tts::detail::all_check(LHS, RHS, ULP, [](auto a, auto b) { return ::tts::ulpdist(a, b); });\
      ::tts::detail::report_all_errors(                                                            \
          runtime, {__FILE__, __LINE__}, r, ULP, TTS_STRING(LHS), TTS_STRING(RHS), "ULP");         \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

//==================================================================================================
/**
  * @brief Exact ULP equality check for sequences
  *
  * **Required header:** `#include <tts/tests/sequence.hpp>`
  *
  * Checks if all values of two containers are within 0 ULP distance and if both arguments
  * have the same size.
  *
  * **Example:**
  *
  * @snippet sequence/ieee.cpp all-ieee-equal
  *
  *  @param LHS Expression to compare
  *  @param RHS Expression to compare
**/
//==================================================================================================
#define TTS_ALL_IEEE_EQUAL(LHS, RHS)  TTS_ALL_ULP_EQUAL(LHS, RHS, 0.)

//==================================================================================================
/**
  * @brief Relative error check for sequences
  *
  * **Required header:** `#include <tts/tests/sequence.hpp>`
  *
  * Checks if all values of two containers are within a given error percentage and if both arguments
  * have the same size.
  *
  * **Example:**
  *
  * @snippet sequence/ulp.cpp all-ulp-equal
  *
  *  @param LHS Expression to compare
  *  @param RHS Expression to compare
  *  @param P   Maximum error percentage between all the values to compare
**/
//==================================================================================================
#define TTS_ALL_RELATIVE_EQUAL(LHS, RHS, P)                                                        \
  do                                                                                               \
  {                                                                                                \
    bool ok = ::tts::detail::has_matching_size(runtime,                                            \
                                               {__FILE__, __LINE__},                               \
                                               TTS_STRING(LHS),                                    \
                                               TTS_STRING(RHS),                                    \
                                               ::tts::detail::size(LHS),                           \
                                               ::tts::detail::size(RHS));                          \
    if(ok)                                                                                         \
    {                                                                                              \
      auto r =                                                                                     \
      ::tts::detail::all_check(LHS, RHS, P, [](auto a, auto b) { return ::tts::reldist(a, b); });  \
      ::tts::detail::report_all_errors(                                                            \
          runtime, {__FILE__, __LINE__}, r, P, TTS_STRING(LHS), TTS_STRING(RHS), "%");             \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

//==================================================================================================
/**
  * @brief Equality check for sequences
  *
  * **Required header:** `#include <tts/tests/sequence.hpp>`
  *
  * Checks if all values of two containers are equals and have the same size.
  *
  * **Example:**
  *
  * @snippet sequence/equal.cpp all-equal
  *
  *  @param LHS Expression to compare
  *  @param RHS Expression to compare
**/
//==================================================================================================
#define TTS_ALL_EQUAL(LHS, RHS) TTS_ALL_RELATIVE_EQUAL(LHS, RHS, 0)


//==================================================================================================
/**
  * @brief Absolute distance check for sequences
  *
  * **Required header:** `#include <tts/tests/sequence.hpp>`
  *
  * Checks if all values of two containers are within a given absolute distance and have the same
  * size.
  *
  * **Example:**
  *
  * @snippet sequence/relative.cpp all-relative-equal
  *
  *  @param LHS Expression to compare
  *  @param RHS Expression to compare
  *  @param X   Maximum error between all the values to compare
**/
//==================================================================================================
#define TTS_ALL_ABSOLUTE_EQUAL(LHS, RHS, X)                                                        \
  do                                                                                               \
  {                                                                                                \
    bool ok = ::tts::detail::has_matching_size(runtime,                                            \
                                               {__FILE__, __LINE__},                               \
                                               TTS_STRING(LHS),                                    \
                                               TTS_STRING(RHS),                                    \
                                               ::tts::detail::size(LHS),                           \
                                               ::tts::detail::size(RHS));                          \
    if(ok)                                                                                         \
    {                                                                                              \
      auto r =                                                                                     \
      ::tts::detail::all_check(LHS, RHS, X, [](auto a, auto b) { return ::tts::absdist(a, b); });  \
      ::tts::detail::report_all_errors(                                                            \
          runtime, {__FILE__, __LINE__}, r, X, TTS_STRING(LHS), TTS_STRING(RHS), " unit");         \
    }                                                                                              \
  } while(::tts::detail::is_false())
/**/

#endif
