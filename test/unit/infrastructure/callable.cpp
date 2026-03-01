//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE("Check empty callable behavior")
{
  tts::_::callable f;
  TTS_EXPECT(!f);
};

inline int outside_data = 0;
void       outside_function()
{
  outside_data = 99;
}

TTS_CASE("Check callable using a function")
{
  tts::_::callable f = outside_function;

  TTS_EXPECT(!!f);
  TTS_EQUAL(outside_data, 0);
  f();
  TTS_EQUAL(outside_data, 99);
};

TTS_CASE("Check callable using a lambda")
{
  int              k = 0;
  tts::_::callable f = [ & ] { k++; };

  TTS_EXPECT(!!f);
  TTS_EQUAL(k, 0);
  f();
  TTS_EQUAL(k, 1);
};

TTS_CASE("Check callable is movable only")
{
  int              k = 0;
  tts::_::callable f = [ & ] { k++; };
  tts::_::callable g;

  TTS_EXPECT_NOT_COMPILES(f, g, { g = f; });
  TTS_EXPECT_NOT_COMPILES(f, { tts::_::callable(f); });

  g = std::move(f);

  TTS_EXPECT(!!g);
  TTS_EXPECT(!f);
  TTS_EQUAL(k, 0);
  g();
  TTS_EQUAL(k, 1);

  tts::_::callable h = std::move(g);
  TTS_EXPECT(!!h);
  TTS_EXPECT(!g);
  h();
  TTS_EQUAL(k, 2);
};
