//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/concepts.hpp>
#include <tts/tools/text.hpp>
namespace tts::_
{
  template<typename T> struct typename_impl
  {
    private:
    using data_t = struct { const char* data; int size; };
    data_t data_ = {};

    public:
    constexpr typename_impl()   { data_ = typename_impl_value(); }
    constexpr auto data() const { return data_.data; }
    constexpr auto size() const { return data_.size; }

    friend text to_text(typename_impl const& t) { return text("%.*s",t.size(),t.data()); }

    template<_::stream OS>
    friend OS& operator<<(OS& os, typename_impl t)
    {
      for(int i=0;i<t.size();++i) os << t.data()[i];
      return os;
    }

    private:
    //==================================================================================================================
    // constexpr C-style string queries
    //==================================================================================================================
    constexpr static int length(const char* str)
    {
      int i = 0;
      while (str[i] != '\0') { ++i; }
      return i;
    }

    constexpr static auto find(const char* str, const char* n)
    {
      auto compare = [](const char* lhs, const char* rhs, int count)
      {
        if (lhs == nullptr || rhs == nullptr) { return rhs != nullptr ? -1 : 1; }
        for (int i = 0; i < count; ++i)
        {
          if (lhs[i] != rhs[i])     { return lhs[i] < rhs[i] ? -1 : 1; }
          else if (lhs[i] == '\0')  { return 0; }
        }
        return 0;
      };

      auto begin = str;
      if (char c = *n++; c != '\0')
      {
        int len = length(n);
        do
        {
          char sc = {};
          do { if ((sc = *str++) == '\0') return 0; } while (sc != c);
        } while (compare(str, n, len) != 0);
        --str;
      }

      return static_cast<int>(str - begin);
    }

    //==================================================================================================================
    // extract type name from easy-to-parse function
    //==================================================================================================================
    constexpr static auto typename_impl_value()
    {
      data_t that = {};

  #if defined(_MSC_VER )
      that.data   = __FUNCSIG__;
      auto offset = find(that.data,"<") + 1;
      that.size   = find(that.data,">::typename_impl_value(") - offset;
      that.data  += offset;
  #else
      that.data = __PRETTY_FUNCTION__;
      auto i = find(that.data,"=") + 2;
      that.data += i;
      that.size = length(that.data) - 1;
  #endif
      return that;
    }
  };
}

namespace tts
{
  /// Provide a string containing the name of the type `T` in readable form.
  template<typename T> inline auto constexpr typename_ = _::typename_impl<T>{};

  /// Provide a string containing the name of the type of its parameter in readable form.
  template<typename T> constexpr auto name(T const&){ return typename_<T>; }
}
