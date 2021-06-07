//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/test/basic.hpp>

#define TTS_EQUAL(LHS, RHS, ...)         TTS_EXPECT(LHS == RHS, __VA_ARGS__)
#define TTS_NOT_EQUAL(LHS, RHS, ...)     TTS_EXPECT(LHS != RHS, __VA_ARGS__)
#define TTS_LESS(LHS, RHS, ...)          TTS_EXPECT(LHS  < RHS, __VA_ARGS__)
#define TTS_GREATER(LHS, RHS, ...)       TTS_EXPECT(LHS  > RHS, __VA_ARGS__)
#define TTS_LESS_EQUAL(LHS, RHS, ...)    TTS_EXPECT(LHS <= RHS, __VA_ARGS__)
#define TTS_GREATER_EQUAL(LHS, RHS, ...) TTS_EXPECT(LHS >= RHS, __VA_ARGS__)

#define TTS_CONSTEXPR_EQUAL(LHS, RHS, ...)         TTS_CONSTEXPR_EXPECT(LHS == RHS, __VA_ARGS__)
#define TTS_CONSTEXPR_NOT_EQUAL(LHS, RHS, ...)     TTS_CONSTEXPR_EXPECT(LHS != RHS, __VA_ARGS__)
#define TTS_CONSTEXPR_LESS(LHS, RHS, ...)          TTS_CONSTEXPR_EXPECT(LHS  < RHS, __VA_ARGS__)
#define TTS_CONSTEXPR_GREATER(LHS, RHS, ...)       TTS_CONSTEXPR_EXPECT(LHS  > RHS, __VA_ARGS__)
#define TTS_CONSTEXPR_LESS_EQUAL(LHS, RHS, ...)    TTS_CONSTEXPR_EXPECT(LHS <= RHS, __VA_ARGS__)
#define TTS_CONSTEXPR_GREATER_EQUAL(LHS, RHS, ...) TTS_CONSTEXPR_EXPECT(LHS >= RHS, __VA_ARGS__)
