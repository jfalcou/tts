//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include <vector>

TTS_CASE("Check correctness of less-than comparison tests")
{
  TTS_LESS(42., 69);

  std::vector<float> a {1, 2, 3}, b {12, 3};
  TTS_LESS(a, b);

  TTS_LESS(42., 69, REQUIRED);
};

constexpr int f()
{
  return 45;
}

TTS_CASE("Check correctness of constexpr less-than comparison tests")
{
  TTS_CONSTEXPR_LESS(17, f());
  TTS_CONSTEXPR_LESS(17, f(), REQUIRED);
};

TTS_CASE("Check correctness of typed less-than comparison tests")
{
  TTS_TYPED_LESS(17.f, 45.f);
  TTS_TYPED_LESS(17.f, 45.f, REQUIRED);
};

TTS_CASE("Check correctness of constexpr typed less-than comparison tests")
{
  TTS_TYPED_CONSTEXPR_LESS(17, f());
  TTS_TYPED_CONSTEXPR_LESS(17, f(), REQUIRED);
};

#include "foo.hpp"

TTS_CASE("Check correctness of less-than comparison tests on custom types")
{
  foo::bar x {0.3f}, y {0.7f};

  TTS_LESS(x, y);
};
