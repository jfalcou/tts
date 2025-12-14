//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================

//! [snippet]
#define TTS_MAIN  // No need for main()
#include <tts/tts.hpp>

struct some_exception_type {};

void foo() { throw some_exception_type(); }

TTS_CASE( "Check that we can capture thrown exceptions" )
{
  TTS_THROW( foo(), some_exception_type );
};
//! [snippet]