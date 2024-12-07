//======================================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once
#include <tts/tools/typename.hpp>
#include <cstdint>

namespace tts
{
  //====================================================================================================================
  //! @brief Type list structure
  //!
  //! Encapsulates a variadic list of types to be processed by scenarios.
  //!
  //! @tparam Ts List of types to embed into a type list
  //====================================================================================================================
  template<typename... Ts>
  struct types
  {
    template<typename... Us> constexpr types<Ts...,Us...> operator+( types<Us...> const&) const;
  };

  /// Concatenate types lists
  template<typename... Ls> using concatenate = decltype( (Ls{} + ...) );

  //====================================================================================================================
  //! @brief Type wrapper
  //!
  //! Encapsulates a type to be processed by scenarios.
  //!
  //! @tparam T Type to embed
  //====================================================================================================================
  template<typename T> struct type
  {
    friend text as_text(type) { return as_text(typename_<T>); }

    template<_::stream OS>
    friend OS& operator<<(OS& os, type const&)
    {
      return os << typename_<T>;
    }
  };

  // Reusable pre-made types lists

  /// List of all standard floating point types.
  using real_types        = types < double,float>;

  /// List of all standard signed integer types.
  using int_types         = types < std::int64_t , std::int32_t , std::int16_t , std::int8_t>;

  /// List of all standard unsigned types.
  using uint_types        = types < std::uint64_t , std::uint32_t , std::uint16_t , std::uint8_t>;

  /// List of all standard integer types.
  using integral_types    = types < std::int64_t  , std::int32_t  , std::int16_t  , std::int8_t
                                  , std::uint64_t , std::uint32_t , std::uint16_t , std::uint8_t
                                  >;

  /// List of all standard signed types.
  using signed_types      = types < double,float
                                  , std::int64_t , std::int32_t , std::int16_t , std::int8_t
                                  >;
  /// List of all standard arithmetic types.
  using arithmetic_types  = types < double,float
                                  , std::int64_t  , std::int32_t  , std::int16_t  , std::int8_t
                                  , std::uint64_t , std::uint32_t , std::uint16_t , std::uint8_t
                                  >;
}
