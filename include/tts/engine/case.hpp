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
    // Declared before the operator: nvc++ reads the body of a hidden friend as an incomplete-class
    // context and finds no member declared below it.
    char const*             name;
    ::tts::expected_outcome tag         = ::tts::expected_outcome::pass;
    nanoseconds             timeout_ns  = 0;
    bool                    timeout_set = false;

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
    friend auto operator+(capture_within const& self, auto body)
    {
      return test::acknowledge(
      {self.name,
       [ body ]() { (Contexts::template run<direct, has_runtime<Contexts...>>(body), ...); },
       /*types=*/ {},
       self.tag,
       self.timeout_ns,
       self.timeout_set});
    }
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
    char const*                                     name;
    ::tts::expected_outcome                         tag         = ::tts::expected_outcome::pass;
    nanoseconds                                     timeout_ns  = 0;
    bool                                            timeout_set = false;

    template<typename T, typename Body> static void process_type(Body body)
    {
      static_assert(std::is_invocable_v<Body&, runtime_context&, type<T>>,
                    "A case run in a context takes TTS_CONTEXT(...) as its parameter list.");
      current_type = as_text(typename_<T>);
      if(::tts::is_detailed()) ::tts::output().writeln(">  With <T = %s>", current_type.data());
      (Contexts::template run<with_type<T>, has_runtime<Contexts...>>(body), ...);
    }

    friend auto operator+(captures_within const& self, auto body)
    {
      return test::acknowledge({self.name,
                                [ body ]()
                                {
                                  (process_type<Types>(body), ...);
                                  current_type = text {""};
                                },
                                joined_type_names<Types...>(),
                                self.tag,
                                self.timeout_ns,
                                self.timeout_set});
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
    friend bool operator<<(refused_contexts, auto)
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
  @brief Defines a test case and registers it with the test driver.

  The block after the macro is the body of the case. A case that runs no check is reported as
  invalid.

  The case runs in @ref tts::runtime when its title names no context. A title wrapped by
  @ref TTS_CONSTEXPR or @ref TTS_CONSTEVAL names the contexts the case runs in, and @ref TTS_CASE_IN
  names any list of them.

  @param ID Title of the case, or a title wrapped by a tag.

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
  @brief Declares the parameter of a @ref TTS_CASE_TPL body run in a named context.

  Wraps the parameter that declares the type, `TTS_CONTEXT(tts::type<T>)`, and adds the context in
  front of it. A @ref TTS_CASE body declares no parameter, and needs no wrapper.

  @param ... The parameter the body declares.

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
  @brief Defines a test case run in each of the given contexts, and registers it with the test
  driver.

  The contexts follow the title, among @ref tts::runtime and @ref tts::constant. The body runs once
  in each, in the order they are given.

  A @ref TTS_CASE whose title is wrapped by @ref TTS_CONSTEXPR runs in both contexts, and one
  wrapped by @ref TTS_CONSTEVAL in @ref tts::constant alone.

  @param ID  Title of the case, or a title wrapped by @ref TTS_XFAIL, @ref TTS_MAYFAIL,
             @ref TTS_XINVALID, or @ref TTS_TIMEOUT.
  @param ... The contexts.

  @groupheader{Example}
  @snippet doc/case_in.cpp snippet

  @see TTS_CONSTEXPR
  @see TTS_CONSTEVAL
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
  @brief Defines a test case run for each type of a list, in each of the given contexts, and
  registers it with the test driver.

  The types follow the title, between parentheses, in any form @ref TTS_CASE_TPL accepts. The
  contexts come last, among @ref tts::runtime and @ref tts::constant. For each type, the body runs
  once in each context, in the order they are given.

  The body declares its type with @ref TTS_CONTEXT "TTS_CONTEXT", as in
  `<typename T>(TTS_CONTEXT(tts::type<T>))`.

  A @ref TTS_CASE_TPL whose title is wrapped by @ref TTS_CONSTEXPR runs in both contexts, and one
  wrapped by @ref TTS_CONSTEVAL in @ref tts::constant alone.

  @param ID    Title of the case, or a title wrapped by @ref TTS_XFAIL, @ref TTS_MAYFAIL,
               @ref TTS_XINVALID, or @ref TTS_TIMEOUT.
  @param TYPES Types to run the case for, between parentheses.
  @param ...   The contexts.

  @groupheader{Example}
  @snippet doc/case_in.cpp snippet

  @see TTS_CASE_IN
  @see TTS_CONTEXT
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
  @brief Runs a test case as ordinary code and in constant evaluation.

  Wraps the title of a @ref TTS_CASE or a @ref TTS_CASE_TPL and names the contexts @ref tts::runtime
  and @ref tts::constant. The body runs as ordinary code, then as a constant expression, and the
  checks of both runs are counted and reported.

  The body uses the ordinary checks. In constant evaluation, a failure is reported with its
  expression, its position, and the values the check compares at run time. A body that calls a
  function the language refuses in a constant expression does not compile.

  @param ID Title of the case, or a title already wrapped by @ref TTS_XFAIL, @ref TTS_MAYFAIL,
            @ref TTS_XINVALID, or @ref TTS_TIMEOUT.

  @groupheader{Example}
  @snippet doc/constexpr.cpp snippet

  @see TTS_CONSTEVAL
  @see TTS_CONTEXT
  @see tts::constant
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
  @brief Runs a test case in constant evaluation alone.

  Wraps the title of a @ref TTS_CASE or a @ref TTS_CASE_TPL and names the context
  @ref tts::constant. The body runs as a constant expression, and its checks are reported after the
  evaluation, each failure with its expression and its position. A body that calls a `consteval`
  function needs this tag, since such a function is callable in constant evaluation alone.

  @param ID Title of the case, or a title already wrapped by @ref TTS_XFAIL, @ref TTS_MAYFAIL,
            @ref TTS_XINVALID, or @ref TTS_TIMEOUT.

  @groupheader{Example}
  @snippet doc/consteval.cpp snippet

  @see TTS_CONSTEXPR
  @see TTS_CONTEXT
  @see tts::constant
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
  @brief Defines a test case run for each type of a list, and registers it with the test driver.

  The block after the macro is a generic lambda, `<typename T>(tts::type<T>) { ... }`, run once per
  type. The types are given as:

  - types separated by commas,
  - a tts::types list,
  - a type whose public member type `types_list` is the list.

  A case that runs no check is reported as invalid.

  A title wrapped by @ref TTS_CONSTEXPR or @ref TTS_CONSTEVAL names the contexts each type runs in,
  and @ref TTS_CASE_TPL_IN names any list of them. The body then declares its type through
  @ref TTS_CONTEXT "TTS_CONTEXT", as in `<typename T>(TTS_CONTEXT(tts::type<T>))`.

  @param ID  Title of the case, or a title wrapped by a tag.
  @param ... Types to run the case for.

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
  @brief Defines a test case run for each type of a list on generated values, and registers it with
  the test driver.

  The block after the macro is a generic lambda with one argument per generator. The lambda runs
  once per type of the list, each generator building its argument for that type. The types are given
  as:

  - types separated by commas, between parentheses,
  - a tts::types list,
  - a type whose public member type `types_list` is the list.

  A case that runs no check is reported as invalid.

  The case runs at run time alone, where its generators produce their values, so a title wrapped by
  @ref TTS_CONSTEXPR or @ref TTS_CONSTEVAL does not compile.

  @param ID    Title of the case, a string literal.
  @param TYPES Types to run the case for.
  @param ...   Generators, one per argument of the body.

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
  @brief Marks a test case as expected to fail.

  Wraps the title of a @ref TTS_CASE, a @ref TTS_CASE_TPL, or a @ref TTS_CASE_WITH, as in
  `TTS_CASE(TTS_XFAIL("..."))`. Nests with @ref TTS_TIMEOUT, @ref TTS_CONSTEXPR, and
  @ref TTS_CONSTEVAL in either order.

  The case passes when at least one of its checks fails, and fails when every check passes.

  A case that runs no check fails too. @ref TTS_XINVALID marks a case expected to run none.

  @param ID Title of the case, a string literal.

  @groupheader{Example}
  @snippet doc/xfail.cpp snippet

  @see TTS_MAYFAIL
  @see TTS_XINVALID
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
  @brief Marks a test case as allowed to fail.

  Wraps the title of a @ref TTS_CASE, a @ref TTS_CASE_TPL, or a @ref TTS_CASE_WITH. Nests with
  @ref TTS_TIMEOUT, @ref TTS_CONSTEXPR, and @ref TTS_CONSTEVAL in either order.

  The case is accepted whether its checks pass or fail, and neither outcome is reported as a
  failure.

  A case that runs no check is still reported as invalid, as for a case without the tag.

  @param ID Title of the case, a string literal.

  @groupheader{Example}
  @snippet doc/mayfail.cpp snippet

  @see TTS_XFAIL
  @see TTS_XINVALID
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
  @brief Marks a test case as expected to run no check.

  Wraps the title of a @ref TTS_CASE, a @ref TTS_CASE_TPL, or a @ref TTS_CASE_WITH. Nests with
  @ref TTS_TIMEOUT, @ref TTS_CONSTEXPR, and @ref TTS_CONSTEVAL in either order.

  The case fails when it runs any check, passing or failing.

  @param ID Title of the case, a string literal.

  @groupheader{Example}
  @snippet doc/xinvalid.cpp snippet

  @see TTS_XFAIL
  @see TTS_MAYFAIL
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
  @brief Gives a test case a deadline of its own.

  Wraps the title of a @ref TTS_CASE, a @ref TTS_CASE_TPL, or a @ref TTS_CASE_WITH. Kills the case
  once it has run for `MS` milliseconds, and ends the run there. `0` removes, for that case, the
  deadline `--timeout` sets.

  Nests with @ref TTS_XFAIL, @ref TTS_MAYFAIL, @ref TTS_XINVALID, @ref TTS_CONSTEXPR, and
  @ref TTS_CONSTEVAL in either order.

  @param MS Deadline in milliseconds.
  @param ID Title of the case, a string literal.

  @groupheader{Example}
  @snippet doc/timeout.cpp snippet

  @see TTS_XFAIL
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
