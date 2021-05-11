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
    logger(bool status) : display(status), done(false) {}

    template<typename Data> logger& operator<<(Data&& d)
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
