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

TTS_CASE_WITH("Check behavior for scalar types",
              (tts::types<int, float, double>),
              tts::value {37},
              tts::between {0, 100},
              tts::randoms {0., 10.})
<typename V, typename B, typename R>(V v, B b, R const& r)
{
  TTS_EQUAL(v, V {37});

  TTS_GREATER_EQUAL(b, B {0});
  TTS_LESS_EQUAL(b, B {100});

  TTS_GREATER_EQUAL(r, R {0});
  TTS_LESS_EQUAL(r, R {10});
};
//! [snippet]
