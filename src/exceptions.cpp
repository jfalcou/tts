//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/detail/infos.hpp>
#include <tts/detail/color.hpp>
#include <cstdlib>
#include <string>
#include <typeinfo>
#include <tts/api.hpp>

namespace tts::detail
{
  TTS_API void throw_test ( env& runtime,  location const& l
                          , std::string_view s, std::string_view t
                          , void(*statement)(), std::size_t ref_code
                          )
  {
    bool caught = false;

    try
    {
      statement();
    }
    catch(std::exception& e)
    {
      caught = ref_code == typeid(e).hash_code();
    }

    if(caught)
    {
      pass(runtime,l)  << white << s << detail::reset << " throws " << white << t << std::endl;
    }
    else
    {
      fail(runtime,l)  << white << s << detail::reset << " does not throw " <<  red << t << std::endl;
    }
  }

  TTS_API void nothrow_test ( env& runtime,  location const& l
                            , std::string_view s, void(*statement)()
                            )
  {
    bool caught = false;

    try
    {
      statement();
    }
    catch(...)
    {
      caught = true;
    }

    if(!caught)
    {
      pass(runtime,l)  << white << s << detail::reset << " does not throws.\n";
    }
    else
    {
      fail(runtime,l)  << white << s << detail::reset << " unexpectedly throws.\n";
    }
  }
}
