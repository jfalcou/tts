//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_DETAIL_CT_HELPERS_HPP_INCLUDED
#define TTS_DETAIL_CT_HELPERS_HPP_INCLUDED

#include <type_traits>
#include <iostream>
#include <cstddef>

namespace tts { namespace detail
{
  // Check if T is streamable
  template<typename T> struct is_streamable_impl
  {
    template<typename>   static auto test( ... ) -> std::false_type;
    template<typename U> static auto test( int ) -> decltype( std::cout << std::declval<U>()
                                                            , std::true_type()
                                                            );
    using type = std::is_same<decltype(test<T>(0)),std::true_type>;
  };

  template<typename T> struct is_streamable : is_streamable_impl<T>::type {};
  template<typename T> using is_streamable_t                  = typename is_streamable<T>::type;
  template<typename T> inline constexpr bool is_streamable_v  = is_streamable<T>::value;

  // Check if T is a container
  template<typename T> struct is_container_impl
  {
    template<typename>    static auto test( ... ) -> std::false_type;
    template<typename U>  static auto test( int ) -> decltype ( std::declval<U>().begin()
                                                              , std::declval<U>().end()
                                                              , std::declval<U>().size()
                                                              , std::true_type()
                                                              );

    using type = std::is_same<decltype(test<T>(0)),std::true_type>;
  };

  template<typename T> struct is_container : is_container_impl<T>::type {};
  template<typename T> using is_container_t                  = typename is_container<T>::type;
  template<typename T> inline constexpr bool is_container_v  = is_container<T>::value;

  // Simple types container
  template<typename... Args> struct typelist {};

  // Type box
  template<typename T> struct box { using type = T; };

  // Iterate statically over a typelist
  template<typename Function, typename... Types>
  void for_each_type(Function&& f, typelist<Types...> const&)
  {
    (f(box<Types>{}), ...);
  }
} }

#endif
