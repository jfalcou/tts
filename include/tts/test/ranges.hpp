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
  //====================================================================================================================
  /*!
    @ingroup tools-precision
    @brief Make a type usable in a range check

    A range check stores its data as scalars, and a specialization maps a type to them:
    `element_type` is the scalar, `size` the number of scalars per value, `load` and `store` the
    conversions between a value and `size` consecutive scalars. The primary template covers a
    scalar type, with `size` equal to one.

    @groupheader{Example}
    @snippet doc/range_adapter.cpp snippet

    @tparam T Type to adapt
  **/
  //====================================================================================================================
  template<typename T> struct adapter
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
      [[no_unique_address]] T value;
    };

    template<typename Seq, typename... Ts> struct pack_base;

    template<std::size_t... Is, typename... Ts>
    struct pack_base<std::index_sequence<Is...>, Ts...> : leaf<Is, Ts>...
    {
      pack_base() = default;
      explicit pack_base(Ts... ts)
          : leaf<Is, Ts> {static_cast<Ts&&>(ts)}...
      {
      }
    };

    template<typename... Ts> struct pack : pack_base<std::index_sequence_for<Ts...>, Ts...>
    {
      using base_type = pack_base<std::index_sequence_for<Ts...>, Ts...>;
      using base_type::base_type;
    };

    template<std::size_t I, typename T> constexpr T& get(leaf<I, T>& l) noexcept
    {
      return l.value;
    }

    template<std::size_t I, typename T> constexpr T const& get(leaf<I, T> const& l) noexcept
    {
      return l.value;
    }

    // A single producer serves every argument of the check.
    template<typename... Gs> struct grid
    {
      template<std::size_t K> auto const& leaf() const noexcept
      {
        if constexpr(sizeof...(Gs) == 1) return _::get<0>(leaves);
        else return _::get<K>(leaves);
      }

      pack<Gs...> leaves;
    };

    template<typename T> struct is_grid : std::false_type
    {
    };

    template<typename... Gs> struct is_grid<grid<Gs...>> : std::true_type
    {
    };

    template<typename... Gs> grid<Gs...> make_grid(Gs... gs)
    {
      return grid<Gs...> {pack<Gs...> {static_cast<Gs&&>(gs)...}};
    }

    // A block of count tuples is the full product of r samples per argument.
    inline std::size_t grid_side(std::size_t count, std::size_t arity) noexcept
    {
      if(arity == 1) return count;

      std::size_t r = 1;
      for(;;)
      {
        std::size_t p = 1;
        for(std::size_t k = 0; k < arity; ++k)
          p *= r;
        if(p >= count) return r;
        ++r;
      }
    }

    template<typename Input, typename Output> struct histogram_entry
    {
      bool   status;
      Input  input_value;
      Output found_value, ref_value;
    };

    inline std::size_t last_bucket_less(std::size_t nb_buckets, double ulp) noexcept
    {
      auto next2 = [](double x)
      {
        auto v = static_cast<std::size_t>(_::ceil(x));
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        return v;
      };

      std::size_t bucket;
      if(ulp <= 1.5) bucket = static_cast<std::size_t>(_::ceil(ulp * 2));
      else if(_::is_inf(ulp)) bucket = nb_buckets - 1;
      else bucket = _::min(nb_buckets - 2, static_cast<std::size_t>(_::log2(next2(ulp)) + 4));
      return bucket;
    }

    template<typename... S> void header(S const&... s)
    {
      ((::tts::output().write("%-*s", 16, s)), ...);
      ::tts::output().writeln();
    }

    template<typename V> void show(V const& v)
    {
      ::tts::output().write(::tts::as_text(v));
    }

    // One value of the challenger as its scalars, the element of the bucket at position mark.
    template<typename T, std::size_t N> struct group
    {
      T           values[ N ]; // NOSONAR - <array> costs compile time
      std::size_t mark;
    };

    template<std::size_t N, typename T> group<T, N> make_group(T const* first, std::size_t mark)
    {
      group<T, N> g {{}, mark};
      for(std::size_t k = 0; k < N; ++k)
        g.values[ k ] = first[ k ];
      return g;
    }

    template<typename T, std::size_t N> void show(group<T, N> const& g)
    {
      if constexpr(N == 1) show(g.values[ 0 ]);
      else
      {
        ::tts::output().write("{ ");
        for(std::size_t k = 0; k < N; ++k)
        {
          ::tts::output().write(k == 0 ? "" : " ");
          if(k == g.mark) ::tts::output().write("*");
          show(g.values[ k ]);
          if(k == g.mark) ::tts::output().write("*");
        }
        ::tts::output().write(" }");
      }
    }

    template<typename... Ts> void show(pack<Ts...> const& p)
    {
      [ & ]<std::size_t... Ks>(std::index_sequence<Ks...>)
      {
        ((::tts::output().write(Ks ? ", " : ""), show(_::get<Ks>(p))), ...);
      }(std::index_sequence_for<Ts...> {});
    }

    template<typename... Ts> void show_call(char const* name, types<Ts...>)
    {
      ::tts::output().write(name);
      ::tts::output().write("(");
      [ & ]<std::size_t... Ks>(std::index_sequence<Ks...>)
      {
        (
        (::tts::output().write(Ks ? ", " : ""),
         ::tts::output().write(::tts::as_text(typename_<Ts>))),
        ...);
      }(std::index_sequence_for<Ts...> {});
      ::tts::output().write(")");
    }

    template<typename U, typename R, typename V>
    void results(U ulp, unsigned int count, R ratio, auto desc, V const& v)
    {
      assert(desc && "Description cannot be null");
      if(ulp != -1) ::tts::output().write("%-16.1f%-16u%-16g%s", ulp, count, ratio, desc);
      else ::tts::output().write("%*s", static_cast<int>(48 + strlen(desc)), desc); // NOSONAR
      show(v);
      ::tts::output().writeln();
    }

    template<typename... Gs> void print_producer(grid<Gs...> const& prod, auto alt)
    {
      if constexpr((::tts::_::described<Gs> && ...))
      {
        [ & ]<std::size_t... Ks>(std::index_sequence<Ks...>)
        {
          (
          (::tts::output().write(Ks ? ", " : ""),
           ::tts::output().write(::tts::as_text(_::get<Ks>(prod.leaves)))),
          ...);
        }(std::index_sequence_for<Gs...> {});
        ::tts::output().writeln();
      }
      else ::tts::output().writeln(alt);
    }

    template<typename Samples, typename Out> struct ulp_report
    {
      static constexpr std::size_t          nb_buckets = 2 + 1 + 16;

      double                                max_ulp    = 0.;
      std::size_t                           nb_ulps    = 0;
      buffer<unsigned int>                  ulp_map    = buffer<unsigned int>(nb_buckets, 0u);
      buffer<histogram_entry<Samples, Out>> samples =
      buffer<histogram_entry<Samples, Out>>(nb_buckets, {false, {}, {}, {}});

      void print() const
      {
        _::header("Max ULP", "Count (#)", "Ratio Sum (%)", "Samples");
        _::separator();

        double ratio = 0.;

        for(std::size_t i = 0; i < ulp_map.size(); ++i)
        {
          if(ulp_map[ i ] != 0)
          {
            double ulps  = 0;
            ratio       += (100. * ulp_map[ i ]) / static_cast<double>(nb_ulps);

            if(i <= 3) ulps = static_cast<double>(i) / 2.0;
            else if(i == nb_buckets - 1) ulps = std::numeric_limits<double>::infinity();
            else ulps = 1 << (i - 4);

            auto const& [ s, in, out, ref ] = samples[ i ];

            _::results(ulps, ulp_map[ i ], ratio, "Input:      ", in);
            _::results(-1., 0, 0., "Found:      ", out);
            _::results(-1., 0, 0., "instead of: ", ref);
            _::separator();
          }
        }
      }
    };

    // An adapter written for TTS 3, through run and retrieve.
    template<typename T>
    concept legacy_adapter = requires { &adapter<T>::retrieve; };

    template<typename RefList, typename NewList> struct ulp_scan;

    template<typename... As, typename... Ws> struct ulp_scan<types<As...>, types<Ws...>>
    {
      static_assert(sizeof...(As) == sizeof...(Ws),
                    "[TTS] TTS_ULP_RANGE_CHECK needs one challenger type per reference type.");

      static_assert((!legacy_adapter<Ws> && ...),
                    "[TTS] tts::adapter<T>::run and retrieve are no longer the protocol. "
                    "Define element_type, size, load, and store instead.");

      template<typename G, typename RefFun, typename NewFun>
      static auto run(G const& g, RefFun reference, NewFun challenger)
      {
        return run(std::index_sequence_for<As...> {}, g, reference, challenger);
      }

      template<std::size_t... Ks, typename G, typename RefFun, typename NewFun>
      static auto run(std::index_sequence<Ks...>, G const& g, RefFun reference, NewFun challenger)
      {
        using out_type              = std::decay_t<std::invoke_result_t<RefFun, As...>>;
        using nout_type             = std::decay_t<std::invoke_result_t<NewFun, Ws...>>;

        constexpr std::size_t arity = sizeof...(As);
        constexpr std::size_t card  = adapter<nout_type>::size;

        static_assert(((adapter<Ws>::size == card) && ...),
                      "[TTS] TTS_ULP_RANGE_CHECK needs every challenger type and its result to "
                      "hold the same number of elements.");
        static_assert((std::is_same_v<typename adapter<Ws>::element_type, As> && ...),
                      "[TTS] TTS_ULP_RANGE_CHECK needs every challenger type to hold elements of "
                      "its reference type.");
        static_assert(std::is_same_v<typename adapter<nout_type>::element_type, out_type>,
                      "[TTS] TTS_ULP_RANGE_CHECK needs the challenger result to hold elements of "
                      "the reference result type.");

        //-- Find how many elements in a block, whole groups only
        std::size_t count = ::tts::arguments().value(std::size_t {4096}, "--block");
        count             = ((count + card - 1) / card) * card;

        //-- Prepare blocks: every combination of the arguments' samples
        std::size_t side = grid_side(count, arity);
        std::size_t stride[ arity ]; // NOSONAR - <array> costs compile time
        {
          std::size_t s = 1;
          for(std::size_t k = arity; k-- > 0;)
          {
            stride[ k ]  = s;
            s           *= side;
          }
        }

        buffer<out_type>    ref_out(count), new_out(count);
        pack<buffer<As>...> inputs {buffer<As>(count)...};

        for(std::size_t i = 0; i < count; ++i)
        {
          (
          (_::get<Ks>(inputs)[ i ] =
           ::tts::produce(type<As> {},
                          g.template leaf<Ks>(),
                          static_cast<std::ptrdiff_t>((i / stride[ Ks ]) % side),
                          static_cast<std::ptrdiff_t>(side))),
          ...);
        }

        std::size_t repetition = ::tts::arguments().value(std::size_t {1}, "--loop");

        using samples_type     = pack<group<As, card>...>;

        ulp_report<samples_type, group<out_type, card>> report;
        auto& [ max_ulp, nb_ulps, ulp_map, samples ] = report;
        constexpr std::size_t nb_buckets             = decltype(report)::nb_buckets;

        for(std::size_t r = 0; r < repetition; ++r)
        {
          //--- Compute reference & challenger outputs
          for(std::size_t i = 0; i < count; ++i)
          {
            ref_out[ i ] = reference(_::get<Ks>(inputs)[ i ]...);
          }

          for(std::size_t i = 0; i < count; i += card)
          {
            auto res = challenger(adapter<Ws>::load(_::get<Ks>(inputs).data() + i)...);
            adapter<nout_type>::store(res, new_out.data() + i);
          }

          //--- Compute ulp distances
          for(std::size_t i = 0; i < count; ++i)
          {
            nb_ulps++;
            auto ulp = ::tts::ulp_check(ref_out[ i ], new_out[ i ]);
            max_ulp  = _::max(max_ulp, ulp);

            auto idx = _::last_bucket_less(nb_buckets, ulp);
            ulp_map[ idx ]++;

            if(!samples[ idx ].status)
            {
              auto mark      = i % card;
              auto first     = i - mark;
              samples[ idx ] = {
              true,
              samples_type {make_group<card>(_::get<Ks>(inputs).data() + first, mark)...},
              make_group<card>(new_out.data() + first, mark),
              make_group<card>(ref_out.data() + first, mark)};
            }
          }
        }

        return report;
      }
    };
  }

  template<typename RefType, typename NewType, typename Generator, typename RefFun, typename NewFun>
  double ulp_histogram(Generator g, RefFun reference, NewFun challenger)
  {
    using scan  = _::ulp_scan<as_type_list_t<RefType>, as_type_list_t<NewType>>;

    auto report = [ & ]
    {
      if constexpr(_::is_grid<Generator>::value) return scan::run(g, reference, challenger);
      else return scan::run(_::make_grid(g), reference, challenger);
    }();

    if(!::tts::is_quiet()) report.print();
    return report.max_ulp;
  }
}

//======================================================================================================================
/*!
  @def TTS_ULP_RANGE_CHECK
  @ingroup  test-precision
  @brief Checks a function against a reference over a data set, within a given ULP distance

  Evaluates `NewFunc` and `RefFunc` on the same data set, sorts the [ULP
distance](rationale.html#ulp) between their results into a histogram, and passes when the largest
distance stays within `Ulpmax`. The data set holds `--block` values, 4096 by default, each built by
the generator from its index.
  `--ulpmax` replaces `Ulpmax` for every check of the run.

  A function of several arguments takes a tts::types list as `RefType` and as `NewType`, one type
  per argument in the order of the call, and one generator per argument between parentheses, or a
  single generator for all. Each generator then produces the smallest number of values whose product
  over the arguments reaches `--block`, 64 for two arguments by default, and the data set covers
  every combination of these values.

  `NewType` may hold several values of `RefType`, as described by a specialization of tts::adapter.

  A check that fails prints its histogram, and every check prints it under `--verbose`. Each bucket
  shows the first element whose distance falls in it, marked between stars within its group when
  `NewType` holds several values.

  @param Producer Data generator, or one per argument between parentheses.
  @param RefType  Argument type of the reference function, or a tts::types list of them.
  @param NewType  Argument type of the challenger function, or a tts::types list of them.
  @param RefFunc  Reference function.
  @param NewFunc  Function compared to the reference.
  @param Ulpmax   Largest ULP distance accepted.

  @groupheader{Example}
  @snippet doc/ulp_ranges.cpp snippet

  @see tts::adapter
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#define TTS_ULP_RANGE_CHECK(Producer, RefType, NewType, RefFunc, NewFunc, Ulpmax)
#else
#define TTS_ULP_RANGE_CHECK(Producer, RefType, NewType, RefFunc, NewFunc, Ulpmax)                  \
  [ & ]()                                                                                          \
  {                                                                                                \
    auto   local_tts_generator = ::tts::_::make_grid(TTS_REMOVE_PARENS(Producer));                 \
    double tts_ulp_max         = Ulpmax;                                                           \
    double local_tts_threshold = ::tts::arguments().value(tts_ulp_max, "--ulpmax");                \
    auto   local_tts_report    = ::tts::_::ulp_scan<                                               \
    ::tts::as_type_list_t<TTS_REMOVE_PARENS(RefType)>,                                             \
    ::tts::as_type_list_t<TTS_REMOVE_PARENS(NewType)>>::run(local_tts_generator,                   \
                                                                 RefFunc,                          \
                                                                 NewFunc);                         \
    bool local_tts_passed = local_tts_report.max_ulp <= local_tts_threshold;                       \
                                                                                                   \
    if(::tts::is_detailed() || (!local_tts_passed && !::tts::is_quiet()))                          \
    {                                                                                              \
      ::tts::output().write("Comparing: ");                                                        \
      ::tts::_::show_call(TTS_STRING(RefFunc),                                                     \
                          ::tts::as_type_list_t<TTS_REMOVE_PARENS(RefType)> {});                   \
      ::tts::output().write(" with ");                                                             \
      ::tts::_::show_call(TTS_STRING(NewFunc),                                                     \
                          ::tts::as_type_list_t<TTS_REMOVE_PARENS(NewType)> {});                   \
      ::tts::output().write(" using ");                                                            \
      ::tts::_::print_producer(local_tts_generator, TTS_STRING(TTS_REMOVE_PARENS(Producer)));      \
      local_tts_report.print();                                                                    \
    }                                                                                              \
                                                                                                   \
    if(local_tts_passed)                                                                           \
    {                                                                                              \
      ::tts::global_runtime.pass();                                                                \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TTS_FAIL("Expecting: %s similar to %s within %s ULP but found: %s ULP instead",              \
               TTS_STRING(NewFunc),                                                                \
               TTS_STRING(RefFunc),                                                                \
               ::tts::as_text(local_tts_threshold).data(),                                         \
               ::tts::as_text(local_tts_report.max_ulp).data());                                   \
    }                                                                                              \
  }()
/**/
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
