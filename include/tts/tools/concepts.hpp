//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <iomanip>
#include <sstream>
#include <type_traits>

namespace tts::detail
{
  template<typename T>
  concept support_std_to_string = requires(T e) { std::to_string(e); };

  template<typename T>
  concept support_to_string = requires(T e) { to_string(e); };

  template<typename T>
  concept sequence = requires(T e) {std::begin(e); std::end(e); };

  template<typename T>
  concept streamable = requires(T e, std::ostream& o) { o << e; };
}
