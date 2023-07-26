//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/engine/suite.hpp>
#include <iostream>

namespace tts::detail
{
  struct fatal_signal {};

  struct logger
  {
    logger(bool status = true) : display(status), done(false) {}

    template<typename Data> logger& operator<<(Data const& d)
    {
      if(display)
      {
        if(!done)
        {
          std::cout << ">> Additional information: \n";
          done = true;
        }

        std::cout << d;
      }
      return *this;
    }

    ~logger() noexcept(false)
    {
      if(display && done) std::cout << "\n";
      if(::tts::fatal_error_status) throw ::tts::detail::fatal_signal();
    }

    bool display, done;
  };
}
