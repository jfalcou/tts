//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#ifndef FOO_HPP_INCLUDED
#define FOO_HPP_INCLUDED

#include <tts/tts.hpp>

namespace foo
{
  struct bar { float v; };

  // Required by TTS
  inline tts::text    as_text( bar const& b ) { return "foo::bar{" + tts::as_text(b.v) + "}"; }

  inline bool compare_equal(foo::bar const& l, foo::bar const& r)
  {
    return l.v == r.v;
  }

  inline bool compare_less(foo::bar const& l, foo::bar const& r)
  {
    return l.v < r.v;
  }
}

#endif
