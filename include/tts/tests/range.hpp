//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_RANGE_HPP_INCLUDED
#define TTS_TESTS_RANGE_HPP_INCLUDED

#include <tts/detail/pp_helpers.hpp>
#include <tts/detail/rt_helpers.hpp>
#include <tts/detail/type_id.hpp>
#include <tts/detail/ulpdist.hpp>
#include <tts/detail/args.hpp>
#include <tts/tests/infos.hpp>
#include <tts/tests/relation.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

#if defined(_OPENMP)
#include <omp.h>
#endif

namespace tts::detail
{
  class text_field
  {
    int width_;

    public:
    text_field( int width ) : width_( width ) {}
    friend std::ostream& operator<<( std::ostream& os, text_field const& manip )
    {
      os.setf( std::ios_base::left, std::ios_base::adjustfield );
      os.fill( ' ' );
      os.width( manip.width_ );
      os.precision( 2 );
      return os;
    }
  };

  class value_field
  {
    int width_, precision_;

    public:
    value_field( int width, int prec = 2 ) : width_( width ), precision_(prec) {}
    friend std::ostream&
    operator<<( std::ostream& os, value_field const& manip )
    {
        os.setf( std::ios_base::left , std::ios_base::adjustfield );
        os.setf( std::ios_base::fixed, std::ios_base::floatfield  );
        os.fill( ' ' );
        os.precision( manip.precision_ );
        os.width( manip.width_ );
        return os;
    }
  };
}

namespace tts
{
  // CRTP base class for data producer
  template<typename T> struct producer
  {
    auto        first()     const noexcept { return *tts::detail::begin(self().first());}
    auto        last()      const noexcept { return *tts::detail::begin(self().last()); }
    auto        next()            noexcept { return self().next();   }
    std::size_t size()      const noexcept { return self().size();   }
    static auto prng_seed()       noexcept { return args.seed();     }
    static auto count()           noexcept { return args.count();    }
    static auto ulpmax()          noexcept { return args.ulpmax();   }
    static auto hex()             noexcept { return args.hex();      }

        

    auto&       self()       noexcept { return static_cast<T&>(*this);        }
    auto const& self() const noexcept { return static_cast<T const&>(*this);  }
  };

  template<typename T, typename S = T> struct checker
  {
    private:
    using base_type = std::decay_t<decltype(*tts::detail::begin(T()))>;
    using resl_type = std::decay_t<decltype(*tts::detail::begin(S()))>;
    std::vector<std::size_t>  histogram;
    std::vector<base_type>    sample_values;
    std::vector<resl_type>    expected_values;
    std::vector<resl_type>    result_values;
    double                    max_ulp = 0.0;
    std::size_t               char_shift;
    std::string               bar;

    // 0 + inf + 0.5 + 16 bits = all ulp between 0, 0.5 and 65536
    static constexpr std::size_t nb_buckets = 2+1+16;
    public:
    checker() : histogram(nb_buckets),
                sample_values(nb_buckets),
                expected_values(nb_buckets), result_values(nb_buckets),
                char_shift(std::numeric_limits<base_type>::digits10+2), bar(100,'-')
    {
    }

    static std::size_t thread_id()
    {
      #if defined(_OPENMP)
      return omp_get_thread_num();
      #else
      return 0;
      #endif
    }

    static std::size_t thread_count()
    {
      #if defined(_OPENMP)
      return omp_get_num_threads();
      #else
      return 1;
      #endif
    }

    template<typename P, typename RefFunc, typename OtherFunc>
    double run(producer<P> const& q, RefFunc f, OtherFunc g, std::string_view fs, std::string_view gs,  double &threshold)
    {
      threshold = (threshold > 1.5) ? next2(threshold) : std::ceil(threshold*2)/2;
      std::size_t nbthreads = 1;

      #pragma omp parallel
      {
        if(!thread_id()) nbthreads = thread_count();
      }

      std::cout << bar << "\n";
      std::cout << q.size() << " inputs comparing " << fs << " vs " << gs
                << " with " << tts::type_id<P>()
                << " in range [" << +q.first() << ", " << +q.last() << "["
                << " using " << nbthreads << " threads.\n"
                << "\n";
      std::cout << bar << "\n";
      std::cout << std::left  << detail::text_field(16) << "Max ULP"
                              << detail::text_field(16) << "Count (#)"
                              << detail::text_field(16) << "Cum. Ratio (%)"
                              << detail::text_field(10) << "Results"
                              << "\n";
      std::cout << bar << std::endl;

      // Compute histogram in parallel
      #pragma omp parallel
      {
        auto sz = thread_count();
        auto id = thread_id();
        auto per_thread = q.size()/sz;
        long unsigned int i0, i1;

        if (id == 0) nbthreads = sz;

        if(id == sz-1)
        {
          per_thread = q.size() - per_thread*(sz-1);
          i1 = q.size();
          i0 = i1 - per_thread;
        }
        else
        {
          i0 = id*per_thread;
          i1 = (id+1)*per_thread;
        }

        P p(q, i0, i1, sz);

        std::vector<std::size_t>  local_histogram(nb_buckets);
        std::vector<base_type>    local_sample_values(nb_buckets, P::max());
        std::vector<bool>         found(nb_buckets);

        std::size_t n = ::tts::detail::size(typename P::value_type{});

        double local_max_ulp = 0.0;

        for(auto i = i0 ; i < i1; i+=n)
        {
          auto v          = p.next();
          auto reference  = f(v);
          auto challenger = g(v);
          auto bv = ::tts::detail::begin(v);
          auto br = ::tts::detail::begin(reference);
          auto bc = ::tts::detail::begin(challenger);

          for(std::size_t k=0;k<n;k++)
          {
            auto ulp = ::tts::ulpdist(*br, *bc);

            // Find bucket to be the upper power of 2 of ulp
            std::size_t bucket = last_bucket_less(ulp);
            local_max_ulp = std::max(local_max_ulp, ulp);

            if( found[bucket] == false)
            {
              found[bucket]           = true;
              sample_values[bucket]   = *bv;
            }

            local_histogram[bucket]++;
            bv++;
            br++;
            bc++;
          }
        }

        #pragma omp barrier

        // Merge histogram
        #pragma omp critical
        {
          for(std::size_t i=0;i<nb_buckets;++i)
          {
            histogram[i]       += local_histogram[i];
            sample_values[i]    = std::min(sample_values[i], local_sample_values[i]);
            expected_values[i]  = *::tts::detail::begin(f(T(sample_values[i])));
            result_values[i]    = *::tts::detail::begin(g(T(sample_values[i])));
          }

          max_ulp = std::max(max_ulp, local_max_ulp);
        }
      }

      std::size_t total = 0;
      std::size_t total_ok = 0;
      std::size_t last_bucket_ok = last_bucket_less(threshold);
      for(std::size_t i=0;i<histogram.size();++i)
      {
        std::size_t tmp = display(i,q.size(),gs, q.hex());
        total += tmp;
        if(i <= last_bucket_ok) total_ok += tmp;
      }

      std::cout << bar << "\n";
      std::cout << detail::text_field(16) << "Total:   " << detail::value_field(16)  << total << "\n";
      std::cout << "Percent of values under "
                << threshold << " ulp" << ((threshold < 2.0) ? "" : "s" )  << " is "
                << std::fixed << (total_ok*100.0)/q.size() << "% (" << total_ok
                << " values over " << q.size() << ").\n";

      std::cout << bar << "\n";

      return max_ulp;
    }

    static std::size_t last_bucket_less(double ulp)
    {
      size_t bucket;
      if     (ulp <= 1.5     ) bucket = std::ceil(ulp*2);
      else if(std::isinf(ulp)) bucket = nb_buckets-1;
      else                     bucket = std::min<std::size_t> ( nb_buckets-2,
                                                                std::log2(next2(ulp))+4
                                                              );
      return bucket;
    }

    static std::size_t next2( double x )
    {
      std::size_t v = std::ceil(x);
      v--;
      v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16;
      v++;

      return v;
    }

    static double ratio(std::size_t value, std::size_t count)
    {
      return 100.*(double(value)/count);
    }

    auto display(std::size_t u, std::size_t cnt, std::string_view gs, bool hex)
    {
      static double cumhist = 0.0;
      double ulps;

      if     (u <= 3            ) ulps = u/2.0;
      else if(u == nb_buckets-1 ) ulps = std::numeric_limits<double>::infinity();
      else                        ulps = 1<<(u-4);

      if(histogram[u])
      {
        cumhist+= histogram[u];
        std::cout << detail::text_field(16)   <<  ulps
                  << detail::value_field(16)  <<  histogram[u]
                  << detail::value_field(16)  <<  ratio(cumhist,cnt)
                  << detail::value_field(10,char_shift) << "Found: "
                  << gs << "(";

        if constexpr(std::is_floating_point_v<base_type>)
        {
          if (hex)
            std::cout << std::hexfloat << std::showpos;
          else
            std::cout << std::scientific << std::showpos;
        }
        
        std::cout << +sample_values[u]  << ") = ";

        if constexpr(!std::is_floating_point_v<resl_type>)
          std::cout << std::fixed << std::noshowpos;
        std::cout << +result_values[u];

        if(u) std::cout << " instead of " << +expected_values[u];
        std::cout << "\n";

        std::cout << std::fixed<< std::noshowpos;
      }

      return histogram[u];
    }
  };
}

// Generate a range based test between two function
#define TTS_ULP_RANGE_CHECK(Producer, RefFunc, NewFunc, Ulpmax)                                     \
  do                                                                                                \
  {                                                                                                 \
    using local_tts_base_type = typename decltype(Producer)::value_type;                            \
    using local_tts_resl_type = decltype(RefFunc(local_tts_base_type()));                           \
                                                                                                    \
    ::tts::checker<local_tts_base_type, local_tts_resl_type> local_tts_checker;                     \
                                                                                                    \
    double local_tts_threshold = ::tts::args.has_ulp() ? Producer.ulpmax() : Ulpmax;                \
    double local_tts_max_ulp = local_tts_checker.run(Producer,RefFunc,NewFunc, TTS_STRING(RefFunc)  \
                                                    , TTS_STRING(NewFunc),local_tts_threshold);     \
                                                                                                    \
    TTS_LESS_EQUAL(local_tts_max_ulp, local_tts_threshold);                                         \
  } while(::tts::detail::is_false())                                                                \
/**/

// Generate a range based test between two function
#define TTS_RANGE_CHECK(Producer, Ref, New) TTS_ULP_RANGE_CHECK(Producer, Ref, New, 2.0)
/**/

#endif
