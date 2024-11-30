//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once
#include <stdio.h>
#include <string.h>

namespace tts::_
{
  class source_location
  {
    public:
    [[nodiscard]] static auto current ( const char* file  = __builtin_FILE()
                                      , int line          = __builtin_LINE()
                                      ) noexcept
    {
      source_location sl{};
      auto offset = strrchr(file, '/') - file + 1;
      sl.file_ = file + offset;
      sl.line_ = line;
      return sl;
    }

    void print(const char* end) const { printf("[%s:%d]%s", file_, line_, end); }
    void print()                const { print(""); }

    private:
    const char* file_{"unknown"};
    int         line_{};
  };
}
