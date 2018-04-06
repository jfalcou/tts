//==================================================================================================
/*!
  @file

  Defines the basic ulpdist function

  @copyright 2018 Joel FALCOU


  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

**/
//==================================================================================================
#ifndef TTS_TESTS_IMPL_ULPDIST_HPP_INCLUDED
#define TTS_TESTS_IMPL_ULPDIST_HPP_INCLUDED

#include <tts/tests/impl/traits.hpp>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <cmath>

namespace tts
{
  namespace ext
  {
    template< typename T1, typename T2 = T1, typename EnableIF = void> struct ulpdist;
  }

  template<typename T1, typename T2>
  struct support_ulpdist
  {
    template<typename U, typename V>
    static    auto test(int)
          ->  decltype( ext::ulpdist<detail::common_t<U,V>>()
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

      @brief User extension point for ULP computation
    **/
    template<typename T1, typename T2, typename EnableIF>
    struct ulpdist
    {
      inline double operator()(T1 const& a, T2 const& b) const
      {
        static_assert ( support_ulpdist<T1,T2>::type::value
                      , "Missing ulpdist specialisation for current types"
                      );

        using common_t = detail::common_t<T1,T2>;
        return ext::ulpdist<common_t>() ( static_cast<common_t>(a)
                                        , static_cast<common_t>(b)
                                        );
      }
    };

    // Overload for booleans
    template< typename T>
    struct ulpdist<T,T,typename std::enable_if<std::is_same<T,bool>::value>::type>
    {
      inline double operator()(T a, T b) const
      {
        return a == b ? 0. : 1.;
      }
    };

    // Overload for reals
    template<typename T>
    struct ulpdist< T, T
                  , typename std::enable_if<std::is_floating_point<T>::value>::type
                  >
    {
      inline double operator()(T a, T b) const
      {
        if( (a==b) || ((a!=a) && (b!=b)) )  return 0.;
        if( (a!=a) || (b!=b) )              return std::numeric_limits<T>::infinity();

        int e1 = 0,e2 = 0;
        T   m1,m2;
        m1 = std::frexp(a, &e1);
        m2 = std::frexp(b, &e2);

        int expo = -std::max(e1, e2);

        T e = (e1 == e2)  ? std::abs(m1-m2)
                          : std::abs(std::ldexp(a, expo)- std::ldexp(b, expo));

        return double(e/std::numeric_limits<T>::epsilon());
      }
    };

    // Overload for integers
    template<typename T>
    struct ulpdist< T, T
                  , typename std::enable_if <   std::is_integral<T>::value
                                            &&  !std::is_same<T,bool>::value
                                            >::type
                  >
    {
      inline double operator()(T a, T b) const
      {
        using u_t = typename std::make_unsigned<T>::type;
        return static_cast<double>( (a<b) ? u_t(b-a) : u_t(a-b) );
      }
    };
  }

  /*!
    @ingroup group-details

    @brief Distance in ULPs between values

    Computes the ULP distance between two values.

    @par Semantic:

    For any types @c T and @c U, any value @c a0 of type @c T and any value @c a1 of type @c U:

    @code
    auto r = ulpdist(a0,a1);
    @endcode

    is equivalent to:

    @code
    auto r = ulpdist( static_cast<std::common_t<T,U>>(a0), static_cast<std::common_t<T,U>>(a1));
    @endcode

    For any type @c T and any value of type @c T @c a0 and @c a1, then:

    @code
    auto r = ulpdist(a0,a1);
    @endcode

    is equivalent to:

    - if @c T is @c bool :
      @code
      auto r = a0==a1 ? 0. : 1.;
      @endcode

    - if @c T is an integral type:
      @code
      auto r = double( (a0>a1) ? a0-a1 : a1-a0 );
      @endcode
      i.e the number of different bits between @c a0 and @c a1

    - if @c T is a floating-point type, @c r is equal to the number of different bits between the
      normalized representation of @c a0 and @c a1 as defined by Golderg et al.

    @par Customization

    Behavior of ulpdist on user defined types can be done by overloading the SFINAE-aware
    ext::ulpdist class.

    @param a0 First value to compare
    @param a1 Second value to compare
    @return The distance in ULP between a0 and a1
  **/
  template<typename T, typename U> inline double ulpdist(T const& a0, U const& a1)
  {
    return ext::ulpdist<T,U>()(a0,a1);
  }
}

#endif
