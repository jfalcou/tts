//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/tools/preprocessor.hpp>
#include <tts/tools/typename.hpp>
#include <tts/tools/types.hpp>

//================================================================================================
// "Seed" capture
//================================================================================================
namespace tts::detail
{
  struct test_capture
  {
    test_capture(const char* id) : name(id) {}
    auto operator+(auto body) const { return test::acknowledge( {name, body} ); }
    const char* name;
  };

  // Global storage for current type used in a given test
  inline std::string current_type = {};

  template<typename... Types> struct test_captures
  {
    test_captures(const char* id) : name(id) {}
    auto operator+(auto body) const
    {
      // We register all the types in a single test to keep the compile-time O(1)
      // Registering different type in different tests generate far too much callable::invoker
      // symbol that make compile-time O(N)
      return test::acknowledge( { name
                                , [=]()
                                  {
                                    // We setup the current type name before each test so we know
                                    ( ( (current_type = " with [T = " + typename_<Types> + "]")
                                      , body(type<Types>())
                                      )
                                    , ...
                                    );
                                    // Clear the current type
                                    current_type.clear();
                                  }
                                }
                              );
    }
    std::string name;
  };

  // Specialisation for types lists
  template<typename... Types>
  struct test_captures<types<Types...>> : test_captures<Types...>
  {};

  // Specialisation for types list generator
  template<typename Generator>
  requires requires(Generator g) { typename Generator::types_list; }
  struct test_captures<Generator> : test_captures<typename Generator::types_list>
  {};
}

//==================================================================================================
// Test case registration macros
//==================================================================================================
#define TTS_CASE(ID)                                                                                \
static bool const TTS_CAT(case_,TTS_FUNCTION) = ::tts::detail::test_capture{ID} + []()              \
/**/

//==================================================================================================
// Test case registration macros
//==================================================================================================
#define TTS_CASE_TPL(ID,...)                                                                        \
static bool const TTS_CAT(case_,TTS_FUNCTION) = ::tts::detail::test_captures<__VA_ARGS__>{ID} + []  \
/**/
