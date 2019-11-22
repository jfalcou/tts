//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

#include <tts/tests/range.hpp>
#include <cmath>

template<typename T> struct random_producer : tts::producer<random_producer<T>>
{
  using value_type = T;

  random_producer(std::size_t count, value_type s = {}) : seed_{s}, count_(count), step_{0} {}

  value_type  next()
  {
    step_++;
    auto old = seed_;
    seed_ += 0.01f;
    return old;
  }

  std::size_t size() const { return count_; }

  private:
  T seed_;
  std::size_t count_, step_;
};

float sin_x(float x)      { return std::sqrt(x); }
float sin_rough(float x)  { return std::sqrt(x)+1e-4; }

int main()
{
  random_producer<float>  p(147896325);
  TTS_RANGE_CHECK(p,sin_x,sin_rough);

  return 0;
}
