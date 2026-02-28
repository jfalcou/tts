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

// A user-defined type for which we will provide custom limits
struct data
{
  int   i;
  float f;
};

// Specialize the limits_set for our user-defined type
template<> struct tts::limits_set<data>
{
  data valmax {std::numeric_limits<int>::max(), std::numeric_limits<float>::max()};
  data valmin {std::numeric_limits<int>::lowest(), std::numeric_limits<float>::lowest()};
  data zero {0, 0.0f};
  data one {1, 1.0f};
};

TTS_CASE_TPL("Limit values for integer types", tts::integral_types)
<typename T>(tts::type<T> tgt)
{
  TTS_EQUAL(tts::limits(tgt).valmax, std::numeric_limits<T>::max());
  TTS_EQUAL(tts::limits(tgt).valmin, std::numeric_limits<T>::lowest());
  TTS_EQUAL(tts::limits(tgt).zero, static_cast<T>(0));
  TTS_EQUAL(tts::limits(tgt).one, static_cast<T>(1));
};

TTS_CASE_TPL("Limit values for floating-point types", tts::real_types)
<typename T>(tts::type<T> tgt)
{
  TTS_IEEE_EQUAL(tts::limits(tgt).nan, std::numeric_limits<T>::quiet_NaN());
  TTS_EQUAL(tts::limits(tgt).inf, std::numeric_limits<T>::infinity());
  TTS_EQUAL(tts::limits(tgt).minf, -std::numeric_limits<T>::infinity());
  TTS_EQUAL(tts::limits(tgt).mzero, static_cast<T>(-0.0));
  TTS_EQUAL(tts::limits(tgt).mindenormal, std::numeric_limits<T>::denorm_min());
  TTS_EQUAL(tts::limits(tgt).smallestposval, std::numeric_limits<T>::min());
  TTS_EQUAL(tts::limits(tgt).mone, static_cast<T>(-1.0));
};

TTS_CASE("Limit values for user-defined type")
{
  auto lim = tts::limits(tts::type<data> {});
  TTS_EQUAL(lim.valmax.i, std::numeric_limits<int>::max());
  TTS_EQUAL(lim.valmax.f, std::numeric_limits<float>::max());
  TTS_EQUAL(lim.valmin.i, std::numeric_limits<int>::lowest());
  TTS_EQUAL(lim.valmin.f, std::numeric_limits<float>::lowest());
  TTS_EQUAL(lim.zero.i, 0);
  TTS_EQUAL(lim.zero.f, 0.0f);
  TTS_EQUAL(lim.one.i, 1);
  TTS_EQUAL(lim.one.f, 1.0f);
};
//! [snippet]
