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
  // Type identity
  template<typename T>    using identity_t = T;

  // Faster declval
  template<typename, typename = void> extern identity_t<void (*)() noexcept> declval;
  template<typename T>                extern identity_t<T && (*)() noexcept> declval<T, std::void_t<T&&>>;
}
