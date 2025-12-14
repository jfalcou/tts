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

TTS_CASE( "Check that expression types can be tested for equality" )
{
  double d;

  TTS_EXPR_IS( &d + 5        , double*   );
  TTS_EXPR_IS( std::move(d)  , double&&  );
  TTS_EXPR_IS( std::swap(d,d), void      );
};
//! [snippet]