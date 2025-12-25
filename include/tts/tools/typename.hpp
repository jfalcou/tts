//======================================================================================================================
//! @file
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

    friend text to_text(typename_impl const& t)
    {
      return text("%.*s",t.size(),t.data());
    }

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
      that.size = find(that.data,"]") - i;
      that.data += i;
  #endif
      return that;
    }
  };
}

namespace tts
{
  //====================================================================================================================
  /**
    @defgroup tools-types Type Utilities
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @public
    @brief Evaluates to an object containing the name of the type `T` in readable form.

    `typename_<T>` is a `constexpr` object that can be used to retrieve the name of a type `T` as a text-like object.
    It can be used in conjunction with `as_text` or stream output operators to display type names.

    @groupheader{Example}
    @snippet doc/typename.cpp snippet1

    @tparam T Type to query.
  **/
  //====================================================================================================================
  template<typename T>
  inline auto constexpr typename_ = _::typename_impl<T>{};

  //====================================================================================================================
  /**
    @public
    @brief Returns the name of the type `T` in readable form.

    This function is a convenience wrapper around @ref typename_.

    @groupheader{Example}
    @snippet doc/typename.cpp snippet2

    @param t Value to retrieve the type name for.
    @return A `typename_` instance representing the name of `T` as a string-like object.
  **/
  //====================================================================================================================
  template<typename T>
  constexpr auto name([[maybe_unused]] T const& t)
  {
    return typename_<T>;
  }

  //====================================================================================================================
  /// @}
  //====================================================================================================================
}
