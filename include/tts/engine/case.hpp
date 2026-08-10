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
    capture(char const* id) // NOSONAR
        : name(id)
    {
    }
    capture(tagged_id id) // NOSONAR
        : name(id.name)
        , tag(id.tag)
    {
    }
    auto operator+(auto body) const
    {
      return test::acknowledge({name, body, /*types=*/ {}, tag});
    }
    char const*             name;
    ::tts::expected_outcome tag = ::tts::expected_outcome::pass;
  };

  // Global storage for current type used in a given test
  inline text current_type = {};

  // Builds a ", "-joined list of type names, known at registration time so --dry can display it
  // without running the test.
  inline text joined_type_names()
  {
    return text {};
  }

  template<typename T, typename... Rest> inline text joined_type_names()
  {
    text out = as_text(typename_<T>);
    if constexpr(sizeof...(Rest) > 0)
    {
      out += ", ";
      out += joined_type_names<Rest...>();
    }
    return out;
  }

  template<typename... Types> struct captures
  {
    captures(char const* id) // NOSONAR
        : name(id)
    {
    }
    captures(tagged_id id) // NOSONAR
        : name(id.name)
        , tag(id.tag)
    {
    }

    auto operator+(auto body) const
    {
      // We register all the types in a single test to keep the compile-time O(1)
      // Registering different type in different tests generate far too much callable::invoker
      // symbol that make compile-time O(N)
      return test::acknowledge(
      {name,
       [ body ]()
       {
         // We setup the current type name before each test so we know
         (((current_type = as_text(typename_<Types>)),
           (::tts::is_detailed()
            ? (::tts::output().writeln(">  With <T = %s>", current_type.data()), 0)
            : 0),
           body(type<Types>())),
          ...);
         // Clear the current type
         current_type = text {""};
       },
       joined_type_names<Types...>(),
       tag});
    }
    char const*             name;
    ::tts::expected_outcome tag = ::tts::expected_outcome::pass;
  };

  // Specialisation for types lists
  template<typename... Types> struct captures<types<Types...>> : captures<Types...>
  {
  };

  // Specialisation for types list generator
  template<typename Generator>
    requires requires { typename Generator::types_list; }
  struct captures<Generator> : captures<typename Generator::types_list>
  {
  };

  template<typename Types, auto... Generators> struct test_generators;

  template<typename... Type, auto... Generators>
  struct test_generators<types<Type...>, Generators...>
  {
    char const*             name;
    ::tts::expected_outcome tag = ::tts::expected_outcome::pass;

    test_generators(char const* id) // NOSONAR
        : name(id)
    {
    }
    test_generators(tagged_id id) // NOSONAR
        : name(id.name)
        , tag(id.tag)
    {
    }

    template<typename... Args> static void process_call(auto body, Args&&... args)
    {
      body(std::forward<Args>(args)...);
    }

    template<typename T> static void process_type(auto body)
    {
      current_type = as_text(typename_<T>);
      if(::tts::is_detailed()) ::tts::output().writeln(">  With <T = %s>", current_type.data());
      process_call(body, produce(type<T> {}, Generators)...);
    }

    friend auto operator<<(test_generators tg, auto body)
    {
      return test::acknowledge({tg.name,
                                [ body ]() mutable
                                {
                                  (process_type<Type>(body), ...);
                                  current_type = text {""};
                                },
                                joined_type_names<Type...>(),
                                tg.tag});
    }
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
  ::tts::_::test_generators<::tts::as_type_list_t<TTS_REMOVE_PARENS(TYPES)>, __VA_ARGS__> {ID}     \
  << [] /**/
#endif

//======================================================================================================================
/**
  @def TTS_XFAIL
  @brief Tags a @ref TTS_CASE (or @ref TTS_CASE_TPL / @ref TTS_CASE_WITH) ID as expected to fail.

  Wraps the case's ID instead of changing the enclosing macro's signature, so it composes with
  @ref TTS_CASE, @ref TTS_CASE_TPL and @ref TTS_CASE_WITH unchanged: `TTS_CASE(TTS_XFAIL("..."))`.

  The case is expected to produce at least one failing assertion. If it runs with no failures
  instead, that mismatch is reported and fails the suite - matching the "X = expected" naming
  convention already used by other test frameworks (pytest's `xfail`, DejaGnu, Perl's
  `Test::More`), so it reads the same way to anyone coming from one of those.

  An empty case (no assertion ran at all) does not satisfy `TTS_XFAIL` either - use
  @ref TTS_XINVALID for that.

  @param ID A literal string describing the scenario intents.

  @see TTS_MAYFAIL
  @see TTS_XINVALID

  @groupheader{Example}
  @snippet doc/xfail.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_XFAIL(ID)
#else
#define TTS_XFAIL(ID) ::tts::expect_fail(ID)
#endif

//======================================================================================================================
/**
  @def TTS_MAYFAIL
  @brief Tags a @ref TTS_CASE (or @ref TTS_CASE_TPL / @ref TTS_CASE_WITH) ID as allowed to fail.

  Unlike @ref TTS_XFAIL, nothing specific is expected: the case may pass or fail, either is
  accepted and neither is reported. Useful for flagging a work-in-progress or a known issue
  that shouldn't block the suite without pretending to require a specific outcome.

  An empty case is still reported, exactly as for an untagged @ref TTS_CASE - `TTS_MAYFAIL`
  only relaxes the pass/fail expectation, not the "did anything actually run" one.

  @param ID A literal string describing the scenario intents.

  @see TTS_XFAIL
  @see TTS_XINVALID

  @groupheader{Example}
  @snippet doc/mayfail.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_MAYFAIL(ID)
#else
#define TTS_MAYFAIL(ID) ::tts::may_fail(ID)
#endif

//======================================================================================================================
/**
  @def TTS_XINVALID
  @brief Tags a @ref TTS_CASE (or @ref TTS_CASE_TPL / @ref TTS_CASE_WITH) ID as expected to be
empty.

  The case is expected to register no assertion at all. If it runs any assertion instead
  (passing or failing), that mismatch is reported and fails the suite.

  @param ID A literal string describing the scenario intents.

  @see TTS_XFAIL
  @see TTS_MAYFAIL

  @groupheader{Example}
  @snippet doc/xinvalid.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_XINVALID(ID)
#else
#define TTS_XINVALID(ID) ::tts::expect_invalid(ID)
#endif

//======================================================================================================================
/// @}
//======================================================================================================================

//======================================================================================================================
/// @}
//======================================================================================================================
