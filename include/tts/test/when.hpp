//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/preprocessor.hpp>
#include <tts/tools/options.hpp>
#include <tts/tools/text.hpp>

namespace tts::_
{
  struct section_guard
  {
    int&       id;
    int const& section;

    section_guard(int& id_, int const& section_, int& count)
        : id(id_)
        , section(section_)
    {
      if(section == 0) id = count++ - 1;
    }

    bool check(char const* desc)
    {
      if(id == section && desc && ::tts::is_verbose())
        ::tts::output().writeln("  And then: %s", desc);
      return id == section;
    }
  };

  struct only_once
  {
    bool     once = true;
    explicit operator bool()
    {
      bool result = once;
      once        = false;
      return result;
    }
  };
}

//======================================================================================================================
/**
  @name Scoped scenarios
  @ingroup test-scenario
  @{
**/
//======================================================================================================================

//======================================================================================================================
/**
  @def TTS_WHEN
  @ingroup test-scenario
  @brief Starts a block whose code runs again before each of its sections.

  A section is a @ref TTS_AND_THEN block inside it.

  @param STORY Text printed under `--verbose` when the block starts.

  @groupheader{Example}
  @snippet doc/when.cpp snippet

  @see TTS_AND_THEN
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_WHEN(STORY)
#else
#define TTS_WHEN(STORY)                                                                            \
  TTS_DISABLE_WARNING_PUSH                                                                         \
  TTS_DISABLE_WARNING_SHADOW(                                                                      \
  ::tts::is_verbose() ? (::tts::output().writeln("When      : %s", ::tts::text {STORY}.data()), 0) \
                      : 0);                                                                        \
  for(int tts_section = 0, tts_count = 1; tts_section < tts_count;                                 \
      tts_count -= 0 == tts_section++)                                                             \
    for(tts::_::only_once tts_only_once_setup {}; tts_only_once_setup;)                            \
      TTS_DISABLE_WARNING_POP                                                                      \
/**/
#endif

#define TTS_AND_THEN_IMPL(TTS_LOCAL_ID, MESSAGE)                                                   \
  TTS_DISABLE_WARNING_PUSH                                                                         \
  TTS_DISABLE_WARNING_SHADOW                                                                       \
  static int TTS_LOCAL_ID = 0;                                                                     \
  if(::tts::_::section_guard(TTS_LOCAL_ID, tts_section, tts_count).check(MESSAGE))                 \
    for(int tts_section = 0, tts_count = 1; tts_section < tts_count;                               \
        tts_count -= 0 == tts_section++)                                                           \
      for(tts::_::only_once tts__only_once_section {}; tts__only_once_section;)                    \
        TTS_DISABLE_WARNING_POP                                                                    \
/**/

//======================================================================================================================
/**
  @def TTS_AND_THEN
  @ingroup test-scenario
  @brief Adds a section to a @ref TTS_WHEN block.

  A change that a section makes to a variable of the block does not reach the next section.

  @param MESSAGE Text printed under `--verbose` when the section starts.

  @groupheader{Example}
  @snippet doc/when.cpp snippet

  @see TTS_WHEN
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_AND_THEN(MESSAGE)
#else
#define TTS_AND_THEN(MESSAGE) TTS_AND_THEN_IMPL(TTS_UNIQUE(id), MESSAGE)
#endif

//======================================================================================================================
/// @}
//======================================================================================================================
