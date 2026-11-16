//======================================================================================================================
/// @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace tts
{
  namespace _
  {
    struct primary_adapter
    {
    };
  }

  //====================================================================================================================
  /*!
    @ingroup tools-precision
    @brief Make a type usable in a range check

    A range check stores its data as scalars, and a specialization maps a type to them:
    `element_type` is the scalar, `size` the number of scalars per value, `load` and `store` the
    conversions between a value and `size` consecutive scalars. The primary template covers a
    scalar type, with `size` equal to one. A specialization also keeps whole a result that follows
    the protocol of structured bindings, which the primary template splits into its members.

    @groupheader{Example}
    @snippet doc/range_adapter.cpp snippet

    @tparam T Type to adapt
  **/
  //====================================================================================================================
  template<typename T> struct adapter : _::primary_adapter
  {
    using element_type                = T;
    static constexpr std::size_t size = 1;

    static T                     load(T const* src) noexcept
    {
      return *src;
    }
    static void store(T const& v, T* dst) noexcept
    {
      *dst = v;
    }
  };

  namespace _
  {
    // A tuple, <tuple> being outside the include budget of TTS. Can't use kumi for dependency issues.
    template<std::size_t I, typename T> struct leaf
    {
      TTS_NO_UNIQUE_ADDRESS T value;
    };

    template<typename Seq, typename... Ts> struct pack_base;

    template<std::size_t... Is, typename... Ts>
    struct pack_base<std::index_sequence<Is...>, Ts...> : leaf<Is, Ts>...
    {
      explicit pack_base(Ts... ts)
          : leaf<Is, Ts> {static_cast<Ts&&>(ts)}...
      {
      }
    };

    template<typename... Ts> struct pack : pack_base<std::index_sequence_for<Ts...>, Ts...>
    {
      using pack_base<std::index_sequence_for<Ts...>, Ts...>::pack_base;
    };

    template<typename... Ts> pack(Ts...) -> pack<Ts...>;

    template<std::size_t I, typename T> constexpr T& get(leaf<I, T>& l) noexcept
    {
      return l.value;
    }

    template<std::size_t I, typename T> constexpr T const& get(leaf<I, T> const& l) noexcept
    {
      return l.value;
    }

    // A result following the protocol of structured bindings is compared member by member, unless
    // an adapter describes it as a whole.
    template<typename T>
    concept tuple_result =
    std::is_base_of_v<primary_adapter, adapter<T>> && requires { std::tuple_size<T>::value; };

    template<typename T> struct result_members
    {
      using type                         = types<T>;
      static constexpr std::size_t count = 1;
      static constexpr std::size_t size  = adapter<T>::size;
    };

    template<tuple_result T> struct result_members<T>
    {
      template<std::size_t... Is>
      static types<std::decay_t<std::tuple_element_t<Is, T>>...> list(std::index_sequence<Is...>);

      using type = decltype(list(std::make_index_sequence<std::tuple_size<T>::value> {}));
      static constexpr std::size_t count = std::tuple_size<T>::value;
      static constexpr std::size_t size  = adapter<std::decay_t<std::tuple_element_t<0, T>>>::size;
    };

    template<std::size_t I, typename T> decltype(auto) result_member(T const& r)
    {
      if constexpr(tuple_result<T>) return get<I>(r);
      else return (r);
    }

    template<std::size_t Size, typename... Ms, typename... Rs>
    constexpr bool members_match(types<Ms...>, types<Rs...>)
    {
      if constexpr(sizeof...(Ms) != sizeof...(Rs)) return false;
      else
        return (
        (adapter<Ms>::size == Size && std::is_same_v<typename adapter<Ms>::element_type, Rs>) &&
        ...);
    }

    inline std::size_t power(std::size_t r, std::size_t n) noexcept
    {
      std::size_t p = 1;
      for(std::size_t k = 0; k < n; ++k)
        p *= r;
      return p;
    }

    inline std::size_t last_bucket_less(std::size_t nb_buckets, double ulp) noexcept
    {
      if(ulp <= 1.5) return static_cast<std::size_t>(_::ceil(ulp * 2));
      if(_::is_inf(ulp)) return nb_buckets - 1;
      auto steps =
      static_cast<std::size_t>(std::bit_width(static_cast<std::size_t>(_::ceil(ulp)) - 1));
      return _::min(nb_buckets - 2, steps + 4);
    }

    // A group of group_size values, the one at mark between stars.
    template<typename T> void show_group(T const* p, std::size_t group_size, std::size_t mark)
    {
      if(group_size == 1) ::tts::output().write(::tts::as_text(*p));
      else
      {
        ::tts::output().write("{");
        for(std::size_t k = 0; k < group_size; ++k)
          ::tts::output().write(k == mark ? " *%s*" : " %s", ::tts::as_text(p[ k ]).data());
        ::tts::output().write(" }");
      }
    }

    template<std::size_t... Ks, typename... As>
    void show_inputs(pack_base<std::index_sequence<Ks...>, buffer<As>...> const& inputs,
                     std::size_t                                                 i,
                     std::size_t                                                 group_size)
    {
      char const* lead = "";
      (
      (::tts::output().write(lead),
       show_group(get<Ks>(inputs).data() + i - i % group_size, group_size, i % group_size),
       lead = ", "),
      ...);
    }

    template<typename... Ts> void show_call(char const* name, types<Ts...>)
    {
      char const* lead = "";
      ::tts::output().write("%s(", name);
      (
      (::tts::output().write(lead),
       ::tts::output().write(::tts::as_text(typename_<Ts>)),
       lead = ", "),
      ...);
      ::tts::output().write(")");
    }

    template<std::size_t... Ks, typename... Gs>
    void show_generators(pack_base<std::index_sequence<Ks...>, Gs...> const& generators,
                         char const*                                         spelling)
    {
      char const* lead = "";
      if constexpr((described<Gs> && ...))
        (
        (::tts::output().write(lead),
         ::tts::output().write(::tts::as_text(get<Ks>(generators))),
         lead = ", "),
        ...);
      else ::tts::output().write(spelling);
      ::tts::output().writeln();
    }

    // The values of one member of the result, and their histogram.
    template<typename T> struct member_scan
    {
      static constexpr std::size_t nb_buckets = 2 + 1 + 16;

      explicit member_scan(std::size_t count)
          : expected(count)
          , found(count)
      {
      }

      void tally()
      {
        for(std::size_t i = 0; i < expected.size(); ++i)
        {
          auto ulp = ::tts::ulp_check(expected[ i ], found[ i ]);
          auto b   = last_bucket_less(nb_buckets, ulp);
          max_ulp  = _::max(max_ulp, ulp);
          if(counts[ b ] == 0) first[ b ] = i;
          counts[ b ]++;
        }
        nb_ulps += expected.size();
      }

      template<typename Inputs> void print(Inputs const& inputs, std::size_t group_size) const
      {
        ::tts::output().writeln(
        "%-16s%-16s%-16s%-16s", "Max ULP", "Count (#)", "Ratio Sum (%)", "Samples");
        _::separator();

        double ratio = 0.;
        for(std::size_t b = 0; b < nb_buckets; ++b)
        {
          if(counts[ b ] == 0) continue;

          double ulps  = 0;
          ratio       += (100. * counts[ b ]) / static_cast<double>(nb_ulps);

          if(b <= 3) ulps = static_cast<double>(b) / 2.0;
          else if(b == nb_buckets - 1) ulps = std::numeric_limits<double>::infinity();
          else ulps = 1 << (b - 4);

          std::size_t i    = first[ b ];
          std::size_t mark = i % group_size;

          ::tts::output().write("%-16.1f%-16u%-16gInput:      ", ulps, counts[ b ], ratio);
          show_inputs(inputs, i, group_size);
          ::tts::output().writeln();
          ::tts::output().write("%48sFound:      ", "");
          show_group(found.data() + i - mark, group_size, mark);
          ::tts::output().writeln();
          ::tts::output().write("%48sinstead of: ", "");
          show_group(expected.data() + i - mark, group_size, mark);
          ::tts::output().writeln();
          _::separator();
        }
      }

      buffer<T>            expected;
      buffer<T>            found;
      buffer<unsigned int> counts  = buffer<unsigned int>(nb_buckets, 0u);
      buffer<std::size_t>  first   = buffer<std::size_t>(nb_buckets, 0u);
      std::size_t          nb_ulps = 0;
      double               max_ulp = 0.;
    };

    template<typename... Rs> pack<member_scan<Rs>...> make_scans(types<Rs...>, std::size_t count)
    {
      return pack<member_scan<Rs>...> {member_scan<Rs>(count)...};
    }

    // The index comes as a std::integral_constant, usable as a template argument.
    template<std::size_t... Js, typename... Ms, typename F>
    void for_each_member(pack_base<std::index_sequence<Js...>, Ms...>& results, F f)
    {
      (f(get<Js>(results), std::integral_constant<std::size_t, Js> {}), ...);
    }

    // The data covers every combination of side values per argument.
    template<std::size_t... Ks, typename... As, typename... Gs>
    void fill_inputs(pack_base<std::index_sequence<Ks...>, buffer<As>...>& inputs,
                     pack<Gs...> const&                                    generators,
                     std::size_t                                           side)
    {
      constexpr std::size_t arity = sizeof...(As);
      for(std::size_t i = 0; i < get<0>(inputs).size(); ++i)
      {
        (
        (get<Ks>(inputs)[ i ] =
         ::tts::produce(type<As> {},
                        get<(sizeof...(Gs) == 1 ? 0 : Ks)>(generators),
                        static_cast<std::ptrdiff_t>(i / power(side, arity - 1 - Ks) % side),
                        static_cast<std::ptrdiff_t>(side))),
        ...);
      }
    }

    template<typename F, std::size_t... Ks, typename... As>
    auto call_reference(F&                                                          f,
                        pack_base<std::index_sequence<Ks...>, buffer<As>...> const& inputs,
                        std::size_t                                                 i)
    {
      return f(get<Ks>(inputs)[ i ]...);
    }

    template<typename... Ws, typename F, std::size_t... Ks, typename... As>
    auto call_challenger(types<Ws...>,
                         F&                                                          f,
                         pack_base<std::index_sequence<Ks...>, buffer<As>...> const& inputs,
                         std::size_t                                                 i)
    {
      return f(adapter<Ws>::load(get<Ks>(inputs).data() + i)...);
    }

    // An adapter written for TTS 3, through run and retrieve.
    template<typename T>
    concept legacy_adapter = requires { &adapter<T>::retrieve; };

    template<typename... As,
             typename... Ws,
             typename... Gs,
             typename RefFun,
             typename NewFun,
             typename... Us>
    void check_ulp_range(types<As...>, // NOSONAR - six macro arguments, call site, generator text
                         types<Ws...>,
                         pack<Gs...> const& generators,
                         RefFun             reference,
                         NewFun             challenger,
                         pack<Us...> const& bounds,
                         check_site const&  site,
                         char const*        spelling)
    {
      static_assert(sizeof...(As) == sizeof...(Ws),
                    "[TTS] TTS_ULP_RANGE_CHECK needs one challenger type per reference type.");
      static_assert((!legacy_adapter<Ws> && ...),
                    "[TTS] tts::adapter<T>::run and retrieve are no longer the protocol. "
                    "Define element_type, size, load, and store instead.");
      static_assert(sizeof...(Gs) == 1 || sizeof...(Gs) == sizeof...(As),
                    "[TTS] TTS_ULP_RANGE_CHECK needs one generator per argument, or one for all.");

      using ref_result                 = std::decay_t<std::invoke_result_t<RefFun, As...>>;
      using new_result                 = std::decay_t<std::invoke_result_t<NewFun, Ws...>>;
      using ref_members                = typename result_members<ref_result>::type;

      constexpr std::size_t arity      = sizeof...(As);
      constexpr std::size_t group_size = result_members<new_result>::size;
      constexpr std::size_t members    = result_members<ref_result>::count;

      static_assert(((adapter<Ws>::size == group_size) && ...),
                    "[TTS] TTS_ULP_RANGE_CHECK needs every challenger type and its result to "
                    "hold the same number of elements.");
      static_assert((std::is_same_v<typename adapter<Ws>::element_type, As> && ...),
                    "[TTS] TTS_ULP_RANGE_CHECK needs every challenger type to hold elements of "
                    "its reference type.");
      static_assert(
      members_match<group_size>(typename result_members<new_result>::type {}, ref_members {}),
      "[TTS] TTS_ULP_RANGE_CHECK needs the challenger result to hold elements of the "
      "reference result type, member by member for a tuple.");
      static_assert(sizeof...(Us) == 1 || sizeof...(Us) == members,
                    "[TTS] TTS_ULP_RANGE_CHECK needs one bound per member of the result, or one "
                    "for all.");

      std::size_t block = ::tts::arguments().value(std::size_t {4096}, "--block");
      std::size_t side  = 1;
      while(power(side, arity) < block)
        ++side;
      std::size_t         count = (power(side, arity) + group_size - 1) / group_size * group_size;

      pack<buffer<As>...> inputs {buffer<As>(count)...};
      fill_inputs(inputs, generators, side);

      auto        results    = make_scans(ref_members {}, count);
      std::size_t repetition = ::tts::arguments().value(std::size_t {1}, "--loop");
      for(std::size_t r = 0; r < repetition; ++r)
      {
        for(std::size_t i = 0; i < count; ++i)
        {
          auto res = call_reference(reference, inputs, i);
          for_each_member(
          results, [ &res, i ](auto& m, auto j) { m.expected[ i ] = result_member<j>(res); });
        }

        for(std::size_t i = 0; i < count; i += group_size)
        {
          auto res = call_challenger(types<Ws...> {}, challenger, inputs, i);
          for_each_member(results,
                          [ &res, i ](auto& m, auto j)
                          {
                            auto const& v = result_member<j>(res);
                            adapter<std::decay_t<decltype(v)>>::store(v, m.found.data() + i);
                          });
        }

        for_each_member(results, [](auto& m, auto) { m.tally(); });
      }

      std::size_t failed  = members;
      double      limit   = 0.;
      double      found   = 0.;
      double      largest = 0.;
      for_each_member(results,
                      [ &bounds, &failed, &limit, &found, &largest ](auto const& m, auto j)
                      {
                        double l = ::tts::arguments().value(
                        static_cast<double>(get<sizeof...(Us) == 1 ? 0 : j()>(bounds)), "--ulpmax");
                        largest = _::max(largest, m.max_ulp);
                        if(failed == members && !(m.max_ulp <= l))
                        {
                          failed = j;
                          limit  = l;
                          found  = m.max_ulp;
                        }
                      });

      if(::tts::is_detailed() || (failed != members && !::tts::is_quiet()))
      {
        ::tts::output().write("Comparing: ");
        show_call(site.lhs, types<As...> {});
        ::tts::output().write(" with ");
        show_call(site.rhs, types<Ws...> {});
        ::tts::output().write(" using ");
        show_generators(generators, spelling);

        for_each_member(results,
                        [ &inputs ](auto const& m, auto j)
                        {
                          if constexpr(tuple_result<ref_result>)
                            ::tts::output().writeln("Member %d of the result",
                                                    static_cast<int>(j()));
                          m.print(inputs, group_size);
                        });
      }

      if(failed == members)
      {
        if(check_passed())
          check_report(site,
                       ::tts::text {"Expression: %s similar to %s, at most %s ULP apart",
                                    site.rhs,
                                    site.lhs,
                                    ::tts::as_text(largest).data()});
      }
      else
      {
        ::tts::text member;
        if constexpr(tuple_result<ref_result>)
          member = ::tts::text {" on member %d of the result", static_cast<int>(failed)};

        check_failed(
        site,
        ::tts::text {"Expecting: %s similar to %s within %s ULP%s but found: %s ULP instead",
                     site.rhs,
                     site.lhs,
                     ::tts::as_text(limit).data(),
                     member.data(),
                     ::tts::as_text(found).data()});
      }
    }
  }
}

//======================================================================================================================
/*!
  @def TTS_ULP_RANGE_CHECK
  @ingroup  test-precision
  @brief Checks a function against a reference over a data set, within a given ULP distance

  Evaluates `NewFunc` and `RefFunc` on the same data set and sorts the [ULP
  distance](rationale.html#ulp) between their results into a histogram. The check passes when the
  largest distance stays within the bound given after `NewFunc`. The data set holds at least
  `--block` values, 4096 by default, and the generator builds each value from its index.
  `--ulpmax` replaces the bounds for every check of the run.

  A function of several arguments takes tts::types lists as `RefType` and `NewType`, with the types
  in call order. Its generators are given between parentheses, as many as arguments, or as a single
  generator shared by all arguments. Each generator then produces the smallest number of values
  whose product over the arguments reaches `--block`: 64 values for two arguments by default. The
  data set covers every combination of these values.

  `NewType` may hold several values of `RefType`, as a specialization of tts::adapter describes.

  `std::pair`, `std::tuple`, and any result type that specializes `std::tuple_size` and provides a
  `get` found by argument-dependent lookup are tuples, unless tts::adapter is specialized for them.
  The members of a tuple are compared separately, each with its own histogram, and the check takes
  a bound for all members or a bound per member.

  A check that fails prints its histogram, and every check prints it under `--verbose`. Each bucket
  shows the first element whose distance falls in it, with the results of the last pass under
  `--loop`. When `NewType` holds several values, that element is marked between stars within its
  group.

  @param Producer Data generator, or a generator per argument between parentheses.
  @param RefType  Argument type of the reference function, or a tts::types list of them.
  @param NewType  Argument type of the challenger function, or a tts::types list of them.
  @param RefFunc  Reference function.
  @param NewFunc  Function compared to the reference.
  @param ...      Largest ULP distance accepted, or one per member of a tuple result.

  @groupheader{Example}
  @snippet doc/ulp_ranges.cpp snippet

  @see tts::adapter
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ULP_RANGE_CHECK(Producer, RefType, NewType, RefFunc, NewFunc, ...)
#else
#define TTS_ULP_RANGE_CHECK(Producer, RefType, NewType, RefFunc, NewFunc, ...)                     \
  ::tts::_::check_ulp_range(                                                                       \
  ::tts::as_type_list_t<TTS_REMOVE_PARENS(RefType)> {},                                            \
  ::tts::as_type_list_t<TTS_REMOVE_PARENS(NewType)> {},                                            \
  ::tts::_::pack {TTS_REMOVE_PARENS(Producer)},                                                    \
  RefFunc,                                                                                         \
  NewFunc,                                                                                         \
  ::tts::_::pack {__VA_ARGS__},                                                                    \
  TTS_CHECK_SITE(TTS_FAIL, TTS_STRING(RefFunc), "", TTS_STRING(NewFunc), ""),                      \
  TTS_STRING(TTS_REMOVE_PARENS(Producer))) /**/
#endif

namespace tts
{
  //====================================================================================================================
  /*!
    @brief Data generator using a realistic random distribution

    @groupheader{Example}
    @code
    #define TTS_MAIN  // No need for main()
    #include <tts/tts.hpp>

    float ok_x (float x) { return x; }
    float bad_x(float x) { return x + x*1e-7f; }

    TTS_CASE( "Test range check" )
    {
      TTS_ULP_RANGE_CHECK ( tts::realistic_generator<float>(-100.f,+100.f)
                          , float, float , ok_x, bad_x
                          , 2.
                          );
    };
    @endcode

    @see realistic_distribution
    @tparam T Type of data to generate
  **/
  //====================================================================================================================
  template<typename T> struct realistic_generator
  {
    realistic_generator(T mn, T mx)
        : mini(mn)
        , maxi(mx)
    {
    }
    T operator()(auto, auto, auto) const
    {
      return ::tts::random_value(mini, maxi);
    }

  private:
    // The rendering used to be a hidden friend reading these directly. It is a tts::display
    // specialization now, and needs the same access.
    template<typename> friend struct display;

    T mini, maxi;
  };

  template<typename T> struct display<realistic_generator<T>>
  {
    static text render(realistic_generator<T> const& s)
    {
      return text {"realistic_generator<%s>(%s,%s)",
                   as_text(typename_<T>).data(),
                   as_text(s.mini).data(),
                   as_text(s.maxi).data()};
    }
  };
}
