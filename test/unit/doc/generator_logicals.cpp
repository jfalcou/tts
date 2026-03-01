//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN
#include <tts/tts.hpp>
#include <array>

TTS_CASE_WITH("Test tts::logicals generator",
              (std::array<float, 9>, std::array<int, 9>),
              tts::logicals {0, 3})
<typename T>(T values)
{
  // idx 0,3,6    -> true
  // idx 1,2,4,5 -> false
  TTS_EQUAL(values[ 0 ], true);
  TTS_EQUAL(values[ 1 ], false);
  TTS_EQUAL(values[ 2 ], false);
  TTS_EQUAL(values[ 3 ], true);
  TTS_EQUAL(values[ 4 ], false);
  TTS_EQUAL(values[ 5 ], false);
  TTS_EQUAL(values[ 6 ], true);
  TTS_EQUAL(values[ 7 ], false);
  TTS_EQUAL(values[ 8 ], false);
};
//! [snippet]
