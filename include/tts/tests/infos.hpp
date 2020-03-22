//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#ifndef TTS_TESTS_INFOS_HPP_INCLUDED
#define TTS_TESTS_INFOS_HPP_INCLUDED

#include <tts/detail/infos.hpp>

#define TTS_PASS(Message)                                                                          \
  ::tts::detail::pass(runtime, {__FILE__, __LINE__}) << Message << std::endl /**/

#define TTS_FAIL(Message)                                                                          \
  ::tts::detail::fail(runtime, {__FILE__, __LINE__}) << Message << std::endl /**/

#define TTS_INVALID(Message)                                                                       \
  ::tts::detail::invalid(runtime, {__FILE__, __LINE__}) << Message << std::endl /**/

#endif
