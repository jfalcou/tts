//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <string_view>
#include <ostream>

namespace tts
{
  class source_location
  {
    public:
    [[nodiscard]] static constexpr auto current ( const char* file  = __builtin_FILE()
                                                , int line          = __builtin_LINE()
                                                ) noexcept
    {
      source_location sl{};
      sl.file_ = file;
      sl.line_ = line;
      return sl;
    }

    [[nodiscard]] constexpr auto filename() const noexcept
    {
      std::string_view f(file_);
      return f.substr(f.find_last_of('/')+1);
    }

    [[nodiscard]] constexpr auto line() const noexcept { return line_; }

    friend std::ostream& operator<<(std::ostream& os, source_location const& s)
    {
      return os << "[" << s.filename() << ":" << s.line() << "]";
    }

    private:
    const char* file_{"unknown"};
    int         line_{};
  };
}
