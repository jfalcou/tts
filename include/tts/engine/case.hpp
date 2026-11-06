//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/context.hpp>
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
        , timeout_ns(id.timeout_ns)
        , timeout_set(id.timeout_set)
    {
    }
    auto operator+(auto body) const
    {
      return test::acknowledge({name, body, /*types=*/ {}, tag, timeout_ns, timeout_set});
    }
    char const*             name;
    ::tts::expected_outcome tag         = ::tts::expected_outcome::pass;
    nanoseconds             timeout_ns  = 0;
    bool                    timeout_set = false;
  };

  // A case run in the contexts named on its title. The body is a generic lambda on the context,
  // instantiated once per context, and each context runs it its own way.
  template<typename... Contexts> struct capture_within
  {
    capture_within(char const* id, Contexts...) // NOSONAR
        : name(id)
    {
    }
    capture_within(tagged_id id, Contexts...) // NOSONAR
        : name(id.name)
        , tag(id.tag)
        , timeout_ns(id.timeout_ns)
        , timeout_set(id.timeout_set)
    {
    }
    auto operator+(auto body) const
    {
      return test::acknowledge(
      {name,
       [ body ]() { (Contexts::template run<direct, has_runtime<Contexts...>>(body), ...); },
       /*types=*/ {},
       tag,
       timeout_ns,
       timeout_set});
    }
    char const*             name;
    ::tts::expected_outcome tag         = ::tts::expected_outcome::pass;
    nanoseconds             timeout_ns  = 0;
    bool                    timeout_set = false;
  };

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
        , timeout_ns(id.timeout_ns)
        , timeout_set(id.timeout_set)
    {
    }
    // The condition depends on Context so that the assertion fires on a use alone: under C++20 a
    // static_assert(false) in a template fires at its definition.
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
         (
         ((current_type = as_text(typename_<Types>)),
          (::tts::is_detailed()
           ? (::tts::output().writeln(">  With <T = %s>", current_type.data()), 0)
           : 0),
          body(type<Types>())),
         ...);
         // Clear the current type
         current_type = text {""};
       },
       joined_type_names<Types...>(),
       tag,
       timeout_ns,
       timeout_set});
    }
    char const*             name;
    ::tts::expected_outcome tag         = ::tts::expected_outcome::pass;
    nanoseconds             timeout_ns  = 0;
    bool                    timeout_set = false;
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

  // A template case run in the contexts named on its title: each type, then each context.
  template<typename Types, typename... Contexts> struct captures_within;

  template<typename... Types, typename... Contexts>
  struct captures_within<types<Types...>, Contexts...>
  {
    char const*                      name;
    ::tts::expected_outcome          tag         = ::tts::expected_outcome::pass;
    nanoseconds                      timeout_ns  = 0;
    bool                             timeout_set = false;

    template<typename T> static void process_type(auto body)
    {
      static_assert(std::is_invocable_v<decltype(body)&, runtime_context&, type<T>>,
                    "A case run in a context takes TTS_CONTEXT(...) as its parameter list.");
      current_type = as_text(typename_<T>);
      if(::tts::is_detailed()) ::tts::output().writeln(">  With <T = %s>", current_type.data());
      (Contexts::template run<with_type<T>, has_runtime<Contexts...>>(body), ...);
    }

    auto operator+(auto body) const
    {
      return test::acknowledge({name,
                                [ body ]()
                                {
                                  (process_type<Types>(body), ...);
                                  current_type = text {""};
                                },
                                joined_type_names<Types...>(),
                                tag,
                                timeout_ns,
                                timeout_set});
    }
  };

  template<typename... Types, typename... Contexts> auto within(char const* id, Contexts...)
  {
    return captures_within<as_type_list_t<Types...>, Contexts...> {id};
  }

  template<typename... Types, typename... Contexts> auto within(tagged_id id, Contexts...)
  {
    return captures_within<as_type_list_t<Types...>, Contexts...> {
    id.name, id.tag, id.timeout_ns, id.timeout_set};
  }

  template<typename Types, auto... Generators> struct test_generators;

  template<typename... Type, auto... Generators>
  struct test_generators<types<Type...>, Generators...>
  {
    char const*             name;
    ::tts::expected_outcome tag         = ::tts::expected_outcome::pass;
    nanoseconds             timeout_ns  = 0;
    bool                    timeout_set = false;

    test_generators(char const* id) // NOSONAR
        : name(id)
    {
    }
    test_generators(tagged_id id) // NOSONAR
        : name(id.name)
        , tag(id.tag)
        , timeout_ns(id.timeout_ns)
        , timeout_set(id.timeout_set)
    {
    }
    // The condition depends on Context, as in captures above.
    template<typename... Args> static void process_call(auto body, Args&&... args)
    {
      body(std::forward<Args>(args)...);
    }

    template<typename T> static void process_type(auto body)
    {
      current_type = as_text(typename_<T>);
      if(::tts::is_detailed()) ::tts::output().writeln(">  With <T = %s>", current_type.data());
      process_call(body, ::tts::produce(type<T> {}, Generators)...);
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
                                tg.tag,
                                tg.timeout_ns,
                                tg.timeout_set});
    }
  };

  // A generated case takes no context: its values are produced at run time. The refusal names
  // the rule, where a brace-init of test_generators with a list of contexts would name nothing.
  struct refused_contexts
  {
    bool operator<<(auto) const
    {
      return false;
    }
  };

  template<typename... Contexts> auto refuse_contexts(auto, Contexts...)
  {
    static_assert(sizeof...(Contexts) == 0,
                  "TTS_CASE_WITH takes no context: its values are produced at run time.");
    return refused_contexts {};
  }
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

  The case runs in @ref tts::runtime when its title names no context. A title wrapped by
  @ref TTS_CONSTEXPR or @ref TTS_CONSTEVAL names the contexts the case runs in, and
  @ref TTS_CASE_IN names any list of them.

  @param ID A literal string describing the scenario intents, or a wrapped title.

  @groupheader{Example}
  @snippet doc/case.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CASE(ID)
#else
// A wrapped title expands to a parenthesized list of contexts, then the title, so each case macro
// reads its first argument once expanded and dispatches on its parentheses.
#define TTS_CASE(...) TTS_CASE_SPLIT(__VA_ARGS__)
#define TTS_CASE_SPLIT(FIRST, ...)                                                                 \
  TTS_CAT(TTS_CASE_, TTS_IS_PARENTHESIZED(FIRST))(FIRST __VA_OPT__(, ) __VA_ARGS__)
#define TTS_CASE_0(ID)                                                                             \
  [[maybe_unused]] static auto const TTS_CAT(case_, TTS_FUNCTION) =                                \
  ::tts::_::capture {ID} + +[]() /**/
#define TTS_CASE_1(CONTEXTS, ID)                                                                   \
  [[maybe_unused]] static bool const TTS_CAT(case_, TTS_FUNCTION) =                                \
  ::tts::_::capture_within {ID, TTS_REMOVE_PARENS(CONTEXTS)} + [](TTS_CONTEXT()) /**/
#endif

//======================================================================================================================
/**
  @def TTS_CONTEXT
  @brief Parameter list of a case body run in a named context.

  Wraps the parameter a @ref TTS_CASE_TPL body declares, its type, and declares the context in
  front of it. A @ref TTS_CASE body run in a named context declares no parameter, and its macro
  writes the context itself.

  @param ... The parameters the body declares.

  @groupheader{Example}
  @snippet doc/context.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONTEXT(...)
#else
#define TTS_CONTEXT(...) [[maybe_unused]] auto& local_tts_ctx __VA_OPT__(, ) __VA_ARGS__
#endif

//======================================================================================================================
/**
  @def TTS_CASE_IN
  @brief Introduces a test scenario run in the contexts given, and registers it into the current
  test driver.

  The code block following TTS_CASE_IN contains user-defined code for a given test case, as
  for @ref TTS_CASE, and runs once per context, in the order given: @ref tts::runtime,
  @ref tts::constant, or both. @ref TTS_CONSTEXPR and @ref TTS_CONSTEVAL on the title of a
  @ref TTS_CASE name the two usual lists.

  @param ID  A literal string describing the scenario intents, or a title wrapped by
  @ref TTS_XFAIL, @ref TTS_MAYFAIL, @ref TTS_XINVALID, or @ref TTS_TIMEOUT.
  @param ... The contexts.

  @see TTS_CONSTEXPR
  @see TTS_CONSTEVAL

  @groupheader{Example}
  @snippet doc/case_in.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CASE_IN(ID, ...)
#else
#define TTS_CASE_IN(ID, ...) TTS_CASE_1((__VA_ARGS__), ID)
#endif

//======================================================================================================================
/**
  @def TTS_CASE_TPL_IN
  @brief Introduces a template test case run in the contexts given, and registers it into the
  current test driver.

  The code block following TTS_CASE_TPL_IN contains user-defined code for a given test case, as
  for @ref TTS_CASE_TPL, with the types list in parentheses, and runs once per context for each
  type, in the order given: @ref tts::runtime, @ref tts::constant, or both. The body declares
  its type through @ref TTS_CONTEXT "TTS_CONTEXT": `<typename T>(TTS_CONTEXT(tts::type<T>))`.
  @ref TTS_CONSTEXPR and @ref TTS_CONSTEVAL on the title of a @ref TTS_CASE_TPL name the two
  usual lists.

  @param ID    A literal string describing the scenario intents, or a title wrapped by
  @ref TTS_XFAIL, @ref TTS_MAYFAIL, @ref TTS_XINVALID, or @ref TTS_TIMEOUT.
  @param TYPES A parenthesized list of types to generate the test case from, as
  @ref TTS_CASE_TPL accepts it.
  @param ...   The contexts.

  @see TTS_CASE_IN
  @see TTS_CONTEXT

  @groupheader{Example}
  @snippet doc/case_in.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CASE_TPL_IN(ID, TYPES, ...)
#else
#define TTS_CASE_TPL_IN(ID, TYPES, ...) TTS_CASE_TPL_1((__VA_ARGS__), ID, TTS_REMOVE_PARENS(TYPES))
#endif

//======================================================================================================================
/**
  @def TTS_CONSTEXPR
  @brief Runs a case as ordinary code and in constant evaluation.

  Wraps the title of a @ref TTS_CASE or a @ref TTS_CASE_TPL, as @ref TTS_XFAIL does, and names
  the contexts @ref tts::runtime and @ref tts::constant. The body runs as ordinary code,
  then as a constant expression, and both runs count and report their checks. A `constexpr`
  function is callable in both contexts, which is what the wrapper tests of the code it calls.

  The body is written with the ordinary checks. In constant evaluation, a failure is reported
  with its expression, its position, and the values the check compares at run time. A body that
  calls a function the language refuses in a constant expression does not compile.

  @param ID A literal string describing the scenario intents, or a title already wrapped by
  @ref TTS_XFAIL, @ref TTS_MAYFAIL, @ref TTS_XINVALID, or @ref TTS_TIMEOUT.

  @see TTS_CONSTEVAL
  @see TTS_CONTEXT
  @see tts::constant

  @groupheader{Example}
  @snippet doc/constexpr.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEXPR(ID)
#else
#define TTS_CONSTEXPR(ID) (::tts::runtime, ::tts::constant), ID
#endif

//======================================================================================================================
/**
  @def TTS_CONSTEVAL
  @brief Runs a case in constant evaluation alone.

  Wraps the title of a @ref TTS_CASE or a @ref TTS_CASE_TPL, as @ref TTS_XFAIL does, and names
  the context @ref tts::constant. The body runs as a constant expression, and its checks
  are reported after the evaluation, each failure with its expression and its position. A
  `consteval` function is callable in that context alone, which is what the wrapper tests of the
  code it calls.

  @param ID A literal string describing the scenario intents, or a title already wrapped by
  @ref TTS_XFAIL, @ref TTS_MAYFAIL, @ref TTS_XINVALID, or @ref TTS_TIMEOUT.

  @see TTS_CONSTEXPR
  @see TTS_CONTEXT
  @see tts::constant

  @groupheader{Example}
  @snippet doc/consteval.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CONSTEVAL(ID)
#else
#define TTS_CONSTEVAL(ID) (::tts::constant), ID
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

  A title wrapped by @ref TTS_CONSTEXPR or @ref TTS_CONSTEVAL names the contexts the case runs
  in, for each type in turn, @ref TTS_CASE_TPL_IN names any list of them, and the body then
  declares its type through
  @ref TTS_CONTEXT "TTS_CONTEXT": `<typename T>(TTS_CONTEXT(tts::type<T>))`.

  @param ID   A literal string describing the scenario intents, or a wrapped title.
  @param ...  Lists of types to generate the test case from.

  @groupheader{Example}
  @snippet doc/case_tpl.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_CASE_TPL(ID, ...)
#else
#define TTS_CASE_TPL(...) TTS_CASE_TPL_SPLIT(__VA_ARGS__)
#define TTS_CASE_TPL_SPLIT(FIRST, ...)                                                             \
  TTS_CAT(TTS_CASE_TPL_, TTS_IS_PARENTHESIZED(FIRST))(FIRST, __VA_ARGS__)
#define TTS_CASE_TPL_0(ID, ...)                                                                    \
  [[maybe_unused]] static bool const TTS_CAT(case_, TTS_FUNCTION) =                                \
  ::tts::_::captures<__VA_ARGS__> {ID} + [] /**/
#define TTS_CASE_TPL_1(CONTEXTS, ID, ...)                                                          \
  [[maybe_unused]] static bool const TTS_CAT(case_, TTS_FUNCTION) =                                \
  ::tts::_::within<__VA_ARGS__>(ID, TTS_REMOVE_PARENS(CONTEXTS)) + [] /**/
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

  A generated case runs at run time alone, where its generators produce its values: a title
  wrapped by @ref TTS_CONSTEXPR or @ref TTS_CONSTEVAL is refused.

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
#define TTS_CASE_WITH(...) TTS_CASE_WITH_SPLIT(__VA_ARGS__)
#define TTS_CASE_WITH_SPLIT(FIRST, ...)                                                            \
  TTS_CAT(TTS_CASE_WITH_, TTS_IS_PARENTHESIZED(FIRST))(FIRST, __VA_ARGS__)
#define TTS_CASE_WITH_0(ID, TYPES, ...)                                                            \
  [[maybe_unused]] static bool const TTS_CAT(case_, TTS_FUNCTION) =                                \
  ::tts::_::test_generators<::tts::as_type_list_t<TTS_REMOVE_PARENS(TYPES)>, __VA_ARGS__> {ID}     \
  << [] /**/
#define TTS_CASE_WITH_1(CONTEXTS, ID, TYPES, ...)                                                  \
  [[maybe_unused]] static bool const TTS_CAT(case_, TTS_FUNCTION) =                                \
  ::tts::_::refuse_contexts(ID, TTS_REMOVE_PARENS(CONTEXTS)) << [] /**/
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
// Each tag wrapper expands its argument before reading it and lets a leading list of contexts
// through, so that the wrappers nest in any order with TTS_CONSTEXPR and TTS_CONSTEVAL.
#define TTS_TAG_WRAP(TAG, ...) TTS_TAG_WRAP_SPLIT(TAG, __VA_ARGS__)
#define TTS_TAG_WRAP_SPLIT(TAG, FIRST, ...)                                                        \
  TTS_CAT(TTS_TAG_WRAP_, TTS_IS_PARENTHESIZED(FIRST))(TAG, FIRST __VA_OPT__(, ) __VA_ARGS__)
#define TTS_TAG_WRAP_0(TAG, ID)           ::tts::TAG(ID)
#define TTS_TAG_WRAP_1(TAG, CONTEXTS, ID) CONTEXTS, ::tts::TAG(ID)
#define TTS_XFAIL(...)                    TTS_TAG_WRAP(expect_fail, __VA_ARGS__)
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
#define TTS_MAYFAIL(...) TTS_TAG_WRAP(may_fail, __VA_ARGS__)
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
#define TTS_XINVALID(...) TTS_TAG_WRAP(expect_invalid, __VA_ARGS__)
#endif

//======================================================================================================================
/**
  @def TTS_TIMEOUT
  @brief Gives a @ref TTS_CASE (or @ref TTS_CASE_TPL / @ref TTS_CASE_WITH) a deadline of its own.

  The case is killed once it has been running for MS milliseconds, and the run ends there. `0`
  removes the deadline `--timeout` set for that one case. Wrapping the ID is how the tag travels,
  so it nests with @ref TTS_XFAIL in either order.

  @param MS Deadline in milliseconds.
  @param ID A literal string describing the scenario intents.

  @see TTS_XFAIL

  @groupheader{Example}
  @snippet doc/timeout.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_TIMEOUT(MS, ID)
#else
#define TTS_TIMEOUT(MS, ...) TTS_TIMEOUT_SPLIT(MS, __VA_ARGS__)
#define TTS_TIMEOUT_SPLIT(MS, FIRST, ...)                                                          \
  TTS_CAT(TTS_TIMEOUT_, TTS_IS_PARENTHESIZED(FIRST))(MS, FIRST __VA_OPT__(, ) __VA_ARGS__)
#define TTS_TIMEOUT_0(MS, ID)           ::tts::with_timeout(MS, ID)
#define TTS_TIMEOUT_1(MS, CONTEXTS, ID) CONTEXTS, ::tts::with_timeout(MS, ID)
#endif

//======================================================================================================================
/// @}
//======================================================================================================================

//======================================================================================================================
/// @}
//======================================================================================================================
