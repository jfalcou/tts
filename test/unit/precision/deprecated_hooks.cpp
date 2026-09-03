//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include <tts/tts.hpp>

//==================================================================================================
// The free customization points the traits replace are still honoured, and the documentation says
// so. That promise is what this file holds to.
//
// Everything here is written the deprecated way on purpose. The route is reached from the primary
// of each trait, so a type using it defines no specialization at all, and the whole file stops
// compiling the day a primary drops its fallback. Nothing else in the suite covers the four
// precision hooks since the documentation examples moved to the traits.
//==================================================================================================

namespace legacy
{
  struct ratio
  {
    int              n, d;

    constexpr double value() const
    {
      return static_cast<double>(n) / static_cast<double>(d);
    }
  };

  tts::text to_text(ratio const& r)
  {
    return tts::as_text(r.n) + "/" + tts::as_text(r.d);
  }

  bool compare_equal(ratio const& a, ratio const& b)
  {
    return a.value() == b.value();
  }
  bool compare_less(ratio const& a, ratio const& b)
  {
    return a.value() < b.value();
  }

  double ulp_distance(ratio const& a, ratio const& b)
  {
    return tts::ulp_check(a.value(), b.value());
  }
  double relative_distance(ratio const& a, ratio const& b)
  {
    return tts::relative_check(a.value(), b.value());
  }
  double absolute_distance(ratio const& a, ratio const& b)
  {
    return tts::absolute_check(a.value(), b.value());
  }
  bool ieee_equal(ratio const& a, ratio const& b)
  {
    return tts::ieee_check(a.value(), b.value());
  }
}

TTS_CASE("Check that a to_text overload is still honoured")
{
  TTS_EQUAL(tts::as_text(legacy::ratio {3, 4}), "3/4");
};

TTS_CASE("Check that compare_equal and compare_less overloads are still honoured")
{
  legacy::ratio const half {1, 2}, same {2, 4}, more {3, 4};

  TTS_EQUAL(half, same);
  TTS_NOT_EQUAL(half, more);
  TTS_LESS(half, more);
  TTS_GREATER(more, half);
};

TTS_CASE("Check that the four precision overloads are still honoured")
{
  legacy::ratio const half {1, 2}, same {2, 4}, more {3, 4};

  TTS_ULP_EQUAL(half, same, 0.);
  TTS_ABSOLUTE_EQUAL(half, more, 0.25);
  TTS_RELATIVE_EQUAL(half, more, 0.25);
  TTS_IEEE_EQUAL(half, same);
};
