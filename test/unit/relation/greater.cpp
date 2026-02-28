//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <vector>

TTS_CASE("Check correctness of greater-than comparison tests")
{
  TTS_GREATER(69., 42ULL);

  std::vector<float> a {13, 5}, b {2, 4, 6};
  TTS_GREATER(a, b);
};

constexpr int f()
{
  return 9;
}

TTS_CASE("Check correctness of constexpr greater-than comparison tests")
{
  TTS_CONSTEXPR_GREATER(17, f());
};

#include "foo.hpp"

TTS_CASE("Check correctness of greater-than comparison tests on custom types")
{
  foo::bar x {0.7f}, y {0.1f};

  TTS_GREATER(x, y);
};
