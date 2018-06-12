//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef TTS_TESTS_IMPL_TRAITS_HPP_INCLUDED
#define TTS_TESTS_IMPL_TRAITS_HPP_INCLUDED

#include <type_traits>

namespace tts { namespace detail
{
  /*!
    @brief SFINAE trigger for integral types
    @ingroup group-details

    This template alias is to be used in SFINAE context to discriminate function dealing
    with integral type only.

    @tparam T Type to check as a integral type
    @tparam R Return type of the function to use if substitution succeed
  **/
  template<typename T, typename R>
  using if_integral = typename std::enable_if<std::is_integral<T>::value, R>::type;

  /*!
    @ingroup group-details

    @brief SFINAE trigger for floating-point types

    This template alias is to be used in SFINAE context to discriminate function dealing
    with floating-point type only.

    @tparam T Type to check as a floating-point type
    @tparam R Return type of the function to use if substitution succeed
  **/
  template<typename T, typename R>
  using if_real = typename std::enable_if<std::is_floating_point<T>::value, R>::type;

  // Short-cut for std::common_type
  template<typename T, typename U>
  using common_t = typename std::common_type<T,U>::type;
} }

#endif
