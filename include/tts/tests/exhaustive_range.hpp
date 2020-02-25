//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_EXHAUSTIVE_RANGE_HPP_INCLUDED
#define TTS_TESTS_EXHAUSTIVE_RANGE_HPP_INCLUDED

#include <tts/tests/range.hpp>
#include <algorithm>

namespace tts
{
  //////////////////////////////////////////////////////////////////////////////////////////////////
  template<typename T>
  struct exhaustive_producer : tts::producer<exhaustive_producer<T>>
  {
    using value_type = T;

    T first() const noexcept  { return first_;  }
    T last()  const noexcept  { return last_;   }

    T next() noexcept
    {
      T that( current_ );
      current_ = tts::detail::next(current_);
      return std::clamp(that, pmi_, pmx_);
    }

    std::size_t size() const noexcept { return size_; }

    template<typename U, typename V>
    exhaustive_producer(U mn, V mx)
                : current_( static_cast<T>(mn) )
                , first_(static_cast<T>(mn)), last_(static_cast<T>(mx))
                , pmi_(first_)
                , pmx_(tts::detail::prev(last_))
                , size_(tts::detail::nb_values(first_,last_))
    {}

    template<typename P>
    exhaustive_producer ( P const& src, std::size_t i0, std::size_t, std::size_t)
                  : exhaustive_producer(src.self())
    {
      current_ = std::clamp(tts::detail::next(current_,i0), pmi_, pmx_);
    }

    private:
    T           current_,first_, last_, pmi_, pmx_;
    std::size_t size_;
  };
}

#endif
