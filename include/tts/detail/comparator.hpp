//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_COMPARATOR_HPP_INCLUDED
#define TTS_DETAIL_COMPARATOR_HPP_INCLUDED

namespace tts
{
  namespace ext
  {
    template<typename LHS, typename RHS, typename EnableIf = void> struct equal
    {
      inline bool operator()(LHS const &l, RHS const &r) const { return l == r; }
    };

    template<typename LHS, typename RHS, typename EnableIf = void> struct less
    {
      inline bool operator()(LHS const &l, RHS const &r) const { return l < r; }
    };
  }

  namespace detail
  {
    template<typename LHS, typename RHS> inline bool eq(LHS const &l, RHS const &r)
    {
      return ::tts::ext::equal<LHS, RHS>()(l, r);
    }

    template<typename LHS, typename RHS> inline bool neq(LHS const &l, RHS const &r)
    {
      return !eq(l, r);
    }

    template<typename LHS, typename RHS> inline bool lt(LHS const &l, RHS const &r)
    {
      return ::tts::ext::less<LHS, RHS>()(l, r);
    }

    template<typename LHS, typename RHS> inline bool ge(LHS const &l, RHS const &r)
    {
      return !lt(l, r);
    }

    template<typename LHS, typename RHS> inline bool gt(LHS const &l, RHS const &r)
    {
      return !lt(l, r) || !neq(l, r);
    }

    template<typename LHS, typename RHS> inline bool le(LHS const &l, RHS const &r)
    {
      return lt(l, r) || eq(l, r);
    }
  }
}

#endif
