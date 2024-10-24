//======================================================================================================================
//! @file
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <iostream>
#include <tts/tools/preprocessor.hpp>

namespace tts::detail
{
  struct section_guard
  {
    int &       id;
    int const & section;

    section_guard(int &id_, int const &section_, int &count) : id(id_) , section(section_)
    {
      if(section == 0) id = count++ - 1;
    }

    template<typename Desc> bool check(Desc const& desc)
    {
      if(id == section) std::cout << "  And then: " << desc << std::endl;
      return id == section;
    }
  };

  struct only_once
  {
    bool once = true;
    explicit operator bool() { bool result = once; once = false; return result; }
  };
}

//======================================================================================================================
/**
  @def TTS_WHEN
  @brief Start a block of scoped environment.

  Code in a scoped environment can contain:
    + Normal expressions
    + scoped tests introduced by @ref TTS_AND_THEN

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check test with sub-test" )
  {
    TTS_WHEN("We start some sub-test")
    {
      int i = 99;

      TTS_AND_THEN("We increment a variable")
      {
        TTS_EQUAL(i,99);
        i++;
        TTS_EQUAL(i,100);
      }

      TTS_AND_THEN("We decrement a variable")
      {
        // At the start of this sub-test, i is equal to 99 again
        TTS_EQUAL(i,99);
        i--;
        TTS_EQUAL(i,98);
      }
    }
  };
  @endcode
**/
//======================================================================================================================
#define TTS_WHEN(STORY)                                                                             \
TTS_DISABLE_WARNING_PUSH                                                                            \
TTS_DISABLE_WARNING_SHADOW                                                                          \
  std::cout << "[^] - For: " << ::tts::detail::current_test << "\n";                                \
  std::cout << "When      : " << STORY << std::endl;                                                \
  for(int tts_section = 0, tts_count = 1; tts_section < tts_count; tts_count -= 0==tts_section++)   \
    for( tts::detail::only_once tts_only_once_setup{}; tts_only_once_setup; )                       \
TTS_DISABLE_WARNING_POP                                                                             \
/**/

#define TTS_AND_THEN_IMPL(TTS_LOCAL_ID, ...)                                                        \
TTS_DISABLE_WARNING_PUSH                                                                            \
TTS_DISABLE_WARNING_SHADOW                                                                          \
  static int TTS_LOCAL_ID = 0;                                                                      \
  std::ostringstream TTS_CAT(desc_,TTS_LOCAL_ID);                                                   \
  if(::tts::detail::section_guard(TTS_LOCAL_ID, tts_section, tts_count )                            \
                  .check( ((TTS_CAT(desc_,TTS_LOCAL_ID)  << __VA_ARGS__)                            \
                          , TTS_CAT(desc_,TTS_LOCAL_ID).str())                                      \
                        )                                                                           \
    )                                                                                               \
  for(int tts_section = 0, tts_count = 1; tts_section < tts_count; tts_count -= 0==tts_section++ )  \
    for(tts::detail::only_once tts__only_once_section{}; tts__only_once_section; )                  \
TTS_DISABLE_WARNING_POP                                                                             \
/**/

//======================================================================================================================
/**
  @def TTS_AND_THEN
  @brief Add a scoped tests to current scoped environment.

  Compared to regular local scope, whenever a scoped test is run, the data defined in the enclosing
  @ref TTS_WHEN are re-initialized, thus serving as a setup/tear-down system.

  @groupheader{Example}

  @code
  #define TTS_MAIN
  #include <tts/tts.hpp>

  TTS_CASE( "Check test with sub-test" )
  {
    TTS_WHEN("We start some sub-test")
    {
      int i = 99;

      TTS_AND_THEN("We increment a variable")
      {
        TTS_EQUAL(i,99);
        i++;
        TTS_EQUAL(i,100);
      }

      TTS_AND_THEN("We decrement a variable")
      {
        // At the start of this sub-test, i is equal to 99 again
        TTS_EQUAL(i,99);
        i--;
        TTS_EQUAL(i,98);
      }
    }
  };
  @endcode
**/
//======================================================================================================================
#define TTS_AND_THEN(...) TTS_AND_THEN_IMPL(TTS_UNIQUE(id), __VA_ARGS__)
