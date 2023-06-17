//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#if defined( __ANDROID__ )
#include <type_traits>

namespace tts
{
  template<typename T>
  concept integral = std::is_integral_v<T>;

  template<typename T>
  concept floating_point = std::is_floating_point_v<T>;

  template<typename T, typename U>
  concept same_as_impl = std::is_same_v<T,U>;

  template<typename T, typename U>
  concept same_as = same_as_impl<T,U> && same_as_impl<U,T>;
}
#else
#include <concepts>

namespace tts
{
  using std::integral;
  using std::floating_point;
  using std::same_as;
}
#endif
