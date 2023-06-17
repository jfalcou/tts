//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <tts/tools/concepts.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/tools/typename.hpp>
#include <tts/tools/types.hpp>
#include <tuple>

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
// Generator-based tests aggregator
//==================================================================================================
namespace tts::detail
{
  template<typename Generator, typename... Types> struct test_generators
  {
    test_generators(const char* id, Generator g, Types...) : name(id), generator(g) {}
    friend auto operator<<(test_generators tg, auto body)
    {
      return test::acknowledge( { tg.name
                                , [tg,body]() mutable
                                  {
                                    using t_t = std::mt19937::result_type;
                                    std::mt19937 gen(static_cast<t_t>(::tts::random_seed()));
                                    ( ( (current_type = " with [T = " + typename_<Types> + "]")
                                      , std::apply(body, tg.generator(type<Types>{}, gen))
                                      ), ...
                                    );
                                    current_type.clear();
                                  }
                                }
                              );
    }
    std::string name;
    Generator   generator;
  };

  template<typename Generator, typename... Types>
  test_generators(const char*,Generator,Types...) -> test_generators<Generator,Types...>;

  // Specialization for types lists
  template<typename Generator, typename... Types>
  struct  test_generators<Generator, types<Types...>>
        : test_generators<Generator,Types...>
  {
    using parent = test_generators<Generator,Types...>;
    test_generators(const char* id, Generator g, types<Types...>) : parent(id,g,Types{}...) {}
  };

  // Specialization for types list generator
  template<typename Generator, typename TypeGenerator>
  requires requires(TypeGenerator g) { typename TypeGenerator::types_list; }
  struct  test_generators<Generator,TypeGenerator>
        : test_generators<Generator,typename TypeGenerator::types_list>
  {
    using parent = test_generators<Generator,typename TypeGenerator::types_list>;
    test_generators ( const char* id, Generator g, TypeGenerator )
                    : parent(id,g,typename TypeGenerator::types_list{}) {}
  };
}

#define TTS_PROTOTYPE(...) [] __VA_ARGS__

//==================================================================================================
// Test case registration macros
//==================================================================================================
#define TTS_CASE(ID)                                                                                \
static bool const TTS_CAT(case_,TTS_FUNCTION) = ::tts::detail::test_capture{ID} + TTS_PROTOTYPE(()) \
/**/

//==================================================================================================
// Test case registration macro from template
//==================================================================================================
#define TTS_CASE_TPL(ID,...)                                                                        \
static bool const TTS_CAT(case_,TTS_FUNCTION) = ::tts::detail::test_captures<__VA_ARGS__>{ID}       \
                                              + TTS_PROTOTYPE()                                     \
/**/

//==================================================================================================
// Test case registration macro from generators
//==================================================================================================
#define TTS_CASE_WITH(ID, TYPES, GENERATOR)                                                         \
static bool const TTS_CAT(case_,TTS_FUNCTION)                                                       \
                  = ::tts::detail::test_generators{ID,GENERATOR,TYPES{}} << TTS_PROTOTYPE()         \
/**/
