//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <array>

TTS_CASE( "Check that types can be tested for equality" )
{
  TTS_TYPE_IS( std::add_pointer<float const>::type, float const* );
}

TTS_CASE( "Check that expression types can be tested for equality" )
{
  [[maybe_unused]] double d;

  TTS_EXPR_IS( &d + 5 , double*   );
  TTS_EXPR_IS( std::move(d)  , double&&  );
  TTS_EXPR_IS( std::swap(d,d), void      );
}

TTS_CASE_TPL("Check interaction with templates", TTS_ALL_TYPES)
{
  TTS_TYPE_IS( std::add_const_t<T>, T const);
}
