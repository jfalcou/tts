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
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

namespace tts
{
  // CRTP base class for data producer
  template<typename T> struct producer
  {
    auto        next()            { return static_cast<T&>(*this).next();       }
    std::size_t size()      const { return static_cast<T const&>(*this).size(); }
    static auto prng_seed()       { return args.seed(); }
  };

  template<typename T> struct checker
  {
    private:
    std::vector<std::size_t>  histogram;
    std::vector<T>            sample_values;
    std::vector<T>            expected_values;
    std::vector<T>            result_values;
    std::size_t               char_shift;
    std::string               bar;

    // 0 + inf + 0.5 + 16 bits = all ulp between 0, 0.5 and 65536
    static constexpr std::size_t nb_buckets = 2+1+16;
    public:

    checker() : histogram(nb_buckets),
                sample_values(nb_buckets),
                expected_values(nb_buckets),
                result_values(nb_buckets),
                char_shift(std::numeric_limits<T>::digits10 + 2), bar(100,'-')
    {
    }

    template<typename P, typename RefFunc, typename OtherFunc>
    void run(producer<P>& p, RefFunc f, OtherFunc g, std::string_view fs, std::string_view gs)
    {
      std::cout << bar << "\n";
      std::cout << p.size() << " inputs of type " << tts::type_id<T>()
                << " comparing " << fs << " vs " << gs
                << " using " <<  tts::type_id<P>()
                << "\n";
      std::cout << bar << "\n";
      std::cout << std::left  << std::setw(12)            << "Max ULP"
                              << std::setw(12)            << "Count (#)"
                              << std::setw(12)            << "Ratio (%)"
                              << std::setw(char_shift+8)  << "Input"
                              << std::setw(char_shift+8)  << "Expected"
                              << std::setw(char_shift+8)  << "Result"
                              << "\n";
      std::cout << bar << std::endl;

      for(std::size_t i = 0 ; i < p.size(); ++i)
      {
        auto v          = p.next();
        auto reference  = f(v);
        auto challenger = g(v);

        auto ulp = ::tts::ulpdist(reference, challenger);

        // Find bucket to be the upper power of 2 of ulp
        std::size_t bucket;

             if(ulp == 0       ) bucket = 0;
        else if(ulp == 0.5     ) bucket = 1;
        else if(std::isinf(ulp)) bucket = nb_buckets-1;
        else                     bucket = std::min<std::size_t> ( nb_buckets-2,
                                                                  std::log2(next2(std::ceil(ulp)))+2
                                                                );

        sample_values[bucket]    = v;
        expected_values[bucket]  = reference;
        result_values[bucket]    = challenger;
        histogram[bucket]++;
      }

      for(std::size_t i=0;i<histogram.size();++i)
        display(i,p.size());

      std::cout << bar << "\n";
    }

    static std::size_t next2( std::size_t v )
    {
      v--;
      v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16;
      v++;

      return v;
    }


    static double ratio(std::size_t value, std::size_t count)
    {
      return 100.*(double(value)/count);
    }

    void display(std::size_t u, std::size_t cnt)
    {
      double ulps;

           if(u == 0            ) ulps = 0;
      else if(u == 1            ) ulps = 0.5;
      else if(u == nb_buckets-1 ) ulps = std::numeric_limits<T>::infinity();
      else                        ulps = 1<<(u-2);

      if(histogram[u])
      {
        std::cout << std::setprecision(4)
                  << std::left << std::setw(12)   << ulps
                  << std::setprecision(5)
                  << std::left << std::setw(12)  <<  histogram[u]
                  << std::setprecision(5)
                  << std::left << std::setw(12)  <<  ratio(histogram[u],cnt)
                  << std::setprecision(char_shift)
                  << std::left  << std::setw(char_shift+8)  <<  sample_values[u]
                  << std::left  << std::setw(char_shift+8)  <<  expected_values[u]
                  << std::left  << std::setw(char_shift+8)  <<  result_values[u]
                  << "\n";
      }
    }
  };
}

// Generate a range based test between two function
#define TTS_RANGE_CHECK(Producer, RefFunc, NewFunc)                                            \
  do                                                                                                \
  {                                                                                                 \
    ::tts::checker<typename decltype(Producer)::value_type> local_tts_checker;                      \
    local_tts_checker.run(Producer,RefFunc,NewFunc, TTS_STRING(RefFunc), TTS_STRING(NewFunc));      \
  } while(::tts::detail::is_false())                                                                \
/**/

#endif
