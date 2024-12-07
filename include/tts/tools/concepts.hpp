//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <concepts>

namespace tts::_
{
  template<typename T> concept stream = requires(T& os)
  {
    { os.copyfmt(os)  };
    { os.fill(0)      };
  };

  template <typename T>
  concept string = requires(T const& s)
  {
    typename T::size_type;
    typename T::value_type;
    { s[0]                                      } -> std::convertible_to<typename T::value_type>;
    { s.data()                                  } -> std::convertible_to<typename T::value_type const*>;
    { s.size()                                  } -> std::same_as<typename T::size_type>;
    { s.find_first_of(typename T::value_type{}) } -> std::same_as<typename T::size_type>;
  };

  template <typename T>
  concept optional = requires(T const& o)
  {
    typename T::value_type;
    { o.has_value() } -> std::convertible_to<bool>;
    { o.value()     } -> std::convertible_to<typename T::value_type>;
    { o.value_or(0) };
  };

  template <typename T>
  concept sequence = requires(T const& s)
  {
    { s.begin() };
    { s.end()   };
  };
}
