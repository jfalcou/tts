//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_RANDOM_RANGE_HPP_INCLUDED
#define TTS_TESTS_RANDOM_RANGE_HPP_INCLUDED

#include <tts/tests/range.hpp>
#include <algorithm>
#include <random>

namespace tts
{
  template<typename T>
  using unifrom_distribution = std::conditional_t < std::is_floating_point_v<T>
                                                  , std::uniform_real_distribution<T>
                                                  , std::uniform_int_distribution<T>
                                                  >;

  //////////////////////////////////////////////////////////////////////////////////////////////////
  template<typename T, typename Distribution = unifrom_distribution<T>>
  struct rng_producer : tts::producer<rng_producer<T,Distribution>>
  {
    using value_type        = T;
    using distribution_type = Distribution;

    T first() const noexcept  { return first_; }
    T last()  const noexcept  { return last_;  }
    T next()        noexcept  { return T{distribution_(generator_)}; }

    std::size_t size() const noexcept { return size_; }

    template<typename U, typename V>
    rng_producer(U mn, V mx)
      : distribution_(static_cast<value_type>(mn),static_cast<value_type>(mx))
      , first_(mn)
      , last_(mx)
      , seed_{std::size_t(this->prng_seed()), std::size_t(0), std::size_t(1), this->count()}
      , generator_(seed_)
      , size_(this->count())
    {}

    template<typename P>
    rng_producer ( P const& src, std::size_t i0, std::size_t i1, std::size_t s)
                  : distribution_(src.self().distribution_)
                  , seed_{std::size_t(this->prng_seed()),i0,i1,s}
                  , generator_(seed_)
                  , size_(src.self().size_)
    {}

    private:
    distribution_type distribution_;
    T                 first_;
    T                 last_;
    std::seed_seq     seed_;
    std::mt19937      generator_;
    std::size_t       size_;
  };
}

#endif
