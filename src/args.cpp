//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <tts/detail/args.hpp>

namespace tts::detail
{
  args_map::args_map() {}

  void args_map::update(int argc, const char **argv) const
  {
    std::vector<std::string_view> args{argv+1,argv+argc};
    auto found = std::find_if(args.begin(), args.end(), [](auto const& a) { return is_option(a); } );

    while(found != args.end())
    {
      // Get an ID
      auto id   = found->substr(2);

      // Fetch the next '--option'
      auto prev = ++found;
      found = std::find_if(found, args.end(), [](auto const& a) { return is_option(a); } );

      // If found, insert everything else in the option
      if(prev != found)
        data_[id].insert(data_[id].begin(),prev,found);
      else
        data_[id].push_back("1");
    }
  }

  bool args_map::is_option(std::string_view const &s)
  {
    return (s.size() > 2) && (s[ 0 ] == '-') && (s[ 1 ] == '-');
  }
}

namespace tts
{
  const detail::args_map args;
}
