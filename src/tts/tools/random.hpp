//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <tts/tools/block.hpp>
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <type_traits>

namespace tts
{
  template<typename T>
  struct realistic_distribution
  {
    using result_type = T;
    struct param_type
    {
      param_type( T pa = 0, T pb = 1
                , std::size_t sz = 65536
                , T mnp = 10
                , T mxp = std::numeric_limits<T>::max()
                )
                : a(pa), b(pb), n(sz), minpos(mnp), maxpos(mxp)
      {
        if(b<a) std::swap(a,b);

        if constexpr(std::is_signed_v<T>)
        {
          if(a < -maxpos)             a = -maxpos;
          else if(a>0 && a < minpos)  a =  minpos;
          if(b > maxpos)              b =  maxpos;
          else if(b<0 && b < -minpos) b = -minpos;
        }
        else
        {
          if(a>0 && a < minpos)  a =  minpos;
          if(b > maxpos)         b =  maxpos;
        }
      }

      T a, b;
      std::size_t n;
      T minpos, maxpos;
    };

    realistic_distribution() noexcept : realistic_distribution(param_type{})
    {}

    realistic_distribution(T a, T b) noexcept : realistic_distribution(param_type{a,b})
    {}

    realistic_distribution(T a, T b, std::size_t n) noexcept : realistic_distribution(param_type{a,b,n})
    {}

    realistic_distribution(T a, T b, std::size_t n, T mn ) noexcept : realistic_distribution(param_type{a,b,n,mn})
    {}

    realistic_distribution(T a, T b, std::size_t n, T mn, T mx) noexcept : realistic_distribution(param_type{a,b,n,mn,mx})
    {}


    realistic_distribution(param_type const& pr) noexcept
    {
      param(pr);
    }

    param_type const& param() const { return params; }

    void param(param_type const& p)
    {
      params = p;
      find_limits();
      find_indexes(1+params.minpos/2);
      selector.param( std::uniform_int_distribution<std::size_t>::param_type(0, size()));
    }

    auto size() const noexcept  { return sizes.back(); }
    auto min()  const noexcept  { return params.a; }
    auto max()  const noexcept  { return params.b; }

    template< class Generator > result_type operator()( Generator& gen )
    {
      auto p  = selector(gen);
      auto it = std::upper_bound(sizes.begin(), sizes.end(), p) - 1;
      auto i  = std::distance(sizes.begin(),it);
      return generate(limits[i],limits[i+1],p - *it,params.n,params.minpos);
    }

    result_type operator()( int p )
    {
      auto it = std::upper_bound(sizes.begin(), sizes.end(), p) - 1;
      auto i  = std::distance(sizes.begin(),it);
      return generate(limits[i],limits[i+1],p - *it,params.n,params.minpos);
    }

    tts::detail::block<T,7>                     limits;
    tts::detail::block<std::size_t,7>           sizes;
    std::uniform_int_distribution<std::size_t>  selector;
    param_type                                  params;

    private:
    void find_limits() noexcept
    {
      limits.clear();

      auto zs = [&]()
      {
        if constexpr( std::floating_point<T>)
          return std::array<T,7>{-params.maxpos,-1,-params.minpos,0,params.minpos,1,params.maxpos};
        else if constexpr(std::is_signed_v<T>)
          return std::array<T,5>{T(-params.maxpos),T(-params.minpos),T(0),params.minpos,params.maxpos};
        else
          return std::array<T,3>{ T(0),params.minpos,params.maxpos};
      }();

      for(auto z : zs)
      {
        if(z>=params.a && z<=params.b) limits.push_back(z);
      }

      if(limits.empty()) { limits.push_back(params.a); limits.push_back(params.b);}
      else
      {
        if(limits.front() > params.a)  { limits.push_front(params.a); }
        if(limits.back()  < params.b)  { limits.push_back(params.b);  }
      }
    }

    void find_indexes(std::size_t nbzero) noexcept
    {
      sizes.resize(limits.size());
      std::size_t t = 0;
      sizes[0]    = 0;

      for(std::size_t i=1;i<limits.size()-1;++i)
      {
        t += (limits[i] == 0 || limits[i-1] == 0)  ? nbzero : params.n;
        sizes[i] = t;
      }

      sizes.back() = t + (limits.back() == 0 ? nbzero : params.n);
    }

   static T generate(T va, T vb, std::size_t p, std::size_t n, T minpos) noexcept
    {
      if constexpr( std::integral<T>)
      {
        const auto eval = [](double x, double y, double i, double sz) -> double
        {
          if(x>=1)  return std::exp2(std::lerp(std::log2(x), std::log2(y)   , i/sz));
          else      return 0.;
        };

        if(va==0 || vb==0)  return static_cast<T>(p*minpos/(1.+minpos/2));
        auto f = va<0 ? -1 : 1;
        return std::round(f * eval(f * va, f * vb,p,n-1));
      }
      else
      {
        const auto eval = [](double x, double y, double i, double sz) -> double
        {
          if(x<1) return 1./std::exp2(std::lerp(std::log2(1./y), std::log2(1./x), i/sz));
          else    return    std::exp2(std::lerp(std::log2(x)   , std::log2(y)   , i/sz));
        };

        if(va==0 || vb==0)  return 0.;
        auto f = va<0 ? -1 : 1;
        return f * eval(f * va, f * vb,p,n-1);
      }
    }
  };
}
