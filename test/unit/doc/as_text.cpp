//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

//! [snippet]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>

namespace space
{
  struct some_type
  {
    int i;
  };

  struct some_other_type
  {
    int j;
  };
}

namespace tts
{
  template<> struct display<space::some_type>
  {
    static text render(space::some_type const& s)
    {
      return "some_type[" + as_text(s.i) + "]";
    }
  };

  template<> struct display<space::some_other_type>
  {
    static text render(space::some_other_type const& s)
    {
      return "[[" + as_text(s.j) + "]]";
    }
  };
}

TTS_CASE("Check display of type with a tts::display specialization")
{
  TTS_EQUAL(tts::as_text(space::some_type {42}), "some_type[42]");
  TTS_EQUAL(tts::as_text(space::some_other_type {63}), "[[63]]");
};
//! [snippet]
