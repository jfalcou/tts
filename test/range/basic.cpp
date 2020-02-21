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

template<typename T> struct some_producer : tts::producer<some_producer<T>>
{
  using value_type = T;

  some_producer(std::size_t count, value_type s = {}) : seed_{s}, count_(count) {}

  template<typename P>
  some_producer ( P const& src, std::size_t i0, std::size_t i1, std::size_t s)
                : some_producer(src.self())
  {
    seed_ += i0;
  }

  auto first()  const noexcept { return seed_;           }
  auto last()   const noexcept { return seed_ + count_;  }

  value_type  next() noexcept
  {
    auto old = seed_;
    seed_++;
    return old;
  }

  std::size_t size() const noexcept { return count_; }

  private:
  T seed_;
  std::size_t count_;
};

float ok_x  (float x) { return x; }
float ajar_x(float x) { return x+1e-7f; }

TTS_CASE( "Test some range on float" )
{
  some_producer<float>  p(200, 1.f);
  TTS_ULP_RANGE_CHECK(p,ok_x,ajar_x,16);
}

std::int8_t absc  (std::int8_t x) { return x; }
std::int8_t absc_c(std::int8_t x) { return x != 78 ? x+1 : x; }

TTS_CASE( "Test some range on int8" )
{
  some_producer<std::int8_t>  p(256,-128);
  TTS_ULP_RANGE_CHECK(p,absc,absc_c,256);
}
