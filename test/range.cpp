//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include <tts/ranges.hpp>

int ok_x  (float x) { return x; }

int ajar_x(float x) { return (int(x) % 2 == 1) ? x : x+x; }

std::array<int,4> ajar_ax(std::array<float,4> const& x)
{
  std::array<int,4> that{ajar_x(x[0]), ajar_x(x[1]), ajar_x(x[2]), ajar_x(x[3])};
  return that;
}

namespace tts
{
  template<typename X> struct adapter<std::array<X,4>>
  {
    template<typename U, typename Func>
    static void run(X const*& src, U*& dst, Func f) noexcept
    {
      std::array<X,4> that{*src++,*src++,*src++,*src++};
      auto res = f(that);
      *dst++ = res[0];
      *dst++ = res[1];
      *dst++ = res[2];
      *dst++ = res[3];
    }

    static auto retrieve(X const* src) noexcept
    {
      // realign on 4*4
      auto b = reinterpret_cast<std::uintptr_t>(src);
      b = (b/16)*16;
      auto p = reinterpret_cast<X*>(b);

      std::array<X,4> that{*p++,*p++,*p++,*p++};
      return that;
    }

    static void display(std::array<X,4> const& v, std::ostream& os) noexcept
    {
      os << "[";
      for(auto const& e : v)
        std::cout << e << " ";
      os << "]";
    }
  };
}

namespace tts
{

  template<typename T, typename Distribution>
  struct prng_generator
  {
    template<typename U, typename V>
    prng_generator(U mn, V mx) : distribution_( static_cast<T>(mn)
                                              , static_cast<T>(mx)
                                              )
    {}

    void init( options const& args )
    {
      auto seed = args.value_or(-1, "-s", "--seed");

      if(seed == -1 )
      {
        auto now  = std::chrono::high_resolution_clock::now();
        seed      = static_cast<unsigned int>(now.time_since_epoch().count());
      }

      generator_.seed(seed);
    }

    T operator()(int, int)
    {
      return distribution_(generator_);
    }

    Distribution  distribution_;
    std::mt19937  generator_;
  };

  template<typename T>
  using uniform_prng = prng_generator < T
                                      , std::conditional_t< std::is_floating_point_v<T>
                                                          , std::uniform_real_distribution<T>
                                                          , std::uniform_int_distribution<T>
                                                          >
                                      >;
}

TTS_CASE( "Test stateless range check" )
{
  TTS_ULP_RANGE_CHECK ( [](int i, int c) { return (100.f*i)/c; }
                      , float, (std::array<float,4>)
                      ,  ok_x, ajar_ax
                      , 128
                      );
}

TTS_CASE( "Test stateful range check" )
{
  TTS_ULP_RANGE_CHECK ( tts::uniform_prng<float>(-10,10)
                      , float, (std::array<float,4>)
                      ,  ok_x, ajar_ax
                      , 128
                      );
}
