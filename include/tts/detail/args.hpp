//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_COMMON_ARGS_HPP_INCLUDED
#define TTS_COMMON_ARGS_HPP_INCLUDED

#include <cstdlib>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace tts::detail
{
  struct args_map
  {
    args_map();

    void update(int argc, const char **argv) const;

    template<typename R, typename ID> R operator()(ID const &id, R def = R {}) const
    {
      auto opt = data_.find(id);
      if(opt != data_.cend())
      {
        std::istringstream s(std::string(opt->second[ 0 ]));
        s >> def;
      }

      return def;
    }

    template<typename ID> std::string operator()(ID const &id, std::string def = "") const
    {
      auto opt = data_.find(id);
      if(opt != data_.cend()) def = data_[ id ][ 0 ];
      return def;
    }

    template<typename ID> auto const& values(ID const &id) const
    {
      return data_[ id ];
    }

    static bool is_option(std::string_view const &s);

    private:
    mutable std::unordered_map<std::string_view, std::vector<std::string_view>> data_;
  };
}

namespace tts
{
  extern const detail::args_map args;
}

#endif
