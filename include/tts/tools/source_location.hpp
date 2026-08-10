//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/text.hpp>

namespace tts::_
{
  class source_location
  {
  public:
    [[nodiscard]] static auto current(char const* file = __builtin_FILE(),
                                      int         line = __builtin_LINE()) noexcept
    {
      // __builtin_FILE() uses whatever separator the compiler was invoked with - '/' almost
      // everywhere, but '\' on Windows - so both need checking to reliably keep just the
      // basename; picking only '/' left the full absolute path in place on MSVC/clang-cl.
      int  offset = 0;
      auto slash  = strrchr(file, '/');
      auto bslash = strrchr(file, '\\');
      auto end    = (bslash && (!slash || bslash > slash)) ? bslash : slash;
      if(end) offset = static_cast<int>(end - file + 1);

      source_location that {};
      that.desc_ = text {"[%s:%d]", file + offset, line};

      return that;
    }

    friend text to_text(source_location const& s)
    {
      return s.desc_;
    }

    decltype(auto) data() const
    {
      return desc_.data();
    }

    template<_::stream OS> friend OS& operator<<(OS& os, source_location const& s)
    {
      return os << s.desc_;
    }

  private:
    text desc_ {"[unknown:?]"};
  };
}
