//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/tts.hpp>
#include <tts/tests/range.hpp>
#include <cmath>

template<typename T> struct some_producer : tts::producer<some_producer<T>>
{
  using value_type = T;

  some_producer(std::size_t count, value_type s = {}) : seed_{s}, count_(count) {}

  template<typename P>
  some_producer ( P const& src, std::size_t i, std::size_t p, std::size_t)
                : some_producer(src.self())
  {
    seed_ += i*p;
  }

  value_type  next()
  {
    auto old = seed_;
    seed_ += 1.f;
    return old;
  }

  std::size_t size() const { return count_; }

  private:
  T seed_;
  std::size_t count_;
};

float ok_x  (float x) { return std::sqrt(x); }
float ajar_x(float x) { return std::sqrt(x)+1e-6; }

TTS_CASE( "Default-constructed pointer behavior" )
{
  some_producer<float>  p(200);
  TTS_RANGE_CHECK(p,ok_x,ajar_x);
}
