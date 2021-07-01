//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <initializer_list>
#include <sstream>
#include <string>

namespace tts
{
  struct option
  {
    option() = default;
    option( std::string arg ) : token(std::move(arg)), position(token.rfind( '=' )) {}

    auto flag()     const { return token.substr(0, position); }
    bool is_valid() const { return !flag().empty(); }

    template<typename T> T get(T const& def = T{}) const
    {
      T that;

      if(is_valid())
      {
        std::istringstream os(token.substr(position+1));
        if(os >> that) return that;
        else           return def;
      }
      else
      {
        return def;
      }
    }

    std::string token     = "";
    size_t      position  = std::string::npos;
  };

  struct options
  {
    using params_t = std::initializer_list<const char*>;

    option find(const char* f ) const { return find({f}); }
    option find(params_t    fs) const
    {
      for(int i=1;i<argc;++i)
      {
        option o(argv[i]);
        for(auto f : fs)
          if( o.flag() == f ) return o;
      }

      return option{};
    }

    bool operator[](params_t    fs) const { return find(fs).is_valid(); }
    bool operator[](const char* f ) const { return operator[]({f}); }

    template<typename T> T value(params_t fs, T that = {}) const
    {
      if( auto o = find(fs); o.is_valid()) that = o.template get<T>(that);
      return that;
    }

    template<typename T> T value(const char* f, T that = {}) const
    {
      return value({f},that);
    }

    int argc;
    char const** argv;
  };

  inline ::tts::options arguments;
  inline bool           verbose_status;
}
