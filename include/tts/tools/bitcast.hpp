//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace tts::_
{
  inline auto as_int(float a)
  {
    return std::bit_cast<std::uint32_t>(a);
  }
  inline auto as_int(double a)
  {
    return std::bit_cast<std::uint64_t>(a);
  }
}
