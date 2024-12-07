//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/engine/info.hpp>
#include <tts/engine/test.hpp>
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
    capture(const char* id) : name(id) {}
    auto operator+(auto body) const { return test::acknowledge( {name, body} ); }
    const char* name;
  };

  // Global storage for current type used in a given test
  inline text current_type = {};

  template<typename... Types> struct captures
  {
    captures(const char* id) : name(id) {}
    auto operator+(auto body) const
    {
      // We register all the types in a single test to keep the compile-time O(1)
      // Registering different type in different tests generate far too much callable::invoker
      // symbol that make compile-time O(N)
      return test::acknowledge( { name
                                , [=]()
                                  {
                                    // We setup the current type name before each test so we know
                                    ( ( (current_type = as_text(typename_<Types>))
                                      , (::tts::_::is_verbose ? printf(">  With <T = %s>\n", current_type.data()) : 0)
                                      , body(type<Types>())
                                      )
                                    , ...
                                    );
                                    // Clear the current type
                                    current_type = text{""};
                                  }
                                }
                              );
    }
    const char* name;
  };

  // Specialisation for types lists
  template<template<typename...> typename List, typename... Types>
  struct captures<List<Types...>> : captures<Types...>
  {};

  // Specialisation for types list generator
  template<typename Generator>
  requires requires(Generator g) { typename Generator::types_list; }
  struct captures<Generator> : captures<typename Generator::types_list>
  {};
}

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
#define TTS_CASE(ID) [[maybe_unused]] static auto const TTS_CAT(case_,TTS_FUNCTION) = ::tts::_::capture{ID} + []()

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
#define TTS_CASE_TPL(ID,...)                                                                                            \
[[maybe_unused]] static bool const TTS_CAT(case_,TTS_FUNCTION) = ::tts::_::captures<__VA_ARGS__>{ID} + []               \
/**/
