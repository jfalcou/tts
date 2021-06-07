//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

//==================================================================================================
// Warning Suppressors
//==================================================================================================
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

//==================================================================================================
// Test engine
//==================================================================================================
#include <tts/engine/main.hpp>
#include <tts/test/case.hpp>
#include <tts/test/info.hpp>

//==================================================================================================
// Test macros
//==================================================================================================
#include <tts/test/basic.hpp>
#include <tts/test/exceptions.hpp>
#include <tts/test/precision.hpp>
#include <tts/test/relation.hpp>
#include <tts/test/sequence.hpp>
#include <tts/test/types.hpp>
