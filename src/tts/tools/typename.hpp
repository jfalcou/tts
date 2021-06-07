//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once
#include <string_view>

namespace tts::detail
{
  template<typename T> struct typename_
  {
    static constexpr auto value() noexcept
    {
  #if defined(_MSC_VER )
      std::string_view data(__FUNCSIG__);
      auto i = data.find('<') + 1,
        j = data.find(">::value");
      return data.substr(i, j - i);
  #else
      std::string_view data(__PRETTY_FUNCTION__);
      auto i = data.find('=') + 2,
        j = data.find_last_of(']');
      return data.substr(i, j - i);
  #endif
    }
  };
}

namespace tts
{
  template<typename T> inline constexpr auto const typename_ = detail::typename_<T>::value();
  template<typename T> constexpr auto typename_of(T&&){ return typename_<T>; }
}
