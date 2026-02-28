//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/info.hpp>
#include <tts/engine/test.hpp>
#include <tts/engine/generator.hpp>
#include <tts/tools/options.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/tools/types.hpp>

//======================================================================================================================
// "Seed" capture
//======================================================================================================================
namespace tts::_
{
  struct capture
  {
    capture(char const* id)
        : name(id)
    {
    }
    auto        operator+(auto body) const { return test::acknowledge({name, body}); }
    char const* name;
  };

  // Global storage for current type used in a given test
  inline text current_type = {};

  template<typename... Types> struct captures
  {
    captures(char const* id)
        : name(id)
    {
    }

    auto operator+(auto body) const
    {
      // We register all the types in a single test to keep the compile-time O(1)
      // Registering different type in different tests generate far too much callable::invoker
      // symbol that make compile-time O(N)
      return test::acknowledge({name,
                                [ = ]()
                                {
                                  // We setup the current type name before each test so we know
                                  (((current_type = as_text(typename_<Types>)),
                                    (::tts::_::is_verbose && !::tts::_::is_quiet
                                         ? printf(">  With <T = %s>\n", current_type.data())
                                         : 0),
                                    body(type<Types>())),
                                   ...);
                                  // Clear the current type
                                  current_type = text {""};
                                }});
    }
    char const* name;
  };

  // Specialisation for types lists
  template<typename... Types> struct captures<types<Types...>> : captures<Types...>
  {
  };

  // Specialisation for types list generator
  template<typename Generator>
    requires requires(Generator) { typename Generator::types_list; }
  struct captures<Generator> : captures<typename Generator::types_list>
  {
  };

  template<typename Types, auto... Generators> struct test_generators;

  template<typename... Type, auto... Generators>
  struct test_generators<types<Type...>, Generators...>
  {
    test_generators(char const* id)
        : name(id)
    {
    }
    friend auto operator<<(test_generators tg, auto body)
    {
      return test::acknowledge({tg.name,
                                [ body ]() mutable
                                {
                                  (process_type<Type>(body), ...);
                                  current_type = text {""};
                                }});
    }

    template<typename T> static void process_type(auto body)
    {
      current_type = as_text(typename_<T>);
      if(::tts::_::is_verbose && !::tts::_::is_quiet)
        printf(">  With <T = %s>\n", current_type.data());
      (body(produce(type<T> {}, Generators)...));
    }
    char const* name;
  };
}

//======================================================================================================================
/**
  @defgroup test-scenario Tests definition macros
  @brief Macros for defining tests scenarios.
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @name Scenarios Definitions
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @def TTS_CASE
  @brief Introduces a new test scenario and registers it into the current test driver.

  The code block following TTS_CASE contains user-defined code for a given test case.
  Test cases performing no actual tests will be reported as invalid.

  @param ID A literal string describing the scenario intents.

  @groupheader{Example}
  @snippet doc/case.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CASE(ID)
#else
#define TTS_CASE(ID)                                                                               \
  [[maybe_unused]] static auto const TTS_CAT(case_, TTS_FUNCTION) =                                \
      ::tts::_::capture {ID} + +[]() /**/
#endif

//======================================================================================================================
/**
  @def TTS_CASE_TPL
  @brief Introduces a template test case  and registers it into the current test driver.

  The code block following TTS_CASE contains user-defined code for a given test case.
  Those tests are parametrized by a template type of your choice passed as lambda function
  parameters of the template type `tts::type` and instantiated for each type in the types list.

  Such types list can be provided as:
  + a variadic list of types separated by commas
  + an instance of the tts::types template class
  + an instance of a Type Generator, ie a type exposing a public `types_list` type definition

  Test cases performing no actual tests will be reported as invalid.

  @param ID   A literal string describing the scenario intents.
  @param ...  Lists of types to generate the test case from.

  @groupheader{Example}
  @snippet doc/case_tpl.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CASE_TPL(ID, ...)
#else
#define TTS_CASE_TPL(ID, ...)                                                                      \
  [[maybe_unused]] static bool const TTS_CAT(case_, TTS_FUNCTION) =                                \
      ::tts::_::captures<__VA_ARGS__> {ID} + [] /**/
#endif

//======================================================================================================================
/**
  @def TTS_CASE_WITH
  @brief Introduces a template test case providing dynamically generated data to the test code.

  The following code block will contain tests parametrized by a template type of your choice passed
  as lambda function parameters and generated for each type in the types list.

  Such types list can be provided as:
    + a parenthesised list of types separated by comma.
    + an instance of the tts::types template class.
    + an instance of a Type Generator, ie a type exposing a public `types_list` type definition

  Test cases performing no actual tests will be reported as invalid.

  @param ID         A literal string describing the scenario intents.
  @param TYPES      Lists of types to generate the test case from.
  @param ...        Lists of generator function

  @groupheader{Example}
  @snippet doc/case_with.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CASE_WITH(ID, TYPES, ...)
#else
#define TTS_CASE_WITH(ID, TYPES, ...)                                                              \
  [[maybe_unused]] static bool const TTS_CAT(case_, TTS_FUNCTION) =                                \
      ::tts::_::test_generators<::tts::as_type_list_t<TTS_REMOVE_PARENS(TYPES)>, __VA_ARGS__> {ID} \
      << [] /**/
#endif

//======================================================================================================================
/// @}
//======================================================================================================================

//======================================================================================================================
/// @}
//======================================================================================================================
