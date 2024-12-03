//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

namespace tts::_
{
  template<typename T> concept stream = requires(T& os)
  {
    { os.copyfmt(os)  };
    { os.fill(0)      };
  };
}
