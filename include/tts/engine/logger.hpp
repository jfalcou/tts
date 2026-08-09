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
  struct fatal_signal
  {
  };

  struct logger
  {
    explicit logger(bool status = true)
        : display(status)
        , done(false)
    {
    }

    template<typename Data> logger& operator<<(Data const& d)
    {
      if(display)
      {
        if(!done)
        {
          ::tts::output().write("     >> Additional information: \n     ");
          done = true;
        }

        ::tts::output().write(as_text(d));
      }
      return *this;
    }

    ~logger() noexcept(false)
    {
      if(display && done) ::tts::output().writeln();
      if(::tts::fatal_error_status) throw ::tts::_::fatal_signal();
    }

    bool display, done;
  };
}
