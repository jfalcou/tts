//======================================================================================================================
//! @file
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/tools/concepts.hpp>
#include <tts/tools/preprocessor.hpp>
#include <tts/tools/typename.hpp>
#include <tts/tools/types.hpp>
#include <tuple>

//======================================================================================================================
// "Seed" capture
//======================================================================================================================
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

//======================================================================================================================
// Generator-based tests aggregator
//======================================================================================================================
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

//======================================================================================================================
/**
  @def TTS_CASE
  @brief Introduces a new test scenario and registers it into the current test driver.

  The code block following TTS_CASE contains user-defined code for a given test case.
  Test cases performing no actual tests will be reported as invalid.

  @param ID A literal string describing the scenario intents.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check basic arithmetic" )
  {
    TTS_EQUAL(2+2, 4);
  };
  @endcode
**/
//======================================================================================================================
#define TTS_CASE(ID)                                                                                                  \
[[maybe_unused]] static bool const TTS_CAT(case_,TTS_FUNCTION) = ::tts::detail::test_capture{ID} + TTS_PROTOTYPE(())  \
/**/

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

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>
  #include <array>

  TTS_CASE_TPL( "Check types using variadic list", char, short, int, double, void* )
  <typename T>( tts::type<T> )
  {
    TTS_GREATER_EQUAL(sizeof(T), 1);
  };

  TTS_CASE_TPL( "Check types using a types list", tts::types<float,double> )
  <typename T>( tts::type<T> )
  {
    TTS_EQUAL(1/T{2}, T{0.5});
  };

  // A Types Generator is any type exposing a types_list internal type
  // In this example we use such a type to generate the list of types:
  //
  // tts::types<std::array<std::byte,1>,...,std::array<std::byte,N>>>;

  template<int N, typename Indexes = std::make_index_sequence<N>>  struct sizes;

  template<int N, std::size_t... I> struct sizes<N, std::index_sequence<I...>>
  {
    using types_list = tts::types<std::array<std::byte,I+1>...>;
  };

  TTS_CASE_TPL( "Check types using a types list generator", sizes<5> )
  <typename T>( tts::type<T> )
  {
    T x;
    TTS_EQUAL(sizeof(x), x.size());
  };
  @endcode
**/
//======================================================================================================================
#define TTS_CASE_TPL(ID,...)                                                                                          \
[[maybe_unused]] static bool const TTS_CAT(case_,TTS_FUNCTION) = ::tts::detail::test_captures<__VA_ARGS__>{ID}        \
                                              + TTS_PROTOTYPE()                                                       \
/**/

//======================================================================================================================
/**
  @def TTS_CASE_WITH
  @brief Introduces a template test case providing dynamically generated data to the test code.

  The following code block will contain tests parametrized by a template type of your choice passed
  as lambda function parameters and generated for each type in the types list.

  Such types list can be provided as:
    + an instance of the tts::types template class
    + an instance of a Type Generator, ie a type exposing a public `types_list` type definition

  Data are passed using the tts::generate function to aggregate data generator.
  Test cases performing no actual tests will be reported as invalid.

  @param ID         A literal string describing the scenario intents.
  @param TYPES      Lists of types to generate the test case from.
  @param GENERATOR  A generator function

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE_WITH ( "Check behavior for scalar types"
                , (tts::types<float, double>)
                , tts::generate(tts::value{37}, tts::between{0,100}, tts::randoms{0.,10.})
                )
  (auto v, auto b, auto const& r)
  {
    TTS_EQUAL(v, 37);

    TTS_GREATER_EQUAL(b,   0);
    TTS_LESS_EQUAL   (b, 100);

    TTS_GREATER_EQUAL(r,  0);
    TTS_LESS_EQUAL   (r, 10);
  };
  @endcode

**/
//======================================================================================================================
#define TTS_CASE_WITH(ID, TYPES, GENERATOR)                                                                           \
[[maybe_unused]] static bool const TTS_CAT(case_,TTS_FUNCTION)                                                        \
                                 = ::tts::detail::test_generators{ID,GENERATOR,TYPES{}} << TTS_PROTOTYPE()            \
/**/
