//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/tools/concepts.hpp>
#include <tts/tools/random.hpp>
#include <tts/tools/types.hpp>
#include <tuple>

namespace tts
{
  //================================================================================================
  // Single evaluation of value extension point
  //================================================================================================
  template<typename T, typename V> auto as_value(V const& v) { return static_cast<T>(v); }

  //================================================================================================
  // Generator extension point
  //================================================================================================
  template<typename T> auto produce(type<T> const& t, auto g, auto& rng, auto... others)
  {
    return g(t,rng, others...);
  }

  //================================================================================================
  // Generators combinator
  //================================================================================================
  template<typename... G> inline auto generate(G... g)
  {
    return [=](auto const& t, auto& rng, auto... others)
    {
      return std::make_tuple(produce(t,g,rng,others...)...);
    };
  }

  //================================================================================================
  // Generator extension point for sequence
  //================================================================================================
  template<tts::sequence T>
  auto produce(type<T> const& t, auto g, auto& rng, auto... args)
  {
    using value_type = std::remove_cvref_t<decltype(*std::begin(std::declval<T>()))>;

    T that;

    auto b = std::begin(that);
    auto e = std::end(that);
    auto sz = e - b;

    for(std::ptrdiff_t i=0;i<sz;++i)
    {
      *b++ = as_value<value_type>(g(tts::type<value_type>{},rng,i,sz,args...));
    }

    return that;
  }

  //================================================================================================
  // Constant value generator : [v v ... v]
  //================================================================================================
  template<typename T> struct value
  {
    value(T v) : seed(v) {}

    template<typename U>
    auto operator()(tts::type<U>, auto&, auto...) const
    {
      return as_value<U>(seed);
    }

    T seed;
  };

  //================================================================================================
  // Ramp generator : [s s+st s+2*st ... ]
  //================================================================================================
  template<typename T, typename U = T> struct ramp
  {
    ramp(T s)       : start(s), step(1)   {}
    ramp(T s, U st) : start(s), step(st)  {}

    template<typename D>
    auto operator()(tts::type<D>, auto&) const { return as_value<D>(start); }

    template<typename D>
    auto operator()(tts::type<D>, auto&, auto idx, auto...) const
    {
      return as_value<D>(start)+idx*as_value<D>(step);
    }

    T start;
    U step;
  };

  //================================================================================================
  // Reverse ramp generator : [ ... s+2*st s+st s ]
  //================================================================================================
  template<typename T, typename U = T> struct reverse_ramp
  {
    reverse_ramp(T s)       : start(s), step(1)   {}
    reverse_ramp(T s, U st) : start(s), step(st)  {}

    template<typename D>
    auto operator()(tts::type<D>, auto&) const { return as_value<D>(start); }

    template<typename D>
    auto operator()(tts::type<D>, auto&, auto idx, auto sz, auto...) const
    {
      return as_value<D>(start)+(sz-1-idx)*as_value<D>(step);
    }

    T start;
    U step;
  };

  //================================================================================================
  // Generator between two values: [ a a+s ... a+(n-2)*s b ]
  //================================================================================================
  template<typename T, typename U = T> struct between
  {
     between(T s, U st) : first(s), last(st)  {}

    template<typename D>
    auto operator()(tts::type<D>, auto&) const { return as_value<D>(first); }

    template<typename D>
    auto operator()(tts::type<D>, auto&, auto idx, auto sz, auto...) const
    {
      auto w1   = as_value<D>(first);
      auto w2   = as_value<D>(last);
      auto step = (sz-1) ? (w2-w1)/(sz-1) : 0;
      return std::min( as_value<D>(w1 + idx*step), w2);
    }

    T first;
    U last;
  };

  //================================================================================================
  // Random generator from an existing distribution
  //================================================================================================
  template<typename Distribution> struct sample
  {
    sample(Distribution d)  : dist(std::move(d))  {}

    template<typename D> auto operator()(tts::type<D>, auto& rng, auto...)
    {
      return as_value<D>(dist(rng));
    }

    Distribution dist;
  };

  //================================================================================================
  // Random generator between two bounds using realistic_distribution
  //================================================================================================
  template<typename Mx, typename Mn> struct randoms
  {
    randoms(Mn mn, Mx mx)  : mini(mn), maxi(mx)  {}

    template<typename D> auto operator()(tts::type<D>, auto& rng, auto...)
    {
      static tts::realistic_distribution<D> dist(as_value<D>(mini), as_value<D>(maxi));
      return dist(rng);
    }

    Mn mini;
    Mx maxi;
  };
}
