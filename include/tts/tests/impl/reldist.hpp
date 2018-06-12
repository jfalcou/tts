//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_IMPL_RELDIST_HPP_INCLUDED
#define TTS_TESTS_IMPL_RELDIST_HPP_INCLUDED

#include <tts/tests/impl/traits.hpp>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <cmath>

namespace tts
{
  namespace ext
  {
    template< typename T1, typename T2 = T1, typename EnableIF = void> struct reldist;
  }

  template<typename T1, typename T2>
  struct support_reldist
  {
    template<typename U, typename V>
    static    auto test(int)
          ->  decltype( ext::reldist<detail::common_t<U,V>>()
                                      ( static_cast<detail::common_t<U,V>>(std::declval<U>())
                                      , static_cast<detail::common_t<U,V>>(std::declval<V>())
                                      )
                        , std::true_type {}
                      );

    template<typename,typename>
    static std::false_type test(...);

    using type = decltype(test<T1,T2>(0));
  };

  namespace ext
  {
    /*!
      @ingroup group-details

      @brief User extension point for relative precision computation
    **/
    template< typename T1, typename T2
            , typename EnableIF
            >
    struct reldist
    {
      inline double operator()(T1 const& a, T2 const& b) const
      {
        static_assert ( support_reldist<T1,T2>::type::value
                      , "Missing reldist specialisation for current types"
                      );

        using common_t = detail::common_t<T1,T2>;
        return ext::reldist<common_t>() ( static_cast<common_t>(a)
                                        , static_cast<common_t>(b)
                                        );
      }
    };

    // Overload for booleans
    template< typename T>
    struct reldist<T,T,typename std::enable_if<std::is_same<T,bool>::value>::type>
    {
      inline double operator()(T a, T b) const
      {
        return a == b ? 0. : 1.;
      }
    };

    // Overload for reals
    template<typename T>
    struct reldist< T, T
                  , typename std::enable_if<std::is_floating_point<T>::value>::type
                  >
    {
      inline double operator()(T a, T b) const
      {
        auto inf_ = std::numeric_limits<T>::infinity();
        auto aa   = std::abs(a);
        auto ab   = std::abs(b);

        if( (a == b  ) || ((a != a) && (b!=b)) )  return 0.;
        if( (a != a  ) || (b  != b) )             return inf_;
        if( (aa==inf_) || (ab == inf_) )          return inf_;

        return std::abs(a-b) / std::max(T(1), std::max(aa,ab));
      }
    };

    // Overload for integers
    template<typename T>
    struct reldist< T, T
                  , typename std::enable_if <   std::is_integral<T>::value
                                            &&  !std::is_same<T,bool>::value
                                            >::type
                  >
    {
      inline double operator()(T a, T b) const
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return reldist<double>()(d0,d1);
      }
    };
  }

  /*!
    @ingroup group-details

    @brief Relative distance between values

    Computes the relative distance between two values.

    @par Semantic:

    For any types @c T and @c U, any value @c a0 of type @c T and any value @c a1 of type @c U:

    @code
    auto r = reldist(a0,a1);
    @endcode

    is equivalent to:

    @code
    auto r = reldist( static_cast<std::common_t<T,U>>(a0), static_cast<std::common_t<T,U>>(a1));
    @endcode

    For any type @c T and any value of type @c T @c a0 and @c a1, then:

    @code
    auto r = reldist(a0,a1);
    @endcode

    is equivalent to:

    - if @c T is @c bool :
      @code
      auto r = a0==a1 ? 0. : 1.;
      @endcode

    @param a0 First value to compare
    @param a1 Second value to compare
    @return The relative distance between a0 and a1
  **/
  template<typename T, typename U> inline double reldist(T const& a0, U const& a1)
  {
    return ext::reldist<T,U>()(a0,a1);
  }
}

#endif
