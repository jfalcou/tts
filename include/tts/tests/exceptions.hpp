//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_EXCEPTIONS_HPP_INCLUDED
#define TTS_TESTS_EXCEPTIONS_HPP_INCLUDED

#include <tts/detail/exceptions.hpp>
#include <tts/detail/infos.hpp>
#include <tts/detail/pp_helpers.hpp>
#include <typeinfo>

#define TTS_THROW(X, T)                                                                            \
  ::tts::detail::throw_test(                                                                       \
      runtime,                                                                                     \
      {__FILE__, __LINE__},                                                                        \
      TTS_STRING(X),                                                                               \
      TTS_STRING(T),                                                                               \
      [] { X; },                                                                                   \
      typeid(T).hash_code()) /**/

#define TTS_NO_THROW(X)                                                                            \
  ::tts::detail::nothrow_test(runtime, {__FILE__, __LINE__}, TTS_STRING(X), [] { X; }) /**/
#endif
