//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>

TTS_CASE_TPL("Check tts::random_value", float, double, int)
<typename T>(tts::type<T>)
{
  for(auto i = 0; i < 100000; ++i)
  {
    T val = tts::random_value(T(-10), T(+10));
    TTS_EXPECT(val >= T(-10)) << val;
    TTS_EXPECT(val <= T(+10)) << val;
  }
};
//! [snippet]
