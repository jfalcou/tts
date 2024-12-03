//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/tools/text.hpp>
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
      int offset = 0;
      auto end = strrchr(file, '/');
      if(end) offset = end - file + 1;

      source_location that{};
      that.desc_ = text{"[%s:%d]",file+offset,line};

      return that;
    }

    text as_text() const { return desc_; }

    decltype(auto) data() const { return desc_.data(); }
    template<_::stream OS>
    friend OS& operator<<(OS& os, source_location const& s)
    {
      return os << s.desc_;
    }

    private:
    text  desc_{"[unknown:?]"};
  };
}
