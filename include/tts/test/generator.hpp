//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/tools/concepts.hpp>
#include <tts/tools/random.hpp>
#include <tts/tools/types.hpp>
#include <tuple>

namespace tts
{
  //====================================================================================================================
  /**
    @brief Single evaluation of value extension point

    This function generate a `T` from a value `v`. Default behavior is to convert but this function can be
    overloaded to handle suer-defined type.

    @tparam T Target data type
    @param  v Value used ot generate the data sample
    @return A value of type `T` obtained by conversion of `v`.
  **/
  //====================================================================================================================
  template<typename T, typename V> auto as_value(V const& v) { return static_cast<T>(v); }

  //====================================================================================================================
  // Generator extension point for sequence
  //====================================================================================================================
  template<tts::detail::sequence Seq, typename U> struct rebuild;

  template<template<class,class...> class Seq, typename T, typename... S, typename U>
  struct rebuild<Seq<T,S...>,U> { using type = Seq<U,S...>; };

  template<template<class,std::size_t> class Seq, typename T, std::size_t N, typename U>
  struct rebuild<Seq<T,N>,U>    { using type = Seq<U,N>; };

  //====================================================================================================================
  // Generator extension point
  //====================================================================================================================
  template<typename T> auto produce(type<T> const& t, auto g, auto& rng, auto... others)
  {
    return g(t,rng, others...);
  }

  template<tts::detail::sequence T>
  auto produce(type<T> const&, auto g, auto& rng, auto... args)
  {
    using elmt_type   = std::remove_cvref_t<decltype(*std::begin(std::declval<T>()))>;
    using value_type  = decltype(produce(tts::type<elmt_type>{},g,rng,args...));

    typename rebuild<T,value_type>::type that;
    auto b = std::begin(that);
    auto e = std::end(that);
    auto sz = e - b;

    for(std::ptrdiff_t i=0;i<sz;++i)
    {
      *b++ = produce(tts::type<value_type>{},g,rng,i,sz,args...);
    }
    return that;
  }

  //====================================================================================================================
  /**
    @brief Build a data generator

    @param  g  List of generator to aggregate
    @return A function producing the tuple containing an aggregation of all the data generator passed as a parameter
  **/
  //====================================================================================================================
  template<typename... G> inline auto generate(G... g)
  {
    return [=](auto const& t, auto& rng, auto... others)
    {
      return std::make_tuple(produce(t,g,rng,others...)...);
    };
  }

  //====================================================================================================================
  /**
    @brief Defines a data generator that always return the same value.

    @groupheader{Example}
    @code
    #define TTS_MAIN
    #include <tts/tts.hpp>

    TTS_CASE_WITH ( "Check behavior for value generator"
                  , tts::arithmetic_types
                  , tts::generate(tts::value{42} )
                  )
    (auto value)
    {
      TTS_EQUAL(value , 42 );
    };
    @endcode
  **/
  //====================================================================================================================
  template<typename T> struct value
  {
    /// Set the value to be generated
    value(T v) : seed(v) {}

    template<typename U>
    auto operator()(tts::type<U>, auto&, auto...) const { return as_value<U>(seed); }

    T seed;
  };

  //====================================================================================================================
  /**
    @brief Defines a data generator that produce a ramp of data

    @groupheader{Example}
    @code
    #define TTS_MAIN
    #include <tts/tts.hpp>
    #include <array>

    TTS_CASE_WITH ( "Check behavior for ramp generator"
                  , (std::array<int,4>)
                  , tts::generate(tts::ramp{65}, tts::ramp{1,2})
                  )
    (auto ramp1, auto ramp2)
    {
      for(int i=0;i<ramp1.size();++i)
        TTS_EQUAL(ramp1[i],65+i);

      for(int i=0;i<ramp2.size();++i)
        TTS_EQUAL(ramp2[i],1+i*2);
    };
    @endcode
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct ramp
  {
    /// Define a ramp with an initial value and an unit step
    ramp(T s)       : start(s), step(1)   {}

    /// Define a ramp with an initial value and a custom step
    ramp(T s, U st) : start(s), step(st)  {}

    template<typename D>
    auto operator()(tts::type<D>, auto&) const { return as_value<D>(start); }

    template<typename D>
    auto operator()(tts::type<D>, auto&, auto idx, auto...) const { return as_value<D>(start+idx*step); }

    T start;
    U step;
  };

  //====================================================================================================================
  /**
    @brief Defines a data generator that produce a reversed ramp of data

    @groupheader{Example}
    @code
    #define TTS_MAIN
    #include <tts/tts.hpp>
    #include <array>

    TTS_CASE_WITH ( "Check behavior for reverse_ramp generator"
                  , (std::array<int,4>)
                  , tts::generate(tts::reverse_ramp{0}, tts::reverse_ramp{10,2})
                  )
    (auto ramp1, auto ramp2)
    {
      TTS_EQUAL(ramp1[0],3);
      TTS_EQUAL(ramp1[1],2);
      TTS_EQUAL(ramp1[2],1);
      TTS_EQUAL(ramp1[3],0);

      TTS_EQUAL(ramp2[0],16);
      TTS_EQUAL(ramp2[1],14);
      TTS_EQUAL(ramp2[2],12);
      TTS_EQUAL(ramp2[3],10);
    };
    @endcode
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct reverse_ramp
  {
    /// Define a reversed ramp with a final value and an unit step
    reverse_ramp(T s)       : start(s), step(1)   {}

    /// Define a reversed ramp with a final value and a custom step
    reverse_ramp(T s, U st) : start(s), step(st)  {}

    template<typename D>
    auto operator()(tts::type<D>, auto&) const { return as_value<D>(start); }

    template<typename D>
    auto operator()(tts::type<D>, auto&, auto idx, auto sz, auto...) const { return as_value<D>(start+(sz-1-idx)*step); }

    T start;
    U step;
  };

  //====================================================================================================================
  /**
    @brief Defines a data generator that produce value within an interval

    @groupheader{Example}
    @code
    #define TTS_MAIN
    #include <tts/tts.hpp>
    #include <array>

    TTS_CASE_WITH ( "Check behavior for interval generator"
                  , (std::array<int,7>)
                  , tts::generate(tts::between{-3,3})
                  )
    (auto data)
    {
      TTS_EQUAL(data[0],-3);
      TTS_EQUAL(data[1],-2);
      TTS_EQUAL(data[2],-1);
      TTS_EQUAL(data[3],0);
      TTS_EQUAL(data[4],1);
      TTS_EQUAL(data[5],2);
      TTS_EQUAL(data[6],3);
    };
    @endcode
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct between
  {
    /// Define the interval for the generator
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

  //====================================================================================================================
  /**
    @brief Defines a data generator that produce random value using a given distribution

    @groupheader{Example}
    @code
    #define TTS_MAIN
    #include <tts/tts.hpp>
    #include <array>

    TTS_CASE_WITH ( "Check behavior for interval generator"
                  , (std::array<float,10>)
                  , tts::generate(tts::sample{ std::uniform_real_distribution{-2.,2.} })
                  )
    (auto data)
    {
      for(auto v : data)
        TTS_EXPECT(v>=-2.f && v<2.f);
    };
    @endcode
  **/
  //====================================================================================================================
  template<typename Distribution> struct sample
  {
    /// Specify the distribution to use for sampling data
    sample(Distribution d)  : dist(std::move(d))  {}
    template<typename D> auto operator()(tts::type<D>, auto& rng, auto...) { return dist(rng); }
    Distribution dist;
  };

  //====================================================================================================================
  // Random generator between two bounds using realistic_distribution
  //====================================================================================================================
  template<typename Mx, typename Mn> struct randoms
  {
    randoms(Mn mn, Mx mx)  : mini(mn), maxi(mx)  {}

    template<typename D> auto operator()(tts::type<D>, auto& rng, auto...)
    {
      tts::realistic_distribution<D> dist(as_value<D>(mini), as_value<D>(maxi));
      return  dist(rng);
    }

    Mn mini;
    Mx maxi;
  };
}
