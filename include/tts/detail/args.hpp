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
#include <unordered_map>
#include <vector>

namespace tts
{
  namespace detail
  {
    struct args_map
    {
      args_map() {}

      void update(int argc, const char **argv) const
      {
        bool        found = false;
        std::string id;

        for(int i = 1; i < argc; ++i)
        {
          // look for --name value pattern
          std::string cur {argv[ i ]};

          if(is_option(cur))
          {
            found = true;
            id    = std::string {cur.begin() + 2, cur.end()}; // Remove the '--'
          }
          else
          {
            // if an option was found and we're looking at another impromptu option, update the map
            if(found && !is_option(cur)) { data_[ id ].push_back(cur); }
          }
        }
      }

      template<typename R> R operator()(std::string const &id, R def = R {}) const
      {
        auto opt = data_.find(id);
        if(opt != data_.cend())
        {
          std::istringstream s(opt->second[ 0 ]);
          s >> def;
        }

        return def;
      }

      std::string operator()(std::string const &id, std::string def = "") const
      {
        auto opt = data_.find(id);
        if(opt != data_.cend()) def = data_[ id ][ 0 ];
        return def;
      }

      std::vector<std::string> operator()(std::string const &      id,
                                          std::vector<std::string> def = {}) const
      {
        auto opt = data_.find(id);
        if(opt != data_.cend()) def = data_[ id ];
        return def;
      }

      static bool is_option(std::string const &s)
      {
        return (s.size() > 2) && (s[ 0 ] == '-') && (s[ 1 ] == '-');
      }

    private:
      mutable std::unordered_map<std::string, std::vector<std::string>> data_;
    };
  }
}

namespace tts
{
  inline const detail::args_map args;
}

#endif
