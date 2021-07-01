//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/engine/suite.hpp>
#include <tts/tools/color.hpp>
#include <tts/tools/source_location.hpp>

//==================================================================================================
// Tests macros - Basic information
//==================================================================================================
#define TTS_PASS(Message)                                                                           \
  [&]()                                                                                             \
  {                                                                                                 \
    ::tts::global_runtime.pass();                                                                   \
    if(::tts::verbose_status)                                                                       \
      std::cout << ::tts::source_location::current() << " - " << ::tts::bold                        \
                << ::tts::green << "!!SUCCESS!!" << ::tts::reset                                    \
                << " - " << Message << std::endl;                                                   \
  }()
/**/

#define TTS_FAIL(Message)                                                                           \
  [&]()                                                                                             \
  {                                                                                                 \
    ::tts::global_runtime.fail();                                                                   \
    std::cout << ::tts::source_location::current() << " - " << ::tts::bold                          \
              << ::tts::red << "**FAILURE**" << ::tts::reset                                        \
              << " - " << Message << std::endl;                                                     \
  }()
/**/

#define TTS_FATAL(Message)                                                                          \
  [&]()                                                                                             \
  {                                                                                                 \
    ::tts::global_runtime.fatal();                                                                  \
    std::cout << ::tts::source_location::current() << " - " << ::tts::bold                          \
              << ::tts::red << "** FATAL **" << ::tts::reset                                        \
              << " - " << Message << std::endl;                                                     \
    throw ::tts::detail::fatal_signal();                                                            \
  }()
/**/

#define TTS_INVALID(Message)                                                                        \
  [&]()                                                                                             \
  {                                                                                                 \
    ::tts::global_runtime.invalid();                                                                \
    std::cout << ::tts::source_location::current() << " - " << ::tts::bold                          \
              << ::tts::magenta << "@@INVALID@@" << ::tts::reset                                    \
              << " - " << Message << std::endl;                                                     \
  }()
/**/
