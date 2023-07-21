//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <tts/tools/block.hpp>
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <type_traits>

namespace tts::detail
{
  template<typename T>
  struct fp_dist
  {
    using result_type = T;
    struct param_type
    {
      param_type( T pa = 0, T pb = 1
                , std::size_t sz = 65536
                , T mnp = std::numeric_limits<T>::epsilon()
                , T mxp = 1./std::numeric_limits<T>::epsilon()
                )
                : a(pa), b(pb), n(sz), minpos(mnp), maxpos(mxp)
      {
        if(b<a) std::swap(a,b);

        if(a < -maxpos)             a = -maxpos;
        else if(a>0 && a < minpos)  a =  minpos;
        if(b > maxpos)              b =  maxpos;
        else if(b<0 && b < -minpos) b = -minpos;
      }

      T a, b;
      std::size_t n;
      T minpos, maxpos;
    };

    fp_dist()                                     noexcept : fp_dist(param_type{}) {}
    fp_dist(T a, T b)                             noexcept : fp_dist(param_type{a,b}) {}
    fp_dist(T a, T b, std::size_t n)              noexcept : fp_dist(param_type{a,b,n}) {}
    fp_dist(T a, T b, std::size_t n, T mn )       noexcept : fp_dist(param_type{a,b,n,mn}) {}
    fp_dist(T a, T b, std::size_t n, T mn, T mx)  noexcept : fp_dist(param_type{a,b,n,mn,mx}) {}
    fp_dist(param_type const& pr)                 noexcept { param(pr); }

    param_type const& param() const { return params; }

    void param(param_type const& p)
    {
      params = p;
      find_limits();
      find_indexes(static_cast<std::size_t>(1+params.minpos/2));
      selector.param( std::uniform_int_distribution<std::size_t>::param_type(0, size()-1));
    }

    auto size() const noexcept  { return sizes.back(); }
    auto min()  const noexcept  { return params.a; }
    auto max()  const noexcept  { return params.b; }

    template< class Generator > result_type operator()( Generator& gen )
    {
      auto p  = selector(gen);
      auto it = std::upper_bound(sizes.begin(), sizes.end(), p) - 1;
      auto i  = std::distance(sizes.begin(),it);
      return generate(limits[i],limits[i+1],p - *it,params.n);
    }

    tts::detail::block<T,7>                     limits;
    tts::detail::block<std::size_t,7>           sizes;
    std::uniform_int_distribution<std::size_t>  selector;
    param_type                                  params;

    private:
    void find_limits() noexcept
    {
      limits.clear();
      std::array<T,7> zs{-params.maxpos,-1,-params.minpos,0,params.minpos,1,params.maxpos};

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

   static T generate(T va, T vb, std::size_t p, std::size_t n) noexcept
    {
      const auto eval = [](double x, double y, double i, double sz) -> double
      {
        if(x<1) return 1./std::exp2(std::lerp(std::log2(1./y), std::log2(1./x), i/sz));
        else    return    std::exp2(std::lerp(std::log2(x)   , std::log2(y)   , i/sz));
      };

      if(va==0 || vb==0)  return 0.;
      auto f = va<0 ? -1 : 1;
      return static_cast<T>(f * eval(f * va, f * vb,p,n-1));
    }
  };

  template<typename T>
  struct  char_dist
       : std::uniform_int_distribution < std::conditional_t< std::is_signed_v<T>
                                                            , short
                                                            , unsigned short
                                        > >
  {
    using parent = std::uniform_int_distribution< std::conditional_t<std::is_signed_v<T>
                                                                    , short
                                                                    , unsigned short
                                                                    >
                                                >;
    using result_type = T;
    using parent::parent;

    template< class Generator > result_type operator()( Generator& gen )
    {
      return static_cast<result_type>(parent::operator()(gen));
    }
  };

  template<typename T>
  struct choose_distribution;

  template<tts::integral T>
  requires(sizeof(T) > 1)
  struct choose_distribution<T>
  {
    using type = std::uniform_int_distribution<T>;
  };

  template<tts::integral T>
  requires(sizeof(T) == 1)
  struct choose_distribution<T>
  {
    using type = char_dist<T>;
  };

  template<tts::floating_point T>
  struct choose_distribution<T>
  {
    using type = fp_dist<T>;
  };
}

namespace tts
{
  //====================================================================================================================
  /*! @brief Realistic standard random distribution

    Provides a standard compatible random distribution that works for any arithmetic types T and have a "realistic"
    distribution, i.e where most natural and expected values are favored.
  **/
  //====================================================================================================================
  template<typename T>
  using realistic_distribution = typename detail::choose_distribution<T>::type;
}
