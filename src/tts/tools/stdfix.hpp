//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#if defined( __ANDROID__ )
#include <type_traits>

namespace std
{
  template<typename T>
  concept integral = std::is_integral_v<T>;

  template<typename T>
  concept floating_point = std::is_floating_point_v<T>;
}
#endif
