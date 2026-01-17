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
  template<typename Base> struct adapter
  {
    template<typename U, typename Func> static void run(Base const *&src, U *&dst, Func f) noexcept
    {
      *dst++ = f(*src++);
    }
    static auto retrieve(Base const *src) noexcept { return *src; }
    static void display(Base const &v) noexcept { printf("%s", as_text(v).data()); }
  };

  namespace _
  {
    template<typename Input, typename Output> struct histogram_entry
    {
      bool   status;
      Input  input_value;
      Output found_value, ref_value;
    };

    template<typename Type, typename In, typename Out, typename Func>
    void compute(In const &inputs, Out &outputs, Func fn)
    {
      auto in  = inputs.data();
      auto end = inputs.data() + inputs.size();
      auto out = outputs.data();
      while(in != end) adapter<Type>::run(in, out, fn);
    }

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
      if(ulp <= 1.5)
        bucket = static_cast<std::size_t>(_::ceil(ulp * 2));
      else if(_::is_inf(ulp))
        bucket = nb_buckets - 1;
      else
        bucket = _::min(nb_buckets - 2, static_cast<std::size_t>(_::log2(next2(ulp)) + 4.));
      return bucket;
    }

    template<typename... S> void header(S const &...s)
    {
      if(::tts::_::is_quiet) return;
      ((printf("%-*s", 16, s)), ...);
      puts("");
    }

    template<typename U, typename R, typename V>
    void results(U ulp, unsigned int count, R ratio, auto desc, V const &v)
    {
      if(::tts::_::is_quiet) return;
      if(ulp != -1)
        printf("%-16.1f%-16u%-16g%s", ulp, count, ratio, desc);
      else
        printf("%*s", static_cast<int>(48 + strlen(desc)), desc);
      adapter<V>::display(v);
      printf("\n");
    }

    template<typename P> void print_producer(P const &prod, auto alt)
    {
      if(::tts::_::is_quiet) return;
      if constexpr(requires(P const &p) { to_text(p); })
        printf("%s\n", ::tts::as_text(prod).data());
      else
        printf("%s\n", alt);
    }
  }

  template<typename RefType, typename NewType, typename Generator, typename RefFun, typename NewFun>
  double ulp_histogram(Generator g, RefFun reference, NewFun challenger)
  {
    using out_type  = std::decay_t<std::invoke_result_t<RefFun, RefType>>;
    using nout_type = std::decay_t<std::invoke_result_t<NewFun, NewType>>;

    //-- Find how many elements in a block
    std::size_t count = ::tts::arguments().value(4096, "--block");

    //-- Prepare blocks
    _::buffer<out_type> ref_out(count), new_out(count);
    _::buffer<RefType>  inputs(count);

    for(std::size_t i = 0; i < inputs.size(); ++i)
      inputs[ i ] = produce(type<RefType> {}, g, i, count);

    std::size_t repetition = ::tts::arguments().value(1, "--loop");

    double                  max_ulp    = 0.;
    std::size_t             nb_buckets = 2 + 1 + 16;
    std::size_t             nb_ulps    = 0;
    _::buffer<unsigned int> ulp_map(nb_buckets, 0);

    _::buffer<_::histogram_entry<NewType, nout_type>> samples(nb_buckets, {false, {}, {}, {}});

    for(std::size_t r = 0; r < repetition; ++r)
    {
      //--- Compute reference & challenger outputs
      _::compute<RefType>(inputs, ref_out, reference);
      _::compute<NewType>(inputs, new_out, challenger);

      //--- Compute ulp distances
      _::buffer<double> ulpdists(count);

      for(std::size_t i = 0; i < ulpdists.size(); ++i)
      {
        nb_ulps++;
        ulpdists[ i ] = ::tts::ulp_check(ref_out[ i ], new_out[ i ]);
        max_ulp       = _::max(max_ulp, ulpdists[ i ]);

        auto idx = _::last_bucket_less(nb_buckets, ulpdists[ i ]);
        ulp_map[ idx ]++;

        if(!samples[ idx ].status)
        {
          samples[ idx ] = {true,
                            adapter<NewType>::retrieve(&inputs[ i ]),
                            adapter<nout_type>::retrieve(&new_out[ i ]),
                            adapter<nout_type>::retrieve(&ref_out[ i ])};
        }
      }
    }

    _::header("Max ULP", "Count (#)", "Ratio Sum (%)", "Samples");
    if(!_::is_quiet)
      printf("--------------------------------------------------------------------------------\n");

    double ratio = 0.;

    for(std::size_t i = 0; i < ulp_map.size(); ++i)
    {
      if(ulp_map[ i ] != 0)
      {
        double ulps = 0;
        ratio += (100. * ulp_map[ i ]) / nb_ulps;

        if(i <= 3)
          ulps = i / 2.0;
        else if(i == nb_buckets - 1)
          ulps = std::numeric_limits<double>::infinity();
        else
          ulps = 1 << (i - 4);

        auto [ s, in, out, ref ] = samples[ i ];

        _::results(ulps, ulp_map[ i ], ratio, "Input:      ", in);
        _::results(-1., -1, -1., "Found:      ", out);
        _::results(-1., -1, -1., "instead of: ", ref);
        if(!_::is_quiet)
          printf(
              "--------------------------------------------------------------------------------\n");
      }
    }

    return max_ulp;
  }
}

//======================================================================================================================
/*!
  @def TTS_ULP_RANGE_CHECK
  @ingroup  test-precision
  @brief Generate a range based test between two functions

  Evaluates the histogram of ULP difference between two functions run on the same data set and that
  they lie in a given [ULP distance](rationale.html#ulp).

  @param Producer Data set generator to use.
  @param RefType  Type to use as reference function input.
  @param NewType  Type to use as challenger function input.
  @param RefFunc  Reference function to compare to.
  @param NewFunc  Challenger function to be compared to the reference one.
  @param Ulpmax   Maximal ULPs acceptable for passing the test.

  @groupheader{Example}
  @snippet doc/ulp_ranges.cpp snippet
**/
//======================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
#  define TTS_ULP_RANGE_CHECK(Producer, RefType, NewType, RefFunc, NewFunc, Ulpmax)
#else
#  define TTS_ULP_RANGE_CHECK(Producer, RefType, NewType, RefFunc, NewFunc, Ulpmax)                \
    [ & ]()                                                                                        \
    {                                                                                              \
      if(!::tts::_::is_quiet)                                                                      \
        printf("Comparing: %s<%s> with %s<%s> using ",                                             \
               TTS_STRING(RefFunc),                                                                \
               TTS_STRING(TTS_REMOVE_PARENS(RefType)),                                             \
               TTS_STRING(NewFunc),                                                                \
               TTS_STRING(TTS_REMOVE_PARENS(NewType)));                                            \
                                                                                                   \
      auto generator = TTS_REMOVE_PARENS(Producer);                                                \
      ::tts::_::print_producer(generator, TTS_STRING(TTS_REMOVE_PARENS(Producer)));                \
                                                                                                   \
      double tts_ulp_max         = Ulpmax;                                                         \
      double local_tts_threshold = ::tts::arguments().value(tts_ulp_max, "--ulpmax");              \
      double local_tts_max_ulp =                                                                   \
          ::tts::ulp_histogram<TTS_REMOVE_PARENS(RefType), TTS_REMOVE_PARENS(NewType)>(            \
              generator, RefFunc, NewFunc);                                                        \
                                                                                                   \
      if(local_tts_max_ulp <= local_tts_threshold) { ::tts::global_runtime.pass(); }               \
      else                                                                                         \
      {                                                                                            \
        TTS_FAIL("Expecting: %s similar to %s within %s ULP but found: %s ULP instead",            \
                 TTS_STRING(NewFunc),                                                              \
                 TTS_STRING(RefFunc),                                                              \
                 ::tts::as_text(local_tts_threshold).data(),                                       \
                 ::tts::as_text(local_tts_max_ulp).data());                                        \
      }                                                                                            \
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
    T operator()(auto, auto, auto) const { return ::tts::random_value(mini, maxi); }

    friend tts::text to_text(realistic_generator const &s)
    {
      return tts::text {"realistic_generator<%s>(%s,%s)",
                        tts::as_text(typename_<T>).data(),
                        tts::as_text(s.mini).data(),
                        tts::as_text(s.maxi).data()};
    }

  private:
    T mini, maxi;
  };
}
