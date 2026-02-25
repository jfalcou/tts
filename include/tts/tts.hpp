//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

namespace tts
{
}

#if defined(_MSC_VER)
#if _MSVC_LANG < 202002L
#error "TTS C++ version error"
#include "TTS requires C++20 or higher. Use /std:c++20 or higher to enable C++20 features."
#endif
#else
#if __cplusplus < 202002L
#error "TTS C++ version error"
#include "TTS requires C++20 or higher. Use -std=c++20 or higher to enable C++20 features."
#endif
#endif

#include <tts/engine/deps.hpp>
#include <tts/engine/main.hpp>
#include <tts/engine/case.hpp>

#include <tts/test/basic.hpp>
#include <tts/test/exceptions.hpp>
#include <tts/test/relation.hpp>
#include <tts/test/types.hpp>
#include <tts/test/precision.hpp>
#include <tts/test/sequence.hpp>
#include <tts/test/when.hpp>
#include <tts/test/ranges.hpp>
