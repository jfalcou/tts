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
#include <limits>

namespace tts
{
  namespace _
  {
    template<std::size_t N, bool Signed> struct sized_integer;

    template<> struct sized_integer<1, true>
    {
      using type = std::int8_t;
    };

    template<> struct sized_integer<2, true>
    {
      using type = std::int16_t;
    };

    template<> struct sized_integer<4, true>
    {
      using type = std::int32_t;
    };

    template<> struct sized_integer<8, true>
    {
      using type = std::int64_t;
    };

    template<> struct sized_integer<1, false>
    {
      using type = std::uint8_t;
    };

    template<> struct sized_integer<2, false>
    {
      using type = std::uint16_t;
    };

    template<> struct sized_integer<4, false>
    {
      using type = std::uint32_t;
    };

    template<> struct sized_integer<8, false>
    {
      using type = std::uint64_t;
    };

    template<typename T, bool Signed = false>
    using sized_integer_t = typename sized_integer<sizeof(T), Signed>::type;
  }

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

    This function generate a `T` from a value `v`. Default behavior is to convert but this function
  can be overloaded to handle user-defined type.

    @tparam T Target data type
    @param  v Value used ot generate the data sample
    @return A value of type `T` obtained by conversion of `v`.
  **/
  //====================================================================================================================
  template<typename T, typename V> auto as_value(V const& v)
  {
    return static_cast<T>(v);
  }

  //====================================================================================================================
  /**
    @brief Type conversion customization point

    Some generators need to rebuild a template sequence type changing only the element type. This
  structure provides a way to do so in an extensible way. The default implementation handles
  standard sequence types like `std::array` or `std::vector`.

    @tparam Seq Source sequence type.
    @tparam U   New element type.
  **/
  //====================================================================================================================
  template<tts::_::sequence Seq, typename U> struct rebuild;

  template<template<typename, typename...> typename Seq, typename T, typename... S, typename U>
  struct rebuild<Seq<T, S...>, U>
  {
    using type = Seq<U, S...>;
  };

  template<template<typename, std::size_t> typename Seq, typename T, std::size_t N, typename U>
  struct rebuild<Seq<T, N>, U>
  {
    using type = Seq<U, N>;
  };

  //====================================================================================================================
  /**
    @brief Data production customization point

    This function is in charge of producing data of type `T` using generator `g` and additional
  parameters `others...`. The default implementation simply calls the generator `g` with the
  provided parameters.

    By default, this function also handles sequence types by producing each of their elements using
  the provided generator.

    @tparam T Target data type
    @param  t      Type wrapper for the target data type
    @param  g      Data generator to use
    @param  others Additional parameters forwarded to the generator
    @return A value of type `T` produced by the generator `g`.
  **/
  //====================================================================================================================
  template<typename T> auto produce(type<T> const& t, auto g, auto... others)
  {
    return g(t, others...);
  }

  template<tts::_::sequence T> auto produce(type<T> const&, auto g, auto... args)
  {
    using elmt_type  = std::remove_cvref_t<decltype(*begin(tts::_::declval<T>()))>;
    using value_type = decltype(produce(tts::type<elmt_type> {}, g, 0, 0ULL, args...));

    typename rebuild<T, value_type>::type that;
    auto                                  b  = begin(that);
    auto                                  e  = end(that);
    std::ptrdiff_t                        sz = e - b;

    for(std::ptrdiff_t i = 0; i < sz; ++i)
    {
      *b++ = produce(tts::type<value_type> {}, g, i, sz, args...);
    }
    return that;
  }

  //====================================================================================================================
  /// @}
  //====================================================================================================================

  //====================================================================================================================
  /**
    @brief Base type extraction

    This structure is used to extract the base type of a type `T`. By default, the base type is `T`
    itself, but this structure can be specialized for user-defined types to provide a different base
    type. The base type is used to determine the limits and special values for the type `T` when
    generating test data.

    **Helper types:**
    - `base_type_t<T>`: Alias for `typename base_type<T>::type`

    @tparam T Type for which to extract the base type.
  **/
  //====================================================================================================================
  template<typename T> struct base_type
  {
    using type = T;
  };

  template<typename T> using base_type_t = typename base_type<T>::type;

  //====================================================================================================================
  /**
    @brief Boolean type computation

    This structure is used to compute a boolean type for a type `T`. By default, the boolean type is
    `bool`, but this structure can be specialized for user-defined types to provide a different
  boolean type.

      **Helper types:**
      - `boolean_type_t<T>`: Alias for `typename boolean_type<T>::type`

      @tparam T Type for which to compute the boolean type.
  **/
  //====================================================================================================================
  template<typename T> struct boolean_type
  {
    using type = bool;
  };

  template<typename T> using boolean_type_t = typename boolean_type<T>::type;

  namespace _
  {
    struct empty_limits
    {
    };

    template<typename T> struct common_limits
    {
      using b_t = base_type_t<T>;
      static_assert(std::numeric_limits<b_t>::is_specialized,
                    "Base type must have std::numeric_limits specialized");

      T valmax = static_cast<T>(std::numeric_limits<b_t>::max());
      T valmin = static_cast<T>(std::numeric_limits<b_t>::lowest());
      T zero   = static_cast<T>(0);
      T one    = static_cast<T>(1);
    };

    template<typename T> struct float_limits
    {
      using b_t = base_type_t<T>;

      static constexpr b_t get_maxflint()
      {
        if constexpr(sizeof(b_t) == 4) return static_cast<b_t>(0x1p+24);
        else return static_cast<b_t>(0x1p+53);
      }

      T nan            = static_cast<T>(std::numeric_limits<b_t>::quiet_NaN());
      T inf            = static_cast<T>(std::numeric_limits<b_t>::infinity());
      T minf           = static_cast<T>(-std::numeric_limits<b_t>::infinity());
      T mzero          = static_cast<T>(-0.0);
      T maxflint       = static_cast<T>(get_maxflint());
      T mindenormal    = static_cast<T>(std::numeric_limits<b_t>::denorm_min());
      T smallestposval = static_cast<T>(std::numeric_limits<b_t>::min());
      T mone           = static_cast<T>(-1.0);
    };
  }

  //====================================================================================================================
  /**
    @defgroup tools-generators-class Data Generators
    @brief Generators to produce data samples for testing.
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @brief Defines a set of limits for a type `T`.

    This structure provides a set of limits and special values for the type `T` that can be used for
    generating test data that covers edge cases and special values. The actual limits provided
    depend on the type `T` and its base type.

    The provided limits include:
    - `valmax`: The maximum representable value of the base type.
    - `valmin`: The minimum representable value of the base type.
    - `zero`: The value representing zero.
    - `one`: The value representing one.

    For floating-point types, additional limits include:
    - `nan`: Not-a-Number value.
    - `inf`: Positive infinity.
    - `minf`: Negative infinity.
    - `maxflint`: The largest integer value that can be exactly represented in the type.
    - `mindenormal`: The smallest positive subnormal value.
    - `smallestposval`: The smallest positive normal value.
    - `mzero`: Negative zero.
    - `mone`: The value representing negative one.

    This class can be specialized for user-defined types to provide custom limits.

    @groupheader{Examples}
    @snippet doc/limits.cpp snippet

    @tparam T Type for which to provide limits for.
  **/
  //====================================================================================================================
  template<typename T>
  struct limits_set
      : _::common_limits<T>
      , std::conditional_t<std::floating_point<base_type_t<T>>, _::float_limits<T>, _::empty_limits>
  {
    using type = T;
  };

  //====================================================================================================================
  /**
    @brief Provides a set of limits for the type `T`.

    This function returns a structure containing various limits and special values for the type `T`
    that can be used for generating test data that covers edge cases and special values.

    @tparam T Type for which to provide limits
    @return A instance of `limits_set<T>` containing limits and special values for type `T`.
  **/
  //====================================================================================================================
  template<typename T> inline auto limits(tts::type<T>)
  {
    return limits_set<T> {};
  }

  //====================================================================================================================
  /**
    @brief Defines a data generator that always return the same value.

    This generator produces always the same value provided at construction converted to the target
    type.

    @tparam T Type of the value to be produced

    @groupheader{Example}
    @snippet doc/generator_value.cpp snippet
  **/
  //====================================================================================================================
  template<typename T> struct value
  {
    constexpr value(T v)
        : seed(v)
    {
    }

    template<typename D> D operator()(tts::type<D>, auto...) const
    {
      return as_value<D>(seed);
    }

    T seed;
  };

  template<typename T, typename U = T> struct logicals
  {
    constexpr logicals(T v, U k)
        : start(v)
        , range(k)
    {
    }

    template<typename D> auto operator()(tts::type<D>) const
    {
      return as_value<tts::boolean_type_t<D>>(false);
    }

    template<typename D> auto operator()(tts::type<D>, auto idx, auto...) const
    {
      return as_value<tts::boolean_type_t<D>>(((start + idx) % range) == 0);
    }

    T start;
    U range;
  };

  //====================================================================================================================
  /**
    @brief Defines a data generator that produce a ramp of data.

    This generator produces a ramp starting from an initial value and increasing by a fixed step at
    each call. I.e., for a size `N`, the produced values are: `start, start+step, start+2*step, ...,
    start+(N-1)*step`.

    @tparam T Type of the initial value
    @tparam U Type of the step value

    @groupheader{Example}
    @snippet doc/generator_ramp.cpp snippet
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct ramp
  {
    constexpr ramp(T s)
        : start(s)
        , step(1)
    {
    }
    constexpr ramp(T s, U st)
        : start(s)
        , step(st)
    {
    }

    template<typename D> D operator()(tts::type<D>, auto idx, auto...) const
    {
      return as_value<D>(start + idx * step);
    }

    template<typename D> D operator()(tts::type<D>) const
    {
      return as_value<D>(start);
    }

    T start;
    U step;
  };

  //====================================================================================================================
  /**
    @brief Defines a data generator that produce a reverse ramp of data.

    This generator produces a ramp starting from a final value and decreasing by a fixed step at
    each call. I.e., for a size `N`, the produced values are: `start, start-step, start-2*step, ...,
    start-(N-1)*step`.

    @tparam T Type of the initial value
    @tparam U Type of the step value

    @groupheader{Example}
    @snippet doc/generator_reverse_ramp.cpp snippet
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct reverse_ramp
  {
    constexpr reverse_ramp(T s)
        : start(s)
        , step(1)
    {
    }
    constexpr reverse_ramp(T s, U st)
        : start(s)
        , step(st)
    {
    }

    template<typename D> D operator()(tts::type<D>, auto idx, auto...) const
    {
      return as_value<D>(start - idx * step);
    }

    template<typename D> D operator()(tts::type<D>) const
    {
      return as_value<D>(start);
    }

    T start;
    U step;
  };

  //====================================================================================================================
  /**
    @brief Defines a data generator that produce values between two bounds.

    This generator produces values linearly spaced between two bounds (inclusive). For a size `N`,
    the produced values are: `first, first+step, first+2*step, ..., last`, where `step =
    (last-first)/(N-1)`.

    When generated values exceed the last bound due to rounding, the last bound is returned instead.

    @tparam T Type of the first bound
    @tparam U Type of the last bound

    @groupheader{Example}
    @snippet doc/generator_between.cpp snippet
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct between
  {
    constexpr between(T first, U last)
        : first_(first)
        , last_(last)
    {
    }

    template<typename D> D operator()(tts::type<D>, auto idx, auto sz, auto...) const
    {
      auto w1   = as_value<D>(first_);
      auto w2   = as_value<D>(last_);
      auto step = (sz - 1) ? (w2 - w1) / as_value<D>(sz - 1) : 0;
      return _::min(as_value<D>(w1 + idx * step), w2);
    }

    template<typename D> D operator()(tts::type<D>) const
    {
      return as_value<D>(first_);
    }

    T first_;
    U last_;
  };

  //====================================================================================================================
  /**
    @brief Random generator between two bounds using realistic_distribution

    This generator produces random values between two bounds (inclusive) using a realistic
    distribution. The realistic distribution aims to provide a more uniform coverage of the range by
    avoiding too many extreme values.

    @tparam Mx Type of the upper bound
    @tparam Mn Type of the lower bound

    @groupheader{Example}
    @snippet doc/generator_randoms.cpp snippet
  **/
  //====================================================================================================================
  template<typename Mx, typename Mn> struct randoms
  {
    constexpr randoms(Mn mn, Mx mx)
        : mini(mn)
        , maxi(mx)
    {
    }

    template<typename D> D operator()(tts::type<D>, auto...)
    {
      if constexpr(std::is_unsigned_v<D>)
      {
        assert(mini >= 0 &&
               "Minimum value for unsigned type random generator must be non-negative");
        assert(maxi >= 0 &&
               "Maximum value for unsigned type random generator must be non-negative");
      }
      return random_value(as_value<D>(mini), as_value<D>(maxi));
    }

    Mn mini;
    Mx maxi;
  };

  namespace _
  {
    struct random_bits_t
    {
      template<typename D> auto operator()(tts::type<D>, auto...)
      {
        using i_t = tts::_::sized_integer_t<tts::base_type_t<D>>;
        return tts::random_value<i_t>(0, 8 * sizeof(i_t) - 1);
      }
    };
  }

  //====================================================================================================================
  /**
    @brief Produces random bits patterns

    This generator produces random bit patterns by generating a random integer value and
  interpreting its bits as the bits of the target type. The number of bits generated is equal to the
  number of bits in the target type.

    @groupheader{Example}
    @snippet doc/generator_random_bits.cpp snippet
  **/
  //====================================================================================================================
  inline constexpr _::random_bits_t random_bits = {};

  //====================================================================================================================
  /**
    @brief Converts a generator to produce integer values

    This generator adapter takes a generator `g` and produces integer values by generating values
    using `g` and converting them to integers. The conversion is done by interpreting the generated
    values as integers of the same size as the base type of the target type.

    @groupheader{Example}
    @snippet doc/generator_as_integer.cpp snippet

    @tparam G Type of the generator to adapt
  **/
  //====================================================================================================================
  template<typename G> struct as_integer
  {
    constexpr explicit as_integer(G g)
        : generator_(g)
    {
    }
    template<typename D> auto operator()(tts::type<D>, auto... args)
    {
      using i_t = tts::_::sized_integer_t<tts::base_type_t<D>>;
      return generator_(tts::type<i_t> {}, args...);
    }
    G generator_;
  };

  //====================================================================================================================
  /**
    @brief Converts a generator to produce integer values

    This generator adapter takes a generator `g` and produces signed integer values by generating
  values using `g` and converting them to integers. The conversion is done by interpreting the
  generated values as integers of the same size as the base type of the target type.

    @groupheader{Example}
    @snippet doc/generator_as_signed_integer.cpp snippet

    @tparam G Type of the generator to adapt
  **/
  //====================================================================================================================
  template<typename G> struct as_signed_integer
  {
    constexpr explicit as_signed_integer(G g)
        : generator_(g)
    {
    }
    template<typename D> auto operator()(tts::type<D>, auto... args)
    {
      using i_t = tts::_::sized_integer_t<tts::base_type_t<D>, true>;
      tts::type<i_t> tgt {};
      return generator_(tgt, args...);
    }
    G generator_;
  };

  //====================================================================================================================
  /// @}
  //====================================================================================================================
}
