//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/engine/environment.hpp>
#include <tts/tools/as_text.hpp>

namespace tts::_
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
          printf("     >> Additional information: \n     ");
          done = true;
        }

        printf("%s",as_text(d).data());
      }
      return *this;
    }

    ~logger() noexcept(false)
    {
      if(display && done)           puts("");
      if(::tts::fatal_error_status) throw ::tts::_::fatal_signal();
    }

    bool display, done;
  };
}
