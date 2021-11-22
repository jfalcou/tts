//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <iostream>

namespace tts
{
  struct logger
  {
    logger(bool status = true) : display(status), done(false) {}

    template<typename Data> logger& operator<<(Data const& d)
    {
      if(display)
      {
        if(!done)
        {
          std::cout << ">> Additionnal information: \n";
          done = true;
        }

        std::cout << d;
      }
      return *this;
    }

    ~logger() { if(display && done) std::cout << "\n"; }

    bool display, done;
  };
}
