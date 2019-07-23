//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2019 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef FOO_HPP_INCLUDED
#define FOO_HPP_INCLUDED

#include <tts/tests/relation.hpp>

namespace foo
{
  struct bar { float v; };
}

namespace tts { namespace ext
{
  template<typename EnableIf>
  struct equal<foo::bar,foo::bar,EnableIf>
  {
    inline bool operator()(foo::bar const& l, foo::bar const& r) const
    {
      return l.v == r.v;
    }
  };

  template<typename EnableIf>
  struct less<foo::bar,foo::bar,EnableIf>
  {
    inline bool operator()(foo::bar const& l, foo::bar const& r) const
    {
      return l.v < r.v;
    }
  };
} }

#endif
