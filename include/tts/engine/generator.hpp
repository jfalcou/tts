//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <tts/tools/concepts.hpp>
#include <tts/tools/enumeration.hpp>
#include <tts/tools/random.hpp>
#include <tts/tools/types.hpp>

namespace tts
{
  namespace _
  {
    template<typename T, typename V> struct builtin_conversion
    {
      static auto from(V const& v)
      {
        return static_cast<T>(v);
      }
    };
  }

  //====================================================================================================================
  /**
    @defgroup tools-generators Data Generators related tools
    @brief Generators of test data and their customization points
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @defgroup tools-generators-custom Data Generators Customization Points
    @brief Customization points of the data generation for user-defined types
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @brief How a value is read as another type

    Generators funnel their bounds through this, so a bound written once has to answer for every
  type the case is run on. Specialize it for your own types rather than overloading convert_as. A
  type without a specialization keeps the built-in cast.

    The `convert_as` free function is the dispatcher rather than a customization point, and the
  call sites reach it qualified, so an overload of that name in another namespace is never found.
  It carries a generic default, which is why a leftover cannot be reported the way tts::precision
  and tts::comparison report theirs.

    @tparam T Target data type
    @tparam V Type of the value being read
  **/
  //====================================================================================================================
  template<typename T, typename V> struct conversion
  {
  };

  //====================================================================================================================
  /**
    @brief Single value evaluation customization point
  **/
  //====================================================================================================================
  template<typename T, typename V> auto convert_as(V const& v, type<T> const&)
  {
    if constexpr(requires { conversion<T, V>::from(v); }) return conversion<T, V>::from(v);
    else
    {
      static_assert(
      !requires { conversion<T, V>::from; },
      "[TTS] tts::conversion<T, V>::from does not accept a value of type V.");

      return _::builtin_conversion<T, V>::from(v);
    }
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
    @brief Produces a value of type `T` from a generator

    If tts::generation is specialized for `T`, calls `tts::generation<T>::make(g, others...)`.
    Otherwise, calls `g` with `t` and `others...` for a scalar type, or fills a sequence type
    element by element, each element produced by `g`.

    @tparam T Target data type
    @param  t      Type wrapper for the target data type
    @param  g      Data generator to use
    @param  others Index and size of the element when a sequence is filled, forwarded to `g`
    @return A value of type `T` produced by the generator `g`.
  **/
  //====================================================================================================================
  template<typename T> auto produce(type<T> const& t, auto g, auto... others);
}

namespace tts::_
{
  template<typename T> struct builtin_generation
  {
    static auto make(auto g, auto... others)
    {
      return g(tts::type<T> {}, others...);
    }
  };

  template<sequence T> struct builtin_generation<T>
  {
    static auto make(auto g, auto... args)
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
  };
}

namespace tts
{
  //====================================================================================================================
  /*!
    @ingroup tools-generators-custom
    @brief How a value of a given type is built from a generator

    Specialize this for your own types rather than overloading produce. A type without a
  specialization keeps the default way of building, which hands a scalar straight to the generator
  and fills a sequence element by element.

    The `produce` free function is the dispatcher rather than a customization point, and the call
  sites reach it qualified, so an overload of that name in another namespace is never found. It
  carries a generic default, which is why a leftover cannot be reported the way tts::precision and
  tts::comparison report theirs.

    @tparam T Target data type
  **/
  //====================================================================================================================
  template<typename T> struct generation
  {
  };

  template<typename T> auto produce(type<T> const&, auto g, auto... others)
  {
    if constexpr(requires { generation<T>::make(g, others...); })
    {
      return generation<T>::make(g, others...);
    }
    else
    {
      static_assert(
      !requires { generation<T>::make; },
      "[TTS] tts::generation<T>::make does not accept the generator.");

      return _::builtin_generation<T>::make(g, others...);
    }
  }

  //====================================================================================================================
  /// @}
  //====================================================================================================================

  //====================================================================================================================
  /**
    @brief Gives the base type of a type `T`

    The base type is `T` itself, and can be changed by a specialization of `base_type` for a
    user-defined type. tts::limits_set takes the limits and special values of `T` from its base
    type, and tts::random_bits, tts::as_integer, and tts::as_signed_integer take their integer width
    from it.

    @par Helper types
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
    @brief Gives the boolean type of a type `T`

    The boolean type is `bool`, and can be changed by a specialization of `boolean_type` for a
    user-defined type whose comparisons return another type, such as a mask of several booleans.
    The values of tts::logicals are of this type.

    @par Helper types
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

}

namespace tts::_
{
  template<typename T, bool Complete = false> struct bounded_range
  {
    T             lo;
    T             hi;
    std::uint64_t samples = 0;
  };

  struct whole_sweep
  {
    template<typename T> constexpr bounded_range<T, true> operator()(T lo, T hi) const
    {
      return {lo, hi};
    }
  };

  struct whole_sample
  {
    std::uint64_t samples;
  };
}

namespace tts
{
  //====================================================================================================================
  /**
    @defgroup tools-generators-class Data Generators
    @brief Generators of the values of the test cases and the range checks
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @brief Holds the limits and special values of a type `T`

    Each member is a value of `T` and an edge case for the test data. A specialization for a
    user-defined type can provide limits of its own.

    @groupheader{Examples}
    @snippet doc/limits.cpp snippet

    @tparam T Type whose limits are held
  **/
  //====================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
  template<typename T> struct limits_set
  {
    /// Largest representable value of the base type.
    T valmax;
    /// Lowest representable value of the base type, its most negative one.
    T valmin;
    /// \f$0\f$.
    T zero;
    /// \f$1\f$.
    T one;
    /// Quiet NaN, for floating-point base types.
    T nan;
    /// \f$+\infty\f$, for floating-point base types.
    T inf;
    /// \f$-\infty\f$, for floating-point base types.
    T minf;
    /// \f$-0\f$, for floating-point base types.
    T mzero;
    /// Largest \f$M\f$ with every integer of \f$[-M, M]\f$ exact, for floating-point base types.
    T maxflint;
    /// Smallest positive subnormal value, for floating-point base types.
    T mindenormal;
    /// Smallest positive normal value, for floating-point base types.
    T smallestposval;
    /// \f$-1\f$, for floating-point base types.
    T mone;
  };
#else
  template<typename T>
  struct limits_set
      : _::common_limits<T>
      , std::conditional_t<std::floating_point<base_type_t<T>>, _::float_limits<T>, _::empty_limits>
  {
    using type = T;
  };
#endif

  //====================================================================================================================
  /**
    @brief Returns the limits and special values of a type `T`

    @tparam T Type for which to provide limits
    @return The tts::limits_set of `T`.

    @relates limits_set
  **/
  //====================================================================================================================
  template<typename T> inline auto limits(tts::type<T>)
  {
    return limits_set<T> {};
  }

  //====================================================================================================================
  /**
    @brief Generates the same value at every call

    Each generated value is the one given at construction, converted to the target type through
    tts::conversion when it is specialized for that type.

    @tparam T Type of the value to be produced

    @groupheader{Example}
    @snippet doc/generator_value.cpp snippet
  **/
  //====================================================================================================================
  template<typename T> struct value
  {
    constexpr explicit value(T v)
        : seed(v)
    {
    }

    template<typename D> D operator()(tts::type<D>, auto...) const
    {
      return ::tts::convert_as(seed, type<D> {});
    }

    T seed;
  };

  //====================================================================================================================
  /**
    @brief Generates boolean values in a regular pattern

    The pattern depends on a starting index `start` and a period `range`, given in that order to the
    constructor. The element at index `i` of a sequence is:

      - `true` if `(start + i) % range == 0`,
      - `false` otherwise.

    A scalar target gets `false`.

    @tparam T Type of the starting index
    @tparam U Type of the range
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct logicals
  {
    constexpr logicals(T v, U k)
        : start(v)
        , range(k)
    {
    }

    template<typename D> auto operator()(tts::type<D>) const
    {
      return ::tts::convert_as(false, type<tts::boolean_type_t<D>> {});
    }

    template<typename D> auto operator()(tts::type<D>, auto idx, auto...) const
    {
      return ::tts::convert_as(((start + idx) % range) == 0, type<tts::boolean_type_t<D>> {});
    }

    T start;
    U range;
  };

  //====================================================================================================================
  /**
    @brief Generates a ramp of values

    The element at index `i` of a sequence is `start + i*step`, the step being 1 when none is given,
    which for a size `N` gives `start, start+step, ..., start+(N-1)*step`. A scalar target gets
    `start`.

    @tparam T Type of the initial value
    @tparam U Type of the step value

    @groupheader{Example}
    @snippet doc/generator_ramp.cpp snippet
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct ramp
  {
    constexpr explicit ramp(T s)
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
      return ::tts::convert_as(start + idx * step, type<D> {});
    }

    template<typename D> D operator()(tts::type<D>) const
    {
      return ::tts::convert_as(start, type<D> {});
    }

    T start;
    U step;
  };

  //====================================================================================================================
  /**
    @brief Generates a ramp of values read backward

    The element at index `i` of a sequence of size `N` is `start + (N-1-i)*step`, which gives
    `start+(N-1)*step, ..., start+step, start`. A scalar target gets `start`.

    @tparam T Type of the initial value
    @tparam U Type of the step value

    @groupheader{Example}
    @snippet doc/generator_reverse_ramp.cpp snippet
  **/
  //====================================================================================================================
  template<typename T, typename U = T> struct reverse_ramp
  {
    constexpr explicit reverse_ramp(T s)
        : start(s)
        , step(1)
    {
    }
    constexpr reverse_ramp(T s, U st)
        : start(s)
        , step(st)
    {
    }

    template<typename D, typename I> D operator()(tts::type<D>, I idx, auto sz, auto...) const
    {
      auto const rev = static_cast<I>(sz) - 1 - idx;
      return ::tts::convert_as(start + rev * step, type<D> {});
    }

    template<typename D> D operator()(tts::type<D>) const
    {
      return ::tts::convert_as(start, type<D> {});
    }

    T start;
    U step;
  };

  //====================================================================================================================
  /**
    @brief Generates values linearly spaced between two bounds

    The element at index `i` of a sequence of size `N` is `first + i*step`, where `step =
    (last-first)/(N-1)`, both computed in the target type, so rounding makes the gaps slightly
    unequal. If rounding carries a value past the last bound, the last bound is returned instead. A
    sequence of size 1 and a scalar target get the first bound.

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
      auto w1    = ::tts::convert_as(first_, type<D> {});
      auto w2    = ::tts::convert_as(last_, type<D> {});
      D    step  = (sz - 1) ? static_cast<D>(::tts::convert_as(last_ - first_, type<D> {}) /
                                             ::tts::convert_as(sz - 1, type<D> {}))
                            : ::tts::convert_as(0, type<D> {});
      auto value = ::tts::convert_as(
      w1 + ::tts::convert_as(idx, type<D> {}) * ::tts::convert_as(step, type<D> {}), type<D> {});

      // Clamp rounding overshoot back to the last bound, whichever direction the range runs in.
      return (w1 <= w2) ? _::min(value, w2) : _::max(value, w2);
    }

    template<typename D> D operator()(tts::type<D>) const
    {
      return ::tts::convert_as(first_, type<D> {});
    }

    T first_;
    U last_;
  };

  //====================================================================================================================
  /**
    @brief Generates random values between two bounds

    The constructor takes the lower bound, then the upper bound. Each value is drawn by
    tts::random_value, uniformly with both bounds included for an integral type, and log-uniformly
    for a floating-point type. In a build without `NDEBUG`, a negative bound for an unsigned type
    stops the run on an assertion.

    @tparam Mx Type of the upper bound
    @tparam Mn Type of the lower bound

    @groupheader{Example}
    @snippet doc/generator_randoms.cpp snippet
  **/
  //====================================================================================================================
  template<typename Mx, typename Mn> struct randoms
  {
    /// Marks tts::randoms and the classes derived from it for tts::is_randoms
    using random_type = void;

    constexpr randoms(Mn mn, Mx mx)
        : mini(mn)
        , maxi(mx)
    {
    }

    template<typename D> D operator()(tts::type<D> t, auto...)
    {
      check(t);
      return random_value(::tts::convert_as(mini, t), ::tts::convert_as(maxi, t));
    }

    /// Stops on a negative bound when `D` is an unsigned type
    template<typename D> void check(tts::type<D>) const
    {
      if constexpr(std::is_unsigned_v<D> && requires { mini >= 0; })
      {
        assert(mini >= 0 &&
               "Minimum value for unsigned type random generator must be non-negative");
      }
      if constexpr(std::is_unsigned_v<D> && requires { maxi >= 0; })
      {
        assert(maxi >= 0 &&
               "Maximum value for unsigned type random generator must be non-negative");
      }
    }

    Mn mini;
    Mx maxi;
  };

  //====================================================================================================================
  /*!
    @ingroup tools-generators-custom
    @brief Detects tts::randoms and the classes derived from it

    Derives from `std::true_type` when `G` is a tts::randoms or a class derived from it, and from
    `std::false_type` otherwise. A range check keeps the values of such a generator between
    `--valmin` and `--valmax`.

    @tparam G Generator type being examined
  **/
  //====================================================================================================================
  template<typename G> struct is_randoms : std::false_type
  {
  };
  template<typename G>
    requires requires { typename G::random_type; }
  struct is_randoms<G> : std::true_type
  {
  };

  //! @brief Value alias for tts::is_randoms, indifferent to references and cv-qualifiers
  template<typename G>
  inline constexpr bool is_randoms_v = is_randoms<std::remove_cvref_t<G>>::value;

  //====================================================================================================================
  /**
    @brief Generates random bit patterns

    Each value is an unsigned integer as wide as the base type of the target type, drawn uniformly
    over all its values, and `std::bit_cast` reads its bits as a target value. A sequence target
    gets one such integer per element.

    @groupheader{Example}
    @snippet doc/generator_random_bits.cpp snippet
  **/
  //====================================================================================================================
  struct random_bits
  {
    template<typename D> auto operator()(tts::type<D>, auto...)
    {
      using i_t = tts::_::sized_integer_t<tts::base_type_t<D>>;
      return tts::random_value<i_t>(0, std::numeric_limits<i_t>::max());
    }
  };

  //====================================================================================================================
  /**
    @brief Produces valid shift amounts for the target type

    Unlike @ref tts::random_bits, which draws a full-width bit pattern, this generator draws an
  index into the bits of the target type - a value in `[0, 8*sizeof(T)[`. That is what a shift
  operand has to be: shifting by the width of the type or more is undefined behaviour.
  **/
  //====================================================================================================================
  struct random_shift
  {
    template<typename D> auto operator()(tts::type<D>, auto...) const
    {
      using i_t = tts::_::sized_integer_t<tts::base_type_t<D>>;
      return tts::random_value<i_t>(0, static_cast<i_t>(8 * sizeof(i_t) - 1));
    }
  };

  //====================================================================================================================
  /**
    @brief Adapts a generator to produce unsigned integer values

    Calls the adapted generator, given at construction, for the unsigned integer type as wide as the
    base type of the target type, element by element for a sequence. The values are integers drawn
    by that generator, in place of values of the target type.

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
    @brief Adapts a generator to produce signed integer values

    Calls the adapted generator, given at construction, for the signed integer type as wide as the
    base type of the target type, element by element for a sequence. The values are integers drawn
    by that generator, in place of values of the target type.

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
  /**
    @brief Generates every value of a type, or of a range

    Produces every value of an argument of a @ref TTS_ULP_RANGE_CHECK, NaN excluded since it has no
    rank in tts::enumeration.

    - `tts::sweep` covers the whole type, from -inf to +inf for a floating-point type.
    - `tts::sweep(lo, hi)` covers every value of [`lo`, `hi`], its bounds given in either order.

    A sweep makes 65536 calls over two `std::uint8_t` and 4278190082 over a `float`. If the number
    of calls exceeds the largest `std::size_t`, as over two `double`, the check fails without
    running, and tts::sample can run such a check on `n` values per argument.

    @groupheader{Example}
    @snippet doc/generator_sweep.cpp snippet

    @see tts::sample
  **/
  //====================================================================================================================
  inline constexpr _::whole_sweep sweep = {};

  //====================================================================================================================
  /**
    @brief Generates a given number of values spread over a type or a range

    Produces `n` values of an argument of a @ref TTS_ULP_RANGE_CHECK, and never NaN.

    - `tts::sample(n)` spreads its values over the whole type.
    - `tts::sample(lo, hi, n)` spreads its values over [`lo`, `hi`], its bounds given in either
      order.

    The values are the two bounds and one value per slice of equal width in the ranks of
    tts::enumeration. The sample thus holds about the same number of values in each binade of a
    floating-point type, the subnormals of each sign counting as one binade. A range over many
    binades of small magnitude, such as [-700, 700] in `double`, has most of its values below 1 in
    magnitude. If `n` is 1, the sample holds the lower bound alone. A sample of at least as many
    values as its range holds gives each value once, as tts::sweep does.

    The position of a value within its slice comes from its index and from a value drawn from the
    generator of the run when the check starts, so each run checks other values, and `--seed`
    repeats those of a run.

    If `n` is 0, the check makes no call and records no check, so a case without another check is
    reported as invalid.

    @groupheader{Example}
    @snippet doc/generator_sample.cpp snippet
  **/
  //====================================================================================================================
  constexpr _::whole_sample sample(std::uint64_t n)
  {
    return {n};
  }

  //! @overload
  template<typename T> constexpr _::bounded_range<T> sample(T lo, T hi, std::uint64_t n)
  {
    return {lo, hi, n};
  }

  //====================================================================================================================
  /// @}
  //====================================================================================================================
}
