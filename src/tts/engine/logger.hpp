//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/tools/color.hpp>
#include <iostream>

namespace tts
{
  struct logger
  {
    logger(bool d = false) : display(d), done(false) {}

    template<typename Result, typename Validator>
    logger& check(Result const& res, Validator validate, auto pass, auto fail)
    {
      display = validate(res) ? pass(res) : fail(res);
      return *this;
    }

    template<typename Result>
    logger& check(Result const& res, auto pass, auto fail)
    {
      return check(res, [](auto const& r) { return static_cast<bool>(r); }, pass, fail);
    }

    template<typename Data>
    logger& operator<<(Data const& d)
    {
      if(display)
      {
        if(!done)
        {
          std::cout << tts::yellow << ">> Additonnal information: " << ::tts::reset << "\n";
          done = true;
        }
        std::cout << d;
      }
      return *this;
    }
    bool display, done;
  };
}
