//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once
#include <tts/tools/bitcast.hpp>
#include <tts/engine/math.hpp>

namespace tts
{
  //====================================================================================================================
  /**
    @defgroup tools-precision Precision Utilities
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /*!
    @ingroup tools-precision
    @public
    @brief Compute the absolute distance between two values

    @note This functions is meant to help implementing @ref TTS_ABSOLUTE_EQUAL support for custom
  types. For testing purposes, use @ref TTS_ABSOLUTE_EQUAL directly. See @ref customize-precision
  for more details.

    @param  a Value to compare
    @param  b Value to compare
    @return The distance between a and b
  **/
  //====================================================================================================================
  template<typename T, typename U> inline double absolute_check(T const& a, U const& b)
  {
    if constexpr(requires { absolute_distance(a, b); }) return absolute_distance(a, b);
    else if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>) // Boolean case
      {
        return a == b ? 0. : 1.;
      }
      else if constexpr(std::is_floating_point_v<T>) // IEEE cases
      {
        if((a == b) || (_::is_nan(a) && _::is_nan(b))) return 0.;

        if(_::is_inf(a) || _::is_inf(b) || _::is_nan(a) || _::is_nan(b))
          return std::numeric_limits<double>::infinity();

        return _::abs(a - b);
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>) // Natural case
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return absolute_check(d0, d1);
      }
      else
      {
        static_assert(
        std::is_floating_point_v<T> || std::is_integral_v<T>,
        "[TTS] TTS_ABSOLUTE_EQUAL requires integral or floating points data to compare."
        "Did you mean to use TTS_ALL_ABSOLUTE_EQUAL or to overload tts::absolute_check ?");
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return absolute_check(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }

  //====================================================================================================================
  /*!
    @ingroup tools-precision
    @public
    @brief Compute the relative distance between two values

    @note This functions is meant to help implementing @ref TTS_RELATIVE_EQUAL support for custom
  types. For testing purposes, use @ref TTS_RELATIVE_EQUAL directly. See @ref customize-precision
  for more details.

    @param  a Value to compare
    @param  b Value to compare
    @return The relative distance between a and b
  **/
  //====================================================================================================================
  template<typename T, typename U> inline double relative_check(T const& a, U const& b)
  {
    if constexpr(requires { relative_distance(a, b); }) return relative_distance(a, b);
    else if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>) // Boolean case
      {
        return a == b ? 0. : 100.;
      }
      else if constexpr(std::is_floating_point_v<T>) // IEEE cases
      {
        if((a == b) || (_::is_nan(a) && _::is_nan(a))) return 0.;

        if(_::is_inf(a) || _::is_inf(b) || _::is_nan(a) || _::is_nan(b))
          return std::numeric_limits<double>::infinity();

        return 100. * (_::abs(a - b) / _::max(T(1), _::max(_::abs(a), _::abs(b))));
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>) // Natural case
      {
        auto d0 = static_cast<double>(a), d1 = static_cast<double>(b);
        return relative_check(d0, d1);
      }
      else
      {
        static_assert(
        std::is_floating_point_v<T> || std::is_integral_v<T>,
        "[TTS] TTS_RELATIVE_EQUAL requires integral or floating points data to compare."
        "Did you mean to use TTS_ALL_RELATIVE_EQUAL or to overload tts::relative_check ?");
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return relative_check(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }

  //====================================================================================================================
  /*!
    @ingroup tools-precision
    @public
    @brief Compute the distance in ULP between two values

    @note This functions is meant to help implementing @ref TTS_ULP_EQUAL support for custom types.
          For testing purposes, use @ref TTS_ULP_EQUAL directly. See @ref customize-precision for
  more details.

    @param  a Value to compare
    @param  b Value to compare
    @return The distance in ULP  between a and b
  **/
  //====================================================================================================================
  template<typename T, typename U> inline double ulp_check(T const& a, U const& b)
  {
    if constexpr(requires { ulp_distance(a, b); }) return ulp_distance(a, b);
    else if constexpr(std::is_same_v<T, U>)
    {
      if constexpr(std::is_same_v<T, bool>) // Boolean case
      {
        return a == b ? 0. : std::numeric_limits<double>::infinity();
      }
      else if constexpr(std::is_floating_point_v<T>) // IEEE cases
      {
        using ui_t = std::conditional_t<std::is_same_v<T, float>, std::uint32_t, std::uint64_t>;

        if((a == b) || (_::is_nan(a) && _::is_nan(b))) { return 0.; }
        else if(_::is_unordered(a, b)) { return std::numeric_limits<double>::infinity(); }
        else
        {
          auto aa = _::bitinteger(a);
          auto bb = _::bitinteger(b);

          if(aa > bb) std::swap(aa, bb);

          auto z = static_cast<ui_t>(bb - aa);

          if(_::signbit(a) != _::signbit(b)) ++z;
          return z / 2.;
        }
      }
      else if constexpr(std::is_integral_v<T> && !std::is_same_v<T, bool>) // Natural case
      {
        using u_t = typename std::make_unsigned<T>::type;

        // TODO: Fix overflow in case of very huge integral value
        return ((a < b) ? u_t(b - a) : u_t(a - b)) / 2.;
      }
      else
      {
        static_assert(std::is_floating_point_v<T> || std::is_integral_v<T>,
                      "[TTS] TTS_ULP_EQUAL requires integral or floating points data to compare."
                      "Did you mean to use TTS_ALL_ULP_EQUAL or to overload tts::ulp_check ?");
      }
    }
    else
    {
      using common_t = std::common_type_t<T, U>;
      return ulp_check(static_cast<common_t>(a), static_cast<common_t>(b));
    }
  }

  //====================================================================================================================
  /*!
    @ingroup tools-precision
    @public
    @brief Compute if values are exactly equals or all NaNs/Invalids

    @note This functions is meant to help implementing @ref TTS_IEEE_EQUAL support for custom types.
          For testing purposes, use @ref TTS_IEEE_EQUAL directly. See @ref customize-precision for
  more details.

    @param  a Value to compare
    @param  b Value to compare
    @return Is `a == b` or `_::is_nan(a) && _::is_nan(b)`
  **/
  //====================================================================================================================
  template<typename T, typename U> inline bool ieee_check(T const& a, U const& b)
  {
    if constexpr(requires { ieee_equal(a, b); }) return ieee_equal(a, b);
    else if constexpr(std::is_floating_point_v<T>) // IEEE cases
    {
      return (a == b) || (_::is_nan(a) && _::is_nan(b));
    }
    else { return _::eq(a, b); }
  }
  //====================================================================================================================
  //! @}
  //====================================================================================================================
}
