//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <tts/tts.hpp>
#include "unit/basic/comparator.hpp"

TTS_CASE("Check that counter-expectation can be met")
{
  int a = 42, b = 69;

  TTS_EXPECT_NOT(a == b);
  TTS_EXPECT_NOT(a != a);
  TTS_EXPECT_NOT(a > b);
  TTS_EXPECT_NOT(a >= b);
  TTS_EXPECT_NOT(b < a);
  TTS_EXPECT_NOT(b <= a);
  TTS_EXPECT_NOT(b && 0);
  TTS_EXPECT_NOT(0 || 0);

  TTS_EXPECT_NOT(a != a, REQUIRED);
  TTS_EXPECT_NOT(a == b, REQUIRED);
  TTS_EXPECT_NOT(a > b, REQUIRED);
  TTS_EXPECT_NOT(a >= b, REQUIRED);
  TTS_EXPECT_NOT(b < a, REQUIRED);
  TTS_EXPECT_NOT(b <= a, REQUIRED);
  TTS_EXPECT_NOT(b && 0, REQUIRED);
  TTS_EXPECT_NOT(0 || 0, REQUIRED);
};

TTS_CASE("Check that constexpr counter-expectation can be met")
{
  TTS_CONSTEXPR_EXPECT_NOT(false);
  TTS_CONSTEXPR_EXPECT_NOT(eq(69, 42));
  TTS_CONSTEXPR_EXPECT_NOT(neq(42, 42));
  TTS_CONSTEXPR_EXPECT_NOT(lt(69, 42));
  TTS_CONSTEXPR_EXPECT_NOT(gt(42, 69));
  TTS_CONSTEXPR_EXPECT_NOT(lte(69, 42));
  TTS_CONSTEXPR_EXPECT_NOT(gte(42, 69));
};
