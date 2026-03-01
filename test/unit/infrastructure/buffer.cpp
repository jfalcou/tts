//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>

TTS_CASE("Check default constructed buffer")
{
  tts::_::buffer<int> b;

  TTS_EQUAL(b.size(), 0UL);
  TTS_EQUAL(b.capacity(), 0UL);
  TTS_EQUAL(b.begin(), b.end());
};

TTS_CASE("Check sized buffer")
{
  constexpr std::size_t n = 7;
  tts::_::buffer<int>   b(n);

  TTS_EQUAL(b.size(), n);
  TTS_EQUAL(b.capacity(), n);
  TTS_EQUAL(b.end(), b.begin() + n);

  for(auto& e: b)
    e = 1;
  for(auto e: b)
    TTS_EQUAL(e, 1);
};

TTS_CASE("Check buffer copy semantic")
{
  constexpr std::size_t n = 7;
  tts::_::buffer<int>   original(n);
  for(auto& e: original)
    e = 1;

  tts::_::buffer<int> copy = original;

  TTS_EQUAL(copy.size(), n);
  TTS_EQUAL(copy.capacity(), n);
  TTS_EQUAL(copy.end(), copy.begin() + n);
  TTS_NOT_EQUAL(copy.begin(), original.begin());

  tts::_::buffer<int> second_copy;
  second_copy = original;

  TTS_EQUAL(second_copy.size(), n);
  TTS_EQUAL(second_copy.capacity(), n);
  TTS_EQUAL(second_copy.end(), second_copy.begin() + n);
  TTS_NOT_EQUAL(second_copy.begin(), original.begin());
};

TTS_CASE("Check buffer move semantic")
{
  constexpr std::size_t n = 7;
  tts::_::buffer<int>   original(n);
  for(auto& e: original)
    e = 1;

  auto                ptr  = original.begin();

  tts::_::buffer<int> copy = std::move(original);

  TTS_EQUAL(copy.size(), n);
  TTS_EQUAL(copy.capacity(), n);
  TTS_EQUAL(copy.end(), copy.begin() + n);
  TTS_EQUAL(copy.begin(), ptr);

  tts::_::buffer<int> second_copy;
  second_copy = std::move(copy);

  TTS_EQUAL(second_copy.size(), n);
  TTS_EQUAL(second_copy.capacity(), n);
  TTS_EQUAL(second_copy.end(), second_copy.begin() + n);
  TTS_EQUAL(second_copy.begin(), ptr);
};
