//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <chrono>
#include <cmath>
#include <initializer_list>
#include <random>
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

    bool is_valid() { return argc && argv != nullptr; }

    int argc;
    char const** argv;
  };

  namespace detail
  {
    inline ::tts::options current_arguments = {0,nullptr};
    inline std::int32_t   current_seed      = -1;
  }

  inline void initialize(int argc, const char** argv)
  {
    if(!detail::current_arguments.is_valid())
      detail::current_arguments = ::tts::options{argc,argv};
  }

  inline ::tts::options const& arguments()
  {
    return detail::current_arguments;
  }

  inline std::int32_t random_seed(int base_seed = -1)
  {
    if(detail::current_seed == -1)
    {
      auto s = ::tts::arguments().value( "--seed", base_seed );
      if(s == -1 )
      {
        auto now = std::chrono::high_resolution_clock::now();
        s        = static_cast<std::int32_t>(now.time_since_epoch().count());
      }

      detail::current_seed = s;
    }

    return detail::current_seed;
  }
}
