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

namespace tts::_
{
  //====================================================================================================================
  // How TTS measures a distance when nothing more specific is known. Pulled into a base so that a
  // specialization of tts::precision only has to redefine what it actually changes.
  //====================================================================================================================
  template<typename T> struct builtin_precision
  {
    static double absolute(T const& a, T const& b)
    {
      static_assert(
      !requires { absolute_distance(a, b); },
      "[TTS] tts::absolute_distance is no longer a customization point. "
      "Specialize tts::precision<T>::absolute instead.");

      if constexpr(std::is_same_v<T, bool>) return a == b ? 0. : 1.;
      else if constexpr(std::is_floating_point_v<T>)
      {
        if((a == b) || (is_nan(a) && is_nan(b))) return 0.;
        if(is_inf(a) || is_inf(b) || is_nan(a) || is_nan(b))
          return std::numeric_limits<double>::infinity();
        return abs(a - b);
      }
      else if constexpr(std::is_integral_v<T>)
        return builtin_precision<double>::absolute(static_cast<double>(a), static_cast<double>(b));
      else
      {
        static_assert(
        std::is_floating_point_v<T> || std::is_integral_v<T>,
        "[TTS] TTS_ABSOLUTE_EQUAL requires integral or floating points data to compare. "
        "Did you mean to use TTS_ALL_ABSOLUTE_EQUAL or to specialize "
        "tts::precision<T>::absolute ?");
        return 0.;
      }
    }

    static double relative(T const& a, T const& b)
    {
      static_assert(
      !requires { relative_distance(a, b); },
      "[TTS] tts::relative_distance is no longer a customization point. "
      "Specialize tts::precision<T>::relative instead.");

      if constexpr(std::is_same_v<T, bool>) return a == b ? 0. : 1.;
      else if constexpr(std::is_floating_point_v<T>)
      {
        if((a == b) || (is_nan(a) && is_nan(b))) return 0.;
        if(is_inf(a) || is_inf(b) || is_nan(a) || is_nan(b))
          return std::numeric_limits<double>::infinity();
        return abs(a - b) / max(T(1), max(abs(a), abs(b)));
      }
      else if constexpr(std::is_integral_v<T>)
        return builtin_precision<double>::relative(static_cast<double>(a), static_cast<double>(b));
      else
      {
        static_assert(
        std::is_floating_point_v<T> || std::is_integral_v<T>,
        "[TTS] TTS_RELATIVE_EQUAL requires integral or floating points data to compare. "
        "Did you mean to use TTS_ALL_RELATIVE_EQUAL or to specialize "
        "tts::precision<T>::relative ?");
        return 0.;
      }
    }

    static double ulp(T const& a, T const& b)
    {
      static_assert(
      !requires { ulp_distance(a, b); },
      "[TTS] tts::ulp_distance is no longer a customization point. "
      "Specialize tts::precision<T>::ulp instead.");

      if constexpr(std::is_same_v<T, bool>)
        return a == b ? 0. : std::numeric_limits<double>::infinity();
      else if constexpr(std::is_floating_point_v<T>)
      {
        using ui_t = std::conditional_t<std::is_same_v<T, float>, std::uint32_t, std::uint64_t>;
        if((a == b) || (is_nan(a) && is_nan(b))) return 0.;
        else if(is_unordered(a, b)) return std::numeric_limits<double>::infinity();
        else
        {
          auto aa = bitinteger(a);
          auto bb = bitinteger(b);
          if(aa > bb) std::swap(aa, bb);
          auto z = static_cast<ui_t>(bb - aa);
          if(signbit(a) != signbit(b)) ++z;
          return static_cast<double>(z) / 2.;
        }
      }
      else if constexpr(std::is_integral_v<T>)
      {
        using u_t = typename std::make_unsigned<T>::type;
        auto ua   = static_cast<u_t>(a);
        auto ub   = static_cast<u_t>(b);
        return static_cast<double>((a < b) ? (ub - ua) : (ua - ub)) / 2.;
      }
      else
      {
        static_assert(std::is_floating_point_v<T> || std::is_integral_v<T>,
                      "[TTS] TTS_ULP_EQUAL requires integral or floating points data to compare. "
                      "Did you mean to use TTS_ALL_ULP_EQUAL or to specialize "
                      "tts::precision<T>::ulp ?");
        return 0.;
      }
    }

    static bool ieee(T const& a, T const& b)
    {
      static_assert(
      !requires { ieee_equal(a, b); },
      "[TTS] tts::ieee_equal is no longer a customization point. "
      "Specialize tts::precision<T>::ieee instead.");

      if constexpr(std::is_floating_point_v<T>) return (a == b) || (is_nan(a) && is_nan(b));
      else return eq(a, b);
    }
  };
}

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
    @brief How far apart two values of type T are, and when they count as IEEE-equal

    Specialize this for your own types rather than overloading the free functions: a specialization
    that does not match is a compilation error, where a misnamed overload used to be ignored in
    silence. Inherit from tts::_::builtin_precision<T> to keep the members you do not redefine.

    The `ulp_distance`, `relative_distance`, `absolute_distance` and `ieee_equal` free functions
    this replaces are gone. An overload left behind is reported where it would have been used,
    rather than ignored in silence.
  **/
  //====================================================================================================================
  template<typename T> struct precision : _::builtin_precision<T>
  {
  };

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
    static_assert(std::is_same_v<T, U>,
                  "[TTS] TTS_ABSOLUTE_EQUAL needs both operands to have the same type. "
                  "Comparing through their common type would express the distance in the unit "
                  "of the promoted type, which is not the one being tested. Convert the "
                  "expected value at the call site instead.");
    if constexpr(std::is_same_v<T, U>) return precision<T>::absolute(a, b);
    else return 0.;
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
    static_assert(std::is_same_v<T, U>,
                  "[TTS] TTS_RELATIVE_EQUAL needs both operands to have the same type. "
                  "Comparing through their common type would express the distance in the unit "
                  "of the promoted type, which is not the one being tested. Convert the "
                  "expected value at the call site instead.");
    if constexpr(std::is_same_v<T, U>) return precision<T>::relative(a, b);
    else return 0.;
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
    static_assert(std::is_same_v<T, U>,
                  "[TTS] TTS_ULP_EQUAL needs both operands to have the same type. "
                  "Comparing through their common type would express the distance in the unit "
                  "of the promoted type, which is not the one being tested. Convert the "
                  "expected value at the call site instead.");
    if constexpr(std::is_same_v<T, U>) return precision<T>::ulp(a, b);
    else return 0.;
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
    // Left permissive on the pair of types, unlike the three distances: this one manufactures no
    // conversion, it ends on eq(a, b). Two different types here are the TTS_EQUAL case, not the
    // precision one, and neither of them can name a single tts::precision to answer.
    if constexpr(std::is_same_v<T, U>) return precision<T>::ieee(a, b);
    else if constexpr(std::is_floating_point_v<T>)
      return (a == b) || (_::is_nan(a) && _::is_nan(b));
    else return _::eq(a, b);
  }
  //====================================================================================================================
  //! @}
  //====================================================================================================================
}
