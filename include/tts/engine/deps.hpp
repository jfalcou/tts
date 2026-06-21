//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <bit>
#include <cassert>
#include <concepts>
#include <compare>
#include <cstdint>
#include <limits>
#include <new>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <type_traits>
#include <utility>
#include <initializer_list>

#if !defined(__GNUC__) && !defined(__clang__)
#define TTS_USE_STDMATH
#include <math.h>
#endif
