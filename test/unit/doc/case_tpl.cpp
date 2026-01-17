//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN  // No need for main()
#include <tts/tts.hpp>
#include <array>

TTS_CASE_TPL( "Check types using variadic list", char, short, int, double, void* )
<typename T>( tts::type<T> )
{
  TTS_GREATER_EQUAL(sizeof(T), 1UL);
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
//! [snippet]