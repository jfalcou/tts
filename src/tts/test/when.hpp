//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
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
#define TTS_WHEN(STORY)                                                                             \
  std::cout << "[?] - For: " << ::tts::detail::current_test << "\n";                                \
  std::cout << "When      : " << STORY << std::endl;                                                \
  for(int tts_section = 0, tts_count = 1; tts_section < tts_count; tts_count -= 0==tts_section++)   \
    for( tts::detail::only_once tts_only_once_setup{}; tts_only_once_setup; )                       \
/**/

#define TTS_AND_THEN_IMPL(TTS_LOCAL_ID, ...)                                                        \
  static int TTS_LOCAL_ID = 0;                                                                      \
  std::ostringstream TTS_CAT(desc_,TTS_LOCAL_ID);                                                   \
  if(::tts::detail::section_guard(TTS_LOCAL_ID, tts_section, tts_count )                            \
                  .check( ((TTS_CAT(desc_,TTS_LOCAL_ID)  << __VA_ARGS__)                            \
                          , TTS_CAT(desc_,TTS_LOCAL_ID).str())                                      \
                        )                                                                           \
    )                                                                                               \
  for(int tts_section = 0, tts_count = 1; tts_section < tts_count; tts_count -= 0==tts_section++ )  \
    for(tts::detail::only_once tts__only_once_section{}; tts__only_once_section; )                  \
/**/

#define TTS_AND_THEN(...) TTS_AND_THEN_IMPL(TTS_UNIQUE(id), __VA_ARGS__)
