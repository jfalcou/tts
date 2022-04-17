//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <cmath>
#include <limits>
#include <numeric>
#include <random>
#include <type_traits>

namespace tts
{
  namespace detail
  {
    template<typename T> auto dec(T x)  { return x-T(1); }
    template<typename T> auto inc(T x)  { return x+T(1); }
    template<typename T> T    abs_s(T x)
    {
      return x == std::numeric_limits<T>::min() ? std::numeric_limits<T>::max() : std::abs(x);
    }

    template< typename T = double > struct fp_realistic_distribution
    {
      using result_type = T;
      struct param_type
      {
        T a, b;
        param_type(T aa, T bb) : a(aa),  b(bb) {};
      };

      fp_realistic_distribution() : fp_realistic_distribution(0.0, 0.1, 300) { }

      fp_realistic_distribution(T aa, T bb)
        : a(std::min(aa, bb)),
          b(std::max(aa, bb)),
          nb(300),
          sd(T(0),T(1)),
          ird(1, nb-1){
      };

      explicit fp_realistic_distribution( const param_type& params )
        : a(params.a),
          b(params.b),
          nb(300),
          sd(T(0),T(1)),
          ird(1, nb-1){};

      void reset(){
        sd.reset();
        ird.reset();
      };

      template< class Generator > result_type operator()( Generator& gen )
      {
        return (*this)(gen, a, b);
      }

      template< class Generator > result_type operator()( Generator& gen, result_type aa, result_type bb)
      {
        result_type res;
        if(aa == bb) res = aa;
        else if(bb <= aa+result_type(0.5)) res = aa+(bb-aa)*sd(gen);
        else if((aa >= 0 && bb <= 1) || (bb <= 0 && aa >= -1)) res = aa+(bb-aa)*sd(gen);
        else if(aa >= -1 && bb <= 1) res =((sd(gen)*(bb-aa) > -aa) ? aa : bb)*sd(gen);
        else
        {
          auto i = ird(gen);
          if (aa >= 1) // bb > aa
          {
            auto la =  std::log2(aa);
            auto f =  std::log2(bb)-la;
            auto rand = sd(gen);
            auto x = la+f*(i-1+rand)/nb;
            res = std::min(std::exp2(x), bb);
          }
          else if (bb <= -1) // aa < bb
          {
            res = -(*this)(gen, std::abs(bb), std::abs(aa));
          }
          else if (aa >= 0) // aa < 1,  bb > aax
          {
            if(i == 1)
            {
              auto r = sd(gen);
              if (r> aa) res =r;
              else {i = 2;res = 0; }
            }
            else res = (*this)(gen, result_type(1), bb);
          }
          else if (bb <= 0) // aa < -1
          {
            if(i == 1)
            {
              auto r = sd(gen);
              if (r> -bb) res =-r;
              else { i = 2; res = 0;}
            }
            else res = (*this)(gen, result_type(-1), aa);
          }
          else // aa < 0 bb > 0
          {
            auto z = result_type(0);
            auto choice = sd(gen)*std::midpoint(bb, -aa) <  bb/2;
            if (choice)
            {
              res = (*this)(gen, z, bb);
            }
            else
            {
              res = (*this)(gen, aa, z);
            }

          }
        }
        return res;
      }

      template< class Generator > result_type operator()( Generator& g, const param_type& params );

      param_type  param() const noexcept { return {a, b}; }
      void        param( const param_type& params ) noexcept
      {
        a = params.a;
        b = params.b;
      }

      result_type min()  const noexcept {return a; };
      result_type max()  const noexcept {return b; };

    private:

      T a;
      T b;
      int nb;
      std::uniform_real_distribution<T> sd;
      std::uniform_int_distribution<int> ird;
    };

    /////////////////////////////////////////////////////////////////////
    // integral case

    template< typename T = int32_t > struct integral_realistic_distribution
    {
      using result_type = T;
      struct param_type
      {
        T a;
        T b;
        param_type(T aa, T bb) : a(aa),  b(bb){};
      };

      integral_realistic_distribution() : integral_realistic_distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max()) { }

      integral_realistic_distribution( T aa, T bb, int nbb = 300)
        : a(std::min(aa, bb)),
          b(std::max(aa, bb)),
          nb(nbb),
          sd(0.0, 1.0),
          ird(a, b),
          ird2(1, nb) {};

      explicit integral_realistic_distribution( const param_type& params )
        : a(params.a),
          b(params.b),
          nb(params.nb),
          sd(0.0, 1.0),
          ird(a, b),
          ird2(1, nb) {};

      void reset(){
        ird.reset();
        ird2.reset();
      };

      template< class Generator > result_type operator()( Generator& gen )
      {
        return (*this)(gen, a, b);
      }

      template< class Generator > result_type operator()( Generator& gen, result_type aa, result_type bb)
      {
        result_type res(0);
        if(detail::abs_s(aa) < 256 && detail::abs_s(bb) < 256)
        {
          res = ird(gen);
        }

        auto l2 = [](auto x){return std::log2(detail::inc(double(x)));   };
        auto e2 = [](auto x){return detail::dec(T(std::round(std::exp2(x)))); };

        if(aa == bb) res = aa;
        else
        {
          auto i = ird2(gen);
          if (aa >= 0) // bb > aa
          {
            auto la =  l2(aa);
            auto lb =  l2(bb);
            auto f =   lb-la;
            auto rand = sd(gen);
            auto x = la+f*(i-1+rand)/nb;
            res = e2(x);
          }
          else if (bb <= 0) // aa < bb
          {
            res = -(*this)(gen, detail::abs_s(bb), detail::abs_s(aa));
          }
          else // aa < 0 bb > 0
          {
            auto z = result_type(0);
            auto choice = sd(gen)*std::midpoint(detail::abs_s(bb), detail::abs_s(aa)) <  bb/2;
            if (choice)
            {
              res = (*this)(gen, z,detail::abs_s(bb));
            }
            else
            {
              res = -(*this)(gen, z, detail::abs_s(aa));
            }
          }
        }
        return res;
      }

      template< class Generator > result_type operator()( Generator& g, const param_type& params );

      param_type param() const noexcept { return {a, b}; }

      void param( const param_type& params ) noexcept
      {
        a = params.a;
        b = params.b;
      }

      result_type min()  const noexcept { return a; };
      result_type max()  const noexcept { return b; };

    private:

      T a;
      T b;
      int nb;
      std::uniform_real_distribution<float> sd;
      std::uniform_int_distribution<int> ird;
      std::uniform_int_distribution<int> ird2;
    };
  }

  template<typename T>
  struct realistic_distribution : tts::detail::integral_realistic_distribution<T>
  {
    using parent = tts::detail::integral_realistic_distribution<T>;
    using parent::parent;
  };

  template<typename T>
  requires(std::is_floating_point_v<T>)
  struct realistic_distribution<T>  : tts::detail::fp_realistic_distribution<T>
  {
    using parent = tts::detail::fp_realistic_distribution<T>;
    using parent::parent;
  };
}
