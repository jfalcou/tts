//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_INFOS_HPP_INCLUDED
#define TTS_TESTS_INFOS_HPP_INCLUDED

#include <tts/detail/rt_helpers.hpp>

// Count a test as passed and display its infos
#define TTS_PASS(MESSAGE)                                                                          \
  runtime.pass() << ::tts::detail::at(__FILE__, __LINE__)                                          \
                 << ": " << ::tts::detail::green_("passed")                                        \
                 << ": " << ::tts::detail::white_(runtime.scenario()) << " - " << MESSAGE          \
                 << std::endl /**/

// Count a test as failed and display its infos
#define TTS_FAIL(MESSAGE)                                                                          \
  runtime.fail() << ::tts::detail::at(__FILE__, __LINE__)                                          \
                 << ": " << ::tts::detail::red_("**FAILED**")                                      \
                 << ": " << ::tts::detail::white_(runtime.scenario()) << " - " << MESSAGE          \
                 << std::endl /**/

// Count a test as invalid and display its infos
#define TTS_INVALID(MESSAGE)                                                                       \
  runtime.invalid() << ::tts::detail::at(__FILE__, __LINE__)                                       \
                 << ": " << ::tts::detail::yellow_("**INVALID**")                                  \
                 << ": " << ::tts::detail::white_(runtime.scenario()) << " - " << MESSAGE          \
                 << std::endl /**/

#endif
