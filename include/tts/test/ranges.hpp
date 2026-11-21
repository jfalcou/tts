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

    // Histogram of one member of the result, with the first element of each bucket.
    template<typename T, typename... As> struct member_scan
    {
      static constexpr std::size_t nb_buckets = 2 + 1 + 16;

      explicit member_scan(std::size_t group_size)
          : expected(group_size)
          , found(group_size)
          , inputs {buffer<As>(nb_buckets * group_size)...}
          , first_expected(nb_buckets * group_size)
          , first_found(nb_buckets * group_size)
      {
      }

      template<std::size_t... Ks>
      void tally(pack_base<std::index_sequence<Ks...>, buffer<As>...> const& group)
      {
        std::size_t group_size = expected.size();
        for(std::size_t e = 0; e < group_size; ++e)
        {
          auto ulp = ::tts::ulp_check(expected[ e ], found[ e ]);
          auto b   = last_bucket_less(nb_buckets, ulp);
          max_ulp  = _::max(max_ulp, ulp);
          if(counts[ b ]++ == 0)
          {
            marks[ b ] = e;
            for(std::size_t k = 0; k < group_size; ++k)
            {
              first_expected[ b * group_size + k ] = expected[ k ];
              first_found[ b * group_size + k ]    = found[ k ];
              ((get<Ks>(inputs)[ b * group_size + k ] = get<Ks>(group)[ k ]), ...);
            }
          }
        }
        nb_ulps += group_size;
      }

      // Adds the histogram of the next slice of the calls, whose samples come after this one's.
      void merge(member_scan const& next)
      {
        for(std::size_t b = 0; b < nb_buckets; ++b)
        {
          if(counts[ b ] == 0 && next.counts[ b ] != 0)
            take_sample(b, next, std::index_sequence_for<As...> {});
          counts[ b ] += next.counts[ b ];
        }
        nb_ulps += next.nb_ulps;
        max_ulp  = _::max(max_ulp, next.max_ulp);
      }

      template<std::size_t... Ks>
      void take_sample(std::size_t b, member_scan const& next, std::index_sequence<Ks...>)
      {
        std::size_t group_size = expected.size();
        marks[ b ]             = next.marks[ b ];
        for(std::size_t k = b * group_size; k < (b + 1) * group_size; ++k)
        {
          first_expected[ k ] = next.first_expected[ k ];
          first_found[ k ]    = next.first_found[ k ];
          ((get<Ks>(inputs)[ k ] = get<Ks>(next.inputs)[ k ]), ...);
        }
      }

      void print() const
      {
        ::tts::output().writeln(
        "%-16s%-16s%-16s%-16s", "Max ULP", "Count (#)", "Ratio Sum (%)", "Samples");
        _::separator();

        std::size_t group_size = expected.size();
        double      ratio      = 0.;
        for(std::size_t b = 0; b < nb_buckets; ++b)
        {
          if(counts[ b ] == 0) continue;

          double ulps  = 0;
          ratio       += (100. * static_cast<double>(counts[ b ])) / static_cast<double>(nb_ulps);

          if(b <= 3) ulps = static_cast<double>(b) / 2.0;
          else if(b == nb_buckets - 1) ulps = std::numeric_limits<double>::infinity();
          else ulps = 1 << (b - 4);

          ::tts::output().write("%-16.1f%-16zu%-16gInput:      ", ulps, counts[ b ], ratio);
          show_inputs(inputs, b * group_size + marks[ b ], group_size);
          ::tts::output().writeln();
          ::tts::output().write("%48sFound:      ", "");
          show_group(first_found.data() + b * group_size, group_size, marks[ b ]);
          ::tts::output().writeln();
          ::tts::output().write("%48sinstead of: ", "");
          show_group(first_expected.data() + b * group_size, group_size, marks[ b ]);
          ::tts::output().writeln();
          _::separator();
        }
      }

      buffer<T>           expected;
      buffer<T>           found;
      pack<buffer<As>...> inputs;
      buffer<T>           first_expected;
      buffer<T>           first_found;
      buffer<std::size_t> counts  = buffer<std::size_t>(nb_buckets, std::size_t {0});
      buffer<std::size_t> marks   = buffer<std::size_t>(nb_buckets, std::size_t {0});
      std::size_t         nb_ulps = 0;
      double              max_ulp = 0.;
    };

    template<typename... Rs, typename... As>
    pack<member_scan<Rs, As...>...> make_scans(types<Rs...>, types<As...>, std::size_t group_size)
    {
      return pack<member_scan<Rs, As...>...> {member_scan<Rs, As...>(group_size)...};
    }

    // The index comes as a std::integral_constant, usable as a template argument.
    template<std::size_t... Js, typename... Ms, typename F>
    void for_each_member(pack_base<std::index_sequence<Js...>, Ms...>& results, F f)
    {
      (f(get<Js>(results), std::integral_constant<std::size_t, Js> {}), ...);
    }

    // A generator over tts::enumeration<T>, such as tts::sweep.
    template<typename G, typename T>
    concept enumerated = requires(G const& g) { g.span(type<T> {}); };

    template<std::size_t K, typename... Gs> auto const& generator_of(pack<Gs...> const& generators)
    {
      return get<(sizeof...(Gs) == 1 ? 0 : K)>(generators);
    }

    // Whether every argument comes from a generator over tts::enumeration, given its pack.
    template<typename P, typename... As, std::size_t... Ks>
    constexpr bool all_enumerated(std::index_sequence<Ks...>)
    {
      return (
      enumerated<std::remove_cvref_t<decltype(generator_of<Ks>(std::declval<P const&>()))>, As> &&
      ...);
    }

    template<typename G> inline constexpr bool is_bounded_sweep                   = false;
    template<typename T> inline constexpr bool is_bounded_sweep<bounded_sweep<T>> = true;

    // Read --valmin and --valmax once per check, never in a generator, which runs once per value.
    struct option_bounds
    {
      template<typename T> static T low(T v)
      {
        return ::tts::arguments().value(v, "--valmin");
      }

      template<typename T> static T high(T v)
      {
        return ::tts::arguments().value(v, "--valmax");
      }

      template<typename A, typename G> static auto apply(type<A> t, G const& g)
      {
        if constexpr(std::is_same_v<G, whole_sweep>)
        {
          using e_t = enumeration<A>;
          return bounded_sweep<A> {low(e_t::value(0)), high(e_t::value(e_t::last)), g.scope};
        }
        else if constexpr(is_bounded_sweep<G>)
          return bounded_sweep<A> {
          low(::tts::convert_as(g.lo, t)), high(::tts::convert_as(g.hi, t)), g.scope};
        else if constexpr(is_randoms_v<G>)
          return randoms<A, A> {low(::tts::convert_as(g.mini, t)),
                                high(::tts::convert_as(g.maxi, t))};
        else return g;
      }
    };

    template<typename... As, std::size_t... Ks, typename... Gs>
    auto with_option_bounds(types<As...>, std::index_sequence<Ks...>, pack<Gs...> const& generators)
    {
      return pack {option_bounds::apply(type<As> {}, generator_of<Ks>(generators))...};
    }

    // Returns the number of calls. A side of 0 marks an argument drawn rather than on a grid.
    template<typename... As, std::size_t... Ks, typename... Gs>
    std::size_t layout_of(types<As...>,
                          std::index_sequence<Ks...>,
                          pack<Gs...> const& generators,
                          std::size_t*       sides)
    {
      constexpr std::size_t arity = sizeof...(As);
      if constexpr(all_enumerated<pack<Gs...>, As...>(std::index_sequence<Ks...> {}))
      {
        // The largest scope given to a tts::sweep of the check, which --scope still replaces.
        std::size_t wanted = 0;
        ((wanted = _::max(wanted, generator_of<Ks>(generators).scope)), ...);
        std::size_t scope =
        ::tts::arguments().value(wanted ? wanted : std::size_t {1} << 24, "--scope");
        buffer<std::uint64_t> spans {generator_of<Ks>(generators).span(type<As> {})...};
        std::size_t           calls = 1;
        bool                  fits  = true;
        for(std::size_t k = 0; k < arity; ++k)
        {
          // Do not multiply by spans[k] + 1: it overflows for a 64-bit type.
          fits = spans[ k ] < scope && calls <= scope / (spans[ k ] + 1);
          if(!fits) break;
          sides[ k ]  = static_cast<std::size_t>(spans[ k ] + 1);
          calls      *= sides[ k ];
        }
        if(fits) return calls;

        for(std::size_t k = 0; k < arity; ++k)
          sides[ k ] = 0;
        return scope;
      }
      else
      {
        std::size_t block = ::tts::arguments().value(std::size_t {4096}, "--block");
        std::size_t side  = 1;
        while(power(side, arity) < block)
          ++side;
        for(std::size_t k = 0; k < arity; ++k)
          sides[ k ] = side;
        return power(side, arity);
      }
    }

    // Shuffles [0, n) with SplitMix64 steps on the bits of n, repeated until the result is below n.
    inline std::uint64_t permute(std::uint64_t i, std::uint64_t n, std::uint64_t seed) noexcept
    {
      auto          bits  = static_cast<unsigned>(std::bit_width(n - 1));
      std::uint64_t mask  = bits == 64 ? ~std::uint64_t {0} : (std::uint64_t {1} << bits) - 1;
      unsigned      shift = bits / 2 + 1;
      std::uint64_t key   = mix(seed);

      do
      {
        i  = (i + key) & mask;
        i ^= i >> shift;
        i  = (i * 0xbf58476d1ce4e5b9ULL) & mask;
        i ^= i >> shift;
        i  = (i * 0x94d049bb133111ebULL) & mask;
        i ^= i >> shift;
      } while(i >= n);

      return i;
    }

    template<std::size_t... Ks, typename... As, typename... Gs>
    void fill_group(pack_base<std::index_sequence<Ks...>, buffer<As>...>& group,
                    pack<Gs...> const&                                    generators,
                    std::size_t const*                                    sides,
                    std::size_t                                           first,
                    std::size_t                                           count)
    {
      for(std::size_t e = 0; e < get<0>(group).size(); ++e)
      {
        std::size_t i = first + e;
        std::size_t idx[ sizeof...(As) ]; // NOSONAR - <array> costs compile time
        for(std::size_t k = sizeof...(As); k-- > 0;)
        {
          if(sides[ k ] == 0)
            idx[ k ] = k == 0 ? i : static_cast<std::size_t>(permute(i, count, k));
          else
          {
            idx[ k ]  = i % sides[ k ];
            i        /= sides[ k ];
          }
        }

        (
        (get<Ks>(group)[ e ] =
         ::tts::produce(type<As> {},
                        generator_of<Ks>(generators),
                        static_cast<std::ptrdiff_t>(idx[ Ks ]),
                        static_cast<std::ptrdiff_t>(sides[ Ks ] ? sides[ Ks ] : count))),
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
                         pack_base<std::index_sequence<Ks...>, buffer<As>...> const& inputs)
    {
      return f(adapter<Ws>::load(get<Ks>(inputs).data())...);
    }

    // Adds the distances between the results of the reference and the challenger on one group.
    template<typename... Ws, typename S, typename G, typename RefFun, typename NewFun>
    void scan_group(types<Ws...>, S& results, G const& group, RefFun& reference, NewFun& challenger)
    {
      for(std::size_t e = 0; e < get<0>(group).size(); ++e)
      {
        auto res = call_reference(reference, group, e);
        for_each_member(results,
                        [ &res, e ](auto& m, auto j) { m.expected[ e ] = result_member<j>(res); });
      }

      auto res = call_challenger(types<Ws...> {}, challenger, group);
      for_each_member(results,
                      [ &res ](auto& m, auto j)
                      {
                        auto const& v = result_member<j>(res);
                        adapter<std::decay_t<decltype(v)>>::store(v, m.found.data());
                      });

      for_each_member(results, [ &group ](auto& m, auto) { m.tally(group); });
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

      auto bounded =
      with_option_bounds(types<As...> {}, std::index_sequence_for<As...> {}, generators);
      std::size_t sides[ sizeof...(As) ]; // NOSONAR - <array> costs compile time
      std::size_t calls =
      layout_of(types<As...> {}, std::index_sequence_for<As...> {}, bounded, sides);
      std::size_t count      = (calls + group_size - 1) / group_size * group_size;

      std::size_t repetition = ::tts::arguments().value(std::size_t {1}, "--loop");
      std::size_t groups     = count / group_size;

      using scans            = decltype(make_scans(ref_members {}, types<As...> {}, group_size));
      auto scan              = [ &reference,
                                 &challenger,
                                 &bounded,
                                 sizes = static_cast<std::size_t const*>(sides),
                                 count,
                                 repetition ](std::size_t first, std::size_t last)
      {
        // Do not scan into partial[u]: the histograms of neighboring slices share cache lines.
        scans               results = make_scans(ref_members {}, types<As...> {}, group_size);
        pack<buffer<As>...> group {buffer<As>(group_size)...};

        for(std::size_t i = first; i < last; i += group_size)
        {
          fill_group(group, bounded, sizes, i, count);
          for(std::size_t r = 0; r < repetition; ++r)
            scan_group(types<Ws...> {}, results, group, reference, challenger);
        }

        return results;
      };

#if defined(_OPENMP)
      // Only a sweep builds each value from its index alone. Any other generator, a random one above
      // all, gives values that depend on the thread calling it, so its check keeps one thread.
      constexpr bool swept =
      all_enumerated<decltype(bounded), As...>(std::index_sequence_for<As...> {});
      std::size_t cores =
      ::tts::arguments().value(static_cast<std::size_t>(omp_get_max_threads()), "--cores");
      int threads = swept && cores > 1 ? static_cast<int>(cores) : 1;

      // Several slices per thread even out the slices whose values cost more.
      std::size_t slices = threads == 1 ? 1 : _::min(groups, std::size_t {16} * cores);
#else
      std::size_t slices = 1;
#endif

      // Each slice keeps its own histograms, merged in the order of the calls, so that the report
      // does not depend on the number of threads.
      buffer<scans> partial(slices, make_scans(ref_members {}, types<As...> {}, group_size));
#if defined(_OPENMP)
#pragma omp parallel for schedule(dynamic) num_threads(threads)
#endif
      for(int s = 0; s < static_cast<int>(slices); ++s)
      {
        auto u = static_cast<std::size_t>(s);
        partial[ u ] =
        scan((groups / slices * u + _::min(u, groups % slices)) * group_size,
             (groups / slices * (u + 1) + _::min(u + 1, groups % slices)) * group_size);
      }

      scans& results = partial[ 0 ];
      for(std::size_t s = 1; s < slices; ++s)
        for_each_member(results,
                        [ &next = partial[ s ] ](auto& m, auto j) { m.merge(get<j()>(next)); });

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
                        [](auto const& m, auto j)
                        {
                          if constexpr(tuple_result<ref_result>)
                            ::tts::output().writeln("Member %d of the result",
                                                    static_cast<int>(j()));
                          m.print();
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
  @brief Checks a function against a reference over a range of inputs, within a ULP distance.

  Generates inputs with `Producer`, calls `RefFunc` and `NewFunc` on each of them, and records the
  ULP distance between the two results. The distances are then gathered in a histogram, and the
  check passes if none exceeds the bound given after `NewFunc`. See the
  [ULP rationale](rationale.html#ulp) for how the distance is measured.

  - For a function of one argument, `RefType` and `NewType` are its type, and `Producer` builds
    `--block` inputs, 4096 by default.
  - For a function of several arguments, `RefType` and `NewType` are tts::types lists between
    parentheses, in call order. `Producer` is one generator per argument between parentheses, or a
    single generator for all. Each generator builds the smallest number of values whose
    combinations reach `--block`, 64 for two arguments by default, and the check runs every
    combination.

  When every generator is tts::sweep, the scope of the check replaces `--block`, as tts::sweep
  describes. Memory does not grow with the number of calls, so `--scope=4294967296` checks each
  float but NaN once.

  `--valmin` and `--valmax` replace the lowest and the highest value of tts::sweep and
  tts::randoms, for every argument of every check of the run.

  Compiled with OpenMP, a check whose generators are all tts::sweep splits its calls between
  `--cores` threads, or the default number of OpenMP threads without it. `RefFunc` and `NewFunc`
  are then called from several threads at once, and the report does not depend on the number of
  threads.

  `RefFunc` returns the element type of the result of `NewFunc`, or the check does not compile. A
  reference computed in `double` for a function of `float` casts its result to `float`.

  `NewType` may hold several values of `RefType`, as a specialization of tts::adapter describes.

  A result of type `std::pair` or `std::tuple`, or of any type that specializes `std::tuple_size`
  and provides a `get` found by argument-dependent lookup, is compared member by member, each
  member with its own histogram, unless tts::adapter is specialized for it. The check then takes
  one bound for all members or one bound per member.

  A check that fails prints its histogram, and `--verbose` prints it for every check. Each bucket
  shows the first input whose distance falls in it. When `NewType` holds several values, that input
  is marked between stars within its group. `--ulpmax` replaces the bounds of every check of the
  run.

  @param Producer Generator of the inputs, or one generator per argument between parentheses.
  @param RefType  Argument type of `RefFunc`, or a tts::types list of them.
  @param NewType  Argument type of `NewFunc`, or a tts::types list of them.
  @param RefFunc  Function whose results are the reference.
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
