//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include <tts/tts.hpp>

//==================================================================================================
// tts::comparison is what TTS_EQUAL and the ordering macros reach for a user-defined type. The
// free functions it replaces are still honoured by the primary, and foo.hpp keeps exercising that
// path, so what is left to pin here is the trait route and the two of them agreeing.
//==================================================================================================

namespace app
{
  // Two values of this type are the same when their tags match, whatever the payload holds. An
  // operator== would have to answer that too, so the type deliberately has none: the trait is the
  // only thing that can say what equality means here.
  struct tagged
  {
    int    tag;
    double payload;
  };

  inline tts::text as_text(tagged const& t)
  {
    return "tagged{" + tts::as_text(t.tag) + ", " + tts::as_text(t.payload) + "}";
  }
}

namespace tts
{
  template<> struct comparison<app::tagged, app::tagged>
  {
    static bool equal(app::tagged const& l, app::tagged const& r)
    {
      return l.tag == r.tag;
    }
    static bool less(app::tagged const& l, app::tagged const& r)
    {
      return l.tag < r.tag;
    }
  };
}

TTS_CASE("Check that tts::comparison drives the relation macros")
{
  app::tagged const a {1, 0.0}, b {1, 99.5}, c {2, 0.0};

  TTS_EQUAL(a, b);
  TTS_NOT_EQUAL(a, c);

  TTS_LESS(a, c);
  TTS_LESS_EQUAL(a, b);
  TTS_GREATER(c, a);
  TTS_GREATER_EQUAL(b, a);
};

TTS_CASE("Check that tts::comparison is reachable on its own")
{
  app::tagged const a {1, 0.0}, c {2, 0.0};

  TTS_EXPECT(tts::comparison<app::tagged>::equal(a, a));
  TTS_EXPECT_NOT(tts::comparison<app::tagged>::equal(a, c));
  TTS_EXPECT(tts::comparison<app::tagged>::less(a, c));
};

//==================================================================================================
// The second parameter defaults to the first, so a homogeneous comparison names one type. A
// heterogeneous one is specialized on the pair, and only on that pair: the reversed order is a
// different specialization and keeps the built-in path.
//==================================================================================================
namespace app
{
  struct feet
  {
    double v;
  };
  struct metres
  {
    double v;
  };

  inline tts::text as_text(feet const& f)
  {
    return tts::as_text(f.v) + "ft";
  }
  inline tts::text as_text(metres const& m)
  {
    return tts::as_text(m.v) + "m";
  }
}

namespace tts
{
  template<> struct comparison<app::feet, app::metres>
  {
    static bool equal(app::feet const& l, app::metres const& r)
    {
      return l.v * 0.3048 == r.v;
    }
  };
}

TTS_CASE("Check that tts::comparison keys on the ordered pair of operands")
{
  app::feet const   f {10.0};
  app::metres const m {3.048};

  TTS_EQUAL(f, m);

  TTS_EXPECT((tts::comparison<app::feet, app::metres>::equal(f, m)));
  TTS_EXPECT_NOT((tts::comparison<app::feet, app::metres>::equal(app::feet {1.0}, m)));
};

//==================================================================================================
// Anything without a specialization keeps the built-in comparison, which is what leaves the trait
// an addition rather than a detour every type has to pay for.
//==================================================================================================
TTS_CASE("Check that the built-in comparison is left in place")
{
  TTS_EQUAL(45, 45);
  TTS_EQUAL(45, 45.0);
  TTS_NOT_EQUAL(45, 46);
  TTS_LESS(45, 46);

  TTS_EXPECT((tts::comparison<int, double>::equal(45, 45.0)));
  TTS_EXPECT((tts::comparison<int>::less(45, 46)));
};
