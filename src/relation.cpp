//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/api.hpp>
#include <tts/engine/env.hpp>
#include <tts/tests/infos.hpp>
#include <tts/detail/color.hpp>
#include <tts/detail/relations.hpp>
#include <string>

namespace tts::detail
{
  TTS_API void check_relation ( ::tts::env& r, location const& l, bool result
                              , std::string_view a , std::string_view b, std::string_view op
                              , std::string_view va, std::string_view vb
                              )
  {
    if(result)
    {
      pass(r,l) << "Expecting: " << white << a << " " << op << " " << b << reset << "\n";
    }
    else
    {
      fail(r,l) << "Expecting: "  << white << a  << " " << op << " " <<  b << reset
                << " but "        << red   << va << " " << op << " " << vb << reset
                << " occured instead.\n";
    }
  }
}
