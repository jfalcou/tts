//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <array>

TTS_CASE( "Check that types can be tested for equality" )
{
  TTS_TYPE_IS( std::add_pointer<float const>::type, float const* );
};

TTS_CASE( "Check that expression types can be tested for equality" )
{
  [[maybe_unused]] double d;

  TTS_EXPR_IS( &d + 5 , double*   );
  TTS_EXPR_IS( std::move(d)  , double&&  );
  TTS_EXPR_IS( std::swap(d,d), void      );
};

TTS_CASE( "Check that expression types can be tested for compilation status" )
{
  int x{}, y{};
  TTS_EXPECT_COMPILES(x, y,  { x +=y;   } );
  TTS_EXPECT_NOT_COMPILES(x, { x.foo(); } );
};

TTS_CASE_TPL( "Check interaction with templates"
            , int,float,char,void*
            )
<typename Type>(::tts::type<Type>)
{
  TTS_TYPE_IS( std::add_const_t<Type>, Type const);
};

TTS_CASE_TPL( "Check interaction with pre-made type lists", ::tts::arithmetic_types )
<typename Type>(::tts::type<Type>)
{
  TTS_TYPE_IS( std::add_const_t<Type>, Type const);
};

template<int N, typename Indexes = std::make_index_sequence<N>>  struct sizes;

template<int N, std::size_t... I> struct sizes<N, std::index_sequence<I...>>
{
  using types_list = tts::types<std::array<std::byte,I+1>...>;
};

TTS_CASE_TPL( "Check interaction with type list generator", sizes<5> )
<typename Type>( tts::type<Type> )
{
  TTS_TYPE_IS( std::add_const_t<Type>, Type const);
};
