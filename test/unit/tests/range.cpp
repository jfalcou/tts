//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <array>

int ok_x  (float x) { return static_cast<int>(x); }

int ajar_x(float x) { return static_cast<int>((int(x) % 2 == 1) ? x : x+x); }

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

    static void display(std::array<X,4> const& v) noexcept
    {
      printf("%s",as_text(v).data());
    }
  };
}

TTS_CASE( "Test stateless range check" )
{
  TTS_ULP_RANGE_CHECK ( [](auto i, auto c) { return (100.f*i)/c; }
                      , float, (std::array<float,4>)
                      ,  ok_x, ajar_ax
                      , 128
                      );
};

TTS_CASE( "Test stateful range check" )
{
  TTS_ULP_RANGE_CHECK ( tts::realistic_generator<float>(-10,10)
                      , float, (std::array<float,4>)
                      ,  ok_x, ajar_ax
                      , 128
                      );
};
