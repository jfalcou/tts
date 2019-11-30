//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//=====================================================================================================
#ifndef TTS_CASE_HPP_INCLUDED
#define TTS_CASE_HPP_INCLUDED

#include <tts/detail/color.hpp>
#include <tts/detail/ct_helpers.hpp>
#include <tts/detail/pp_helpers.hpp>
#include <tts/detail/rt_helpers.hpp>
#include <tts/detail/test.hpp>
#include <tts/engine/env.hpp>

//==================================================================================================
/**
  * @brief Introduces new test scenario and registers it into the current test driver.
  *
  * **Required header:** `#include <tts/case.hpp>`
  *
  *  The following block will contains user defined preparation, actual tests, expectations and
  *  clean-up for said test case.
  *
  *  **Example:**
  *
  *  @snippet simple.cpp tts_case
  *
  *  @note
  *  Test cases performing no actual tests swill be reported as invalid.
  *
  *  @param Description A literal string describing the scenario intents.
**/
//==================================================================================================
#define TTS_CASE(Description)                                                                      \
  void TTS_CALLER(::tts::env &, int&, int&);                                                       \
                                                                                                   \
  void TTS_FUNCTION(::tts::env & runtime)                                                          \
  {                                                                                                \
    runtime.output() << std::string(80, '-') << std::endl;                                         \
    runtime.output() << "[SCENARIO] - " << ::tts::detail::yellow_(Description) << std::endl;       \
    runtime.output() << std::string(80, '-') << std::endl;                                         \
    for(int tts_section = 0, tts_cnt = 1; tts_section < tts_cnt; tts_cnt -= 0 == tts_section++)    \
    {                                                                                              \
      TTS_CALLER(runtime,tts_section,tts_cnt);                                                     \
    }                                                                                              \
  }                                                                                                \
  namespace                                                                                        \
  {                                                                                                \
    static bool TTS_REGISTRATION =                                                                 \
        ::tts::detail ::registration(::tts::detail::test(Description, TTS_FUNCTION));              \
  }                                                                                                \
  void TTS_CALLER(::tts::env &runtime, int& tts_section, int& tts_cnt)                             \
/**/

//==================================================================================================
/**
  * @brief Starts a block defining a template test case.
  *
  * **Required header:** `#include <tts/case.hpp>`
  *
  * The following block will contains user defined preparation, actual tests, expectations and
  * clean-up for said test case parametrized by a template type named @c T and instantiated for
  * each type in the types sequence.
  *
  *  **Example:**
  *
  *  @snippet simple.cpp tts_case_tpl
  *
  *  @note
  *  Test cases performing no actual tests swill be reported as invalid.
  *
  *  @param Description A literal string describing the scenario intents.
  *  @param ...         Lists of types to generate the test case from
**/
//==================================================================================================
#define TTS_CASE_TPL(Description, ...)                                                             \
  template<typename T> void TTS_CALLER(::tts::env &, int&, int&);                                  \
                                                                                                   \
  template<typename T> void TTS_FUNCTION(::tts::env & runtime)                                     \
  {                                                                                                \
    for(int tts_section = 0, tts_cnt = 1; tts_section < tts_cnt; tts_cnt -= 0 == tts_section++)    \
    {                                                                                              \
      TTS_CALLER<T>(runtime,tts_section,tts_cnt);                                                  \
    }                                                                                              \
  }                                                                                                \
  namespace                                                                                        \
  {                                                                                                \
    static bool TTS_REGISTRATION =                                                                 \
        ::tts::detail::registration(::tts::detail::test(Description, [](::tts::env &runtime) {     \
          ::tts::detail::for_each_type(                                                            \
              [&](auto t) {                                                                        \
                using T = typename decltype(t)::type;                                              \
                runtime.output() << std::string(80, '-') << std::endl;                             \
                runtime.output() << "[SCENARIO] - " << ::tts::detail::yellow_(Description);        \
                runtime.output() << " with T = [" << ::tts::detail::magenta_(::tts::type_id<T>())  \
                                 << "] " << std::endl;                                             \
                runtime.output() << std::string(80, '-') << std::endl;                             \
                TTS_FUNCTION<T>(runtime);                                                          \
              },                                                                                   \
              ::tts::detail::typelist<__VA_ARGS__> {});                                            \
        }));                                                                                       \
  }                                                                                                \
  template<typename T>                                                                             \
  void TTS_CALLER(::tts::env &runtime, int& tts_section, int& tts_cnt)                             \
/**/

//==================================================================================================
/**
  * @brief Defines a nested test case.
  *
  * **Required header:** `#include <tts/case.hpp>`
  *
  * Classic approach for test design is to use fixtures to specify how data required by multiples
  * test should be constructed and/or destructed. This leads to a very object-oriented style of
  * tests which may be inconvenient.
  *
  * Using nested cases, one can define a common set of code to be run before any nested cases is run
  * itself, thus enforcing the fact that each nested cases is run which similar setup.
  *
  * Nested cases can be nested to an arbitrary depth and no nested cases of a given depth can
  * interact with each other.

  *  **Example:**
  *
  *  @snippet sections.cpp tts_subcase
  *
  *  @note
  *  Test cases performing no actual tests swill be reported as invalid.
  *
  *  @param Description A literal string describing the scenario intents.
**/
//==================================================================================================
#define TTS_SUBCASE(Description)                                                                   \
  static int TTS_UNIQUE(id) = 0;                                                                   \
  if(::tts::detail::section_guard(TTS_UNIQUE(id), tts_section, tts_cnt)                            \
         .check(Description, runtime))                                                             \
    for(int TTS_UNIQUE(tts_cnt) = 0; TTS_UNIQUE(tts_cnt) < 1; TTS_UNIQUE(tts_cnt)++) /**/

#endif
