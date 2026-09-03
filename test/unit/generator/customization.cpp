//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include <tts/tts.hpp>

//==================================================================================================
// tts::generation and tts::conversion are the two customization points a generator goes through:
// one builds the value, the other reads the bounds the case wrote once for every type it runs on.
//
// Neither can report a stale free overload, their names carrying a generic default, so the route
// through the trait is the only thing a test can pin. What it catches is a specialization that
// stopped being selected: the built-in path still produces values, and only a bound quietly
// widened or a member left at its default would say so.
//==================================================================================================

namespace app
{
  // Holds a value that must stay inside [0, 100], which is exactly what the built-in path cannot
  // know: it would hand the generator the type and store whatever came back.
  struct percent
  {
    int v;
  };

  inline tts::text as_text(percent const& p)
  {
    return tts::as_text(p.v) + "%";
  }
}

namespace tts
{
  template<> struct comparison<app::percent, app::percent>
  {
    static bool equal(app::percent const& l, app::percent const& r)
    {
      return l.v == r.v;
    }
  };

  template<> struct generation<app::percent>
  {
    static app::percent make(auto g, auto... args)
    {
      auto raw = produce(tts::type<int> {}, g, args...);
      return {raw < 0 ? 0 : (raw > 100 ? 100 : raw)};
    }
  };
}

TTS_CASE("Check that tts::generation drives the value production")
{
  auto const clamped = tts::produce(tts::type<app::percent> {}, tts::value(250));
  auto const kept    = tts::produce(tts::type<app::percent> {}, tts::value(42));

  TTS_EQUAL(clamped, app::percent {100});
  TTS_EQUAL(kept, app::percent {42});

  // Through the dispatcher and through the member, which is the half a single call cannot see.
  TTS_EQUAL(tts::produce(tts::type<app::percent> {}, tts::value(7)),
            tts::generation<app::percent>::make(tts::value(7)));
};

TTS_CASE("Check that the built-in generation is left in place")
{
  TTS_EQUAL(tts::produce(tts::type<int> {}, tts::value(7)), 7);

  // A sequence is still filled element by element, each with its own index and the length.
  auto const ramped = tts::produce(tts::type<std::array<int, 4>> {}, tts::ramp(0, 2));
  TTS_EQUAL(ramped[ 0 ], 0);
  TTS_EQUAL(ramped[ 1 ], 2);
  TTS_EQUAL(ramped[ 3 ], 6);
};

//==================================================================================================
// A bound is written once and has to answer for each type the case runs on, so a value is cast
// while a recipe has to be evaluated against the type instead.
//==================================================================================================
namespace app
{
  // Answers the largest value the type can hold, so the same bound means something different for
  // each of them. A cast would answer the same number converted.
  struct biggest
  {
    template<typename T> constexpr T operator()(tts::type<T>) const
    {
      return std::numeric_limits<T>::max();
    }
  };
}

namespace tts
{
  template<typename T> struct conversion<T, app::biggest>
  {
    static T from(app::biggest const& b)
    {
      return b(tts::type<T> {});
    }
  };
}

TTS_CASE("Check that tts::conversion drives the bound reading")
{
  TTS_EQUAL(tts::convert_as(app::biggest {}, tts::type<std::int8_t> {}), std::int8_t(127));
  TTS_EQUAL(tts::convert_as(app::biggest {}, tts::type<std::int16_t> {}), std::int16_t(32767));

  TTS_EQUAL(tts::convert_as(app::biggest {}, tts::type<std::int8_t> {}),
            (tts::conversion<std::int8_t, app::biggest>::from(app::biggest {})));
};

TTS_CASE("Check that the built-in conversion is left in place")
{
  TTS_EQUAL(tts::convert_as(3, tts::type<double> {}), 3.0);
  TTS_EQUAL(tts::convert_as(3.7, tts::type<int> {}), 3);

  TTS_TYPE_IS(decltype(tts::convert_as(3, tts::type<double> {})), double);
};

//==================================================================================================
// A specialization keyed on the type being built cannot see the generator in an overload set the
// way a produce overload could, so recognizing a range is a question about its type.
//==================================================================================================
TTS_CASE("Check that tts::is_randoms recognizes a range generator")
{
  TTS_EXPECT(tts::is_randoms_v<decltype(tts::randoms(0, 10))>);
  TTS_EXPECT(tts::is_randoms_v<decltype(tts::randoms(0.5, 1.5)) const&>);

  TTS_EXPECT_NOT(tts::is_randoms_v<decltype(tts::value(3))>);
  TTS_EXPECT_NOT(tts::is_randoms_v<decltype(tts::ramp(0, 2))>);
  TTS_EXPECT_NOT(tts::is_randoms_v<int>);
};
