//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#ifndef TTS_TESTS_INFOS_HPP_INCLUDED
#define TTS_TESTS_INFOS_HPP_INCLUDED

#include <tts/detail/infos.hpp>

//==================================================================================================
/**
  * @brief Register a test which always passes and streams a custom Message string.
  *
  * **Required header:** `#include <tts/tests/infos.hpp>`
  *
  *  **Example:**
  *
  *  @snippet infos/pass.cpp pass
  *
  *  @param Message A literal string to display as additional informations
**/
//==================================================================================================
#define TTS_PASS(Message)                                                                          \
  ::tts::detail::pass(runtime, {__FILE__, __LINE__}) << Message << std::endl /**/

//==================================================================================================
/**
  * @brief Register a test which always fails and streams a custom message string.
  *
  * **Required header:** `#include <tts/tests/infos.hpp>`
  *
  *  **Example:**
  *
  *  @snippet infos/fail.cpp fail
  *
  *  @param Message A literal string to display as additional informations
**/
//==================================================================================================
#define TTS_FAIL(Message)                                                                          \
  ::tts::detail::fail(runtime, {__FILE__, __LINE__}) << Message << std::endl /**/

//==================================================================================================
/**
  * @brief Register a test as invalid and streams a custom message string.
  *
  * **Required header:** `#include <tts/tests/infos.hpp>`
  *
  *  **Example:**
  *
  *  @snippet infos/invalid.cpp invalid
  *
  *  @param Message A literal string to display as additional informations
**/
//==================================================================================================
#define TTS_INVALID(Message)                                                                       \
  ::tts::detail::invalid(runtime, {__FILE__, __LINE__}) << Message << std::endl /**/

#endif
