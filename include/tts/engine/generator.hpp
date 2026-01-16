//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/concepts.hpp>
#include <tts/tools/random.hpp>
#include <tts/tools/types.hpp>

namespace tts
{
  //====================================================================================================================
  /**
    @defgroup tools-generators Data Generators related tools
    @brief Tools for data generation.
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @defgroup tools-generators-custom Data Generators Customization Points
    @brief Customization points to customize data generation for user-defined types.
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @brief Single value evaluation customization point

    This function generate a `T` from a value `v`. Default behavior is to convert but this function can be
    overloaded to handle user-defined type.

    @tparam T Target data type
    @param  v Value used ot generate the data sample
    @return A value of type `T` obtained by conversion of `v`.
  **/
  //====================================================================================================================
  template<typename T, typename V> auto as_value(V const& v) { return static_cast<T>(v); }

  //====================================================================================================================
  /**
    @brief Type conversion customization point

    Some generators need to rebuild a template sequence type changing only the element type. This structure provides a
    way to do so in an extensible way. The default implementation handles standard sequence types like `std::array` or
    `std::vector`.

    @tparam Seq Source sequence type.
    @tparam U   New element type.
  **/
  //====================================================================================================================
  template<tts::_::sequence Seq, typename U> struct rebuild;

  template<template<typename,typename...> typename Seq, typename T, typename... S, typename U>
  struct rebuild<Seq<T,S...>,U> { using type = Seq<U,S...>; };

  template<template<typename,std::size_t> typename Seq, typename T, std::size_t N, typename U>
  struct rebuild<Seq<T,N>,U>    { using type = Seq<U,N>; };

  //====================================================================================================================
  /**
    @brief Data production customization point

    This function is in charge of producing data of type `T` using generator `g` and additional parameters
    `others...`. The default implementation simply calls the generator `g` with the provided parameters.

    By default, this function also handles sequence types by producing each of their elements using the provided
    generator.

    @tparam T Target data type
    @param  t      Type wrapper for the target data type
    @param  g      Data generator to use
    @param  others Additional parameters forwarded to the generator
    @return A value of type `T` produced by the generator `g`.
  **/
  //====================================================================================================================
  template<typename T> auto produce(type<T> const& t, auto g, auto... others)
  {
    return g(t,others...);
  }

  template<tts::_::sequence T>
  auto produce(type<T> const&, auto g, auto... args)
  {
    using elmt_type   = std::remove_cvref_t<decltype(*begin(tts::_::declval<T>()))>;
    using value_type  = decltype(produce(tts::type<elmt_type>{},g,0,0ULL,args...));

    typename rebuild<T,value_type>::type that;
    auto b = begin(that);
    auto e = end(that);
    std::ptrdiff_t sz = e - b;

    for(std::ptrdiff_t i=0;i<sz;++i)
    {
      *b++ = produce(tts::type<value_type>{},g,i,sz,args...);
    }
    return that;
  }

  //====================================================================================================================
  /// @}
  //====================================================================================================================

  //====================================================================================================================
  /**
    @defgroup tools-generators-class Data Generators
    @brief Generators to produce data samples for testing.
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @brief Defines a data generator that always return the same value.

    This generator produces always the same value provided at construction converted to the target type.
    @tparam T Type of the value to be produced

    @groupheader{Example}
    @snippet doc/generator_value.cpp snippet
  **/
  //====================================================================================================================
  template<typename T> struct value
  {
    constexpr value(T v) : seed(v) {}

    template<typename D>
    D operator()(tts::type<D>, auto...) const { return as_value<D>(seed); }

    T seed;
  };

  //====================================================================================================================
  /**
    @brief Defines a data generator that produce a ramp of data.

    This generator produces a ramp starting from an initial value and increasing by a fixed step at each call.
    I.e., for a size `N`, the produced values are: `start, start+step, start+2*step, ..., start+(N-1)*step`.

    @tparam T Type of the initial value
    @tparam U Type of the step value

    @groupheader{Example}
    @snippet doc/generator_ramp.cpp snippet
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct ramp
  {
    constexpr ramp(T s)       : start(s), step(1)   {}
    constexpr ramp(T s, U st) : start(s), step(st)  {}

    template<typename D>
    D operator()(tts::type<D>, auto idx, auto...) const { return as_value<D>(start+idx*step); }

    template<typename D>
    D operator()(tts::type<D>) const { return as_value<D>(start); }

    T start;
    U step;
  };

  //====================================================================================================================
  /**
    @brief Defines a data generator that produce a reverse ramp of data.

    This generator produces a ramp starting from a final value and decreasing by a fixed step at each call.
    I.e., for a size `N`, the produced values are: `start, start-step, start-2*step, ..., start-(N-1)*step`.

    @tparam T Type of the initial value
    @tparam U Type of the step value

    @groupheader{Example}
    @snippet doc/generator_reverse_ramp.cpp snippet
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct reverse_ramp
  {
    constexpr reverse_ramp(T s)       : start(s), step(1)   {}
    constexpr reverse_ramp(T s, U st) : start(s), step(st)  {}

    template<typename D>
    D operator()(tts::type<D>, auto idx, auto...) const { return as_value<D>(start-idx*step); }

    template<typename D>
    D operator()(tts::type<D>) const { return as_value<D>(start); }

    T start;
    U step;
  };

  //====================================================================================================================
  /**
    @brief Defines a data generator that produce values between two bounds.

    This generator produces values linearly spaced between two bounds (inclusive). For a size `N`, the produced values
    are: `first, first+step, first+2*step, ..., last`, where `step = (last-first)/(N-1)`.

    When generated values exceed the last bound due to rounding, the last bound is returned instead.

    @tparam T Type of the first bound
    @tparam U Type of the last bound

    @groupheader{Example}
    @snippet doc/generator_between.cpp snippet
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct between
  {
    constexpr between(T first, U last) : first_(first), last_(last)  {}

    template<typename D>
    D operator()(tts::type<D>, auto idx, auto sz, auto...) const
    {
      auto w1   = as_value<D>(first_);
      auto w2   = as_value<D>(last_);
      auto step = (sz-1) ? (w2-w1)/(sz-1) : 0;
      return _::min( as_value<D>(w1 + idx*step), w2);
    }

    template<typename D>
    D operator()(tts::type<D>) const { return as_value<D>(first_); }

    T first_;
    U last_;
  };

  //====================================================================================================================
  /**
   * @brief Random generator between two bounds using realistic_distribution

    This generator produces random values between two bounds (inclusive) using a realistic distribution. The realistic
    distribution aims to provide a more uniform coverage of the range by avoiding too many extreme values.

    @tparam Mx Type of the upper bound
    @tparam Mn Type of the lower bound

    @groupheader{Example}
    @snippet doc/generator_randoms.cpp snippet
  **/
  //====================================================================================================================
  template<typename Mx, typename Mn> struct randoms
  {
    constexpr randoms(Mn mn, Mx mx)  : mini(mn), maxi(mx)  {}

    template<typename D> D operator()(tts::type<D>, auto...)
    {
      if constexpr(std::is_unsigned_v<D>)
      {
        assert(mini >= 0 && "Minimum value for unsigned type random generator must be non-negative");
        assert(maxi >= 0 && "Maximum value for unsigned type random generator must be non-negative");
      }
      return random_value(as_value<D>(mini), as_value<D>(maxi));
    }

    Mn mini;
    Mx maxi;
  };

  //====================================================================================================================
  /// @}
  //====================================================================================================================
}
