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

TTS_CASE( "ULP distance")
{
  float a{1};
  float eps  = std::numeric_limits<float>::epsilon();
  float qnan = std::numeric_limits<float>::quiet_NaN();
  float inf  = std::numeric_limits<float>::infinity();
  float minf = -inf;

  TTS_ULP_EQUAL(a    , a     , 0.  );

  TTS_ULP_EQUAL(a    , qnan  , inf );
  TTS_ULP_EQUAL(qnan , qnan  , 0.  );

  TTS_ULP_EQUAL(a    , inf   , inf );
  TTS_ULP_EQUAL(inf  , inf   , inf );
  TTS_ULP_EQUAL(minf ,minf   , inf );

  TTS_ULP_EQUAL(a, a-eps   , 1   );
  TTS_ULP_EQUAL(a, a+eps   , 0.5 );
  TTS_ULP_EQUAL(a, a+3*eps , 1.5, REQUIRED );
};
//! [snippet]