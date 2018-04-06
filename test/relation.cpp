//==================================================================================================
/*
  Copyright 2018 Joel FALCOU

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/relation.hpp>
#include <limits>

//! [bar]
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
//! [bar]

//! [equal]
TTS_CASE( "Check correctness of equality tests" )
{
  foo::bar x{0.3f},y{0.3f};
  std::vector<float> a{1,2,3}, b{1,2,3};

  TTS_EQUAL(a,b); // natural operator==
  TTS_EQUAL(x,y); // ADL-accessible compare_equal
}
//! [equal]

//! [not_equal]
TTS_CASE( "Check correctness of inequality tests" )
{
  foo::bar x{0.3f},y{0.7f};
  std::vector<float> a{1,2,3}, b{7,9};

  TTS_NOT_EQUAL(a,b); // natural operator!=
  TTS_NOT_EQUAL(x,y); // ADL-accessible compare_not_equal
}
//! [not_equal]

//! [less]
TTS_CASE( "Check correctness of less-than tests" )
{
  foo::bar x{0.3f},y{0.7f};
  std::vector<float> a{1,2,3}, b{7,9};

  TTS_LESS(a,b); // natural operator<
  TTS_LESS(x,y); // ADL-accessible compare_less
}
//! [less]

//! [greater]
TTS_CASE( "Check correctness of greater-than tests" )
{
  foo::bar x{0.3f},y{0.7f};
  std::vector<float> a{1,2,3}, b{7,9};

  TTS_GREATER(b,a); // natural operator>
  TTS_GREATER(y,x); // ADL-accessible compare_greater
}
//! [greater]

//! [less_equal]
TTS_CASE( "Check correctness of less-or-equal-than tests" )
{
  foo::bar x{0.3f},y{0.7f};
  std::vector<float> a{1,2,3}, b{7,9};

  TTS_LESS_EQUAL(a,b); // natural operator<=
  TTS_LESS_EQUAL(b,b);
  TTS_LESS_EQUAL(x,y); // ADL-accessible compare_greater
  TTS_LESS_EQUAL(x,x);
}
//! [less_equal]

//! [greater_equal]
TTS_CASE( "Check correctness of greater-or-equal-than tests" )
{
  foo::bar x{0.3f},y{0.7f};
  std::vector<float> a{1,2,3}, b{7,9};

  TTS_GREATER_EQUAL(b,a); // natural operator>=
  TTS_GREATER_EQUAL(a,a);
  TTS_GREATER_EQUAL(y,x); // ADL-accessible compare_greater
  TTS_GREATER_EQUAL(y,y);
}
//! [greater_equal]
