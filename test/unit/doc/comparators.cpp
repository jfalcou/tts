//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_CUSTOM_DRIVER_FUNCTION fail_main

//! [snippet1]
#define TTS_MAIN                   // No need for main()
#include <tts/tts.hpp>

namespace sample
{
  template<typename T> struct box
  {
    T value;
  };

}

namespace tts
{
  template<typename T> struct comparison<sample::box<T>, sample::box<T>>
  {
    static bool equal(sample::box<T> const& l, sample::box<T> const& r)
    {
      return l.value == r.value;
    }
  };
}

TTS_CASE("Compare values with custom equality")
{
  sample::box<int> a {42};
  sample::box<int> b {13};

  TTS_EQUAL(a, a);
  TTS_NOT_EQUAL(a, b);
};
//! [snippet1]

#undef TTS_MAIN

//! [snippet2]
#define TTS_MAIN // No need for main()
#include <tts/tts.hpp>
#include <cmath>

namespace sample
{
  template<typename T> struct absolute
  {
    T value;
  };

}

namespace tts
{
  template<typename T> struct comparison<sample::absolute<T>, sample::absolute<T>>
  {
    static bool equal(sample::absolute<T> const& l, sample::absolute<T> const& r)
    {
      return std::abs(l.value) == std::abs(r.value);
    }

    static bool less(sample::absolute<T> const& l, sample::absolute<T> const& r)
    {
      return std::abs(l.value) < std::abs(r.value);
    }
  };
}

TTS_CASE("Compare values with custom comparisons")
{
  sample::absolute<int> a {42};
  sample::absolute<int> b {-42};
  sample::absolute<int> c {-13};

  TTS_LESS(c, b);
  TTS_LESS_EQUAL(a, a);
  TTS_GREATER(b, c);
  TTS_GREATER_EQUAL(b, a);
};
//! [snippet2]

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);
  fail_main(argc, argv);
  return ::tts::report(0, 0);
}
