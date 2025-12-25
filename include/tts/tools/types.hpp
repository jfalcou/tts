//======================================================================================================================
//! @file
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once
#include <tts/tools/typename.hpp>

namespace tts
{
  //====================================================================================================================
  /**
    @addtogroup tools-types
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /// Encapsulates a variadic list of types into a reusable object
  //====================================================================================================================
  template<typename... Ts>
  struct types
  {
    template<typename... Us> constexpr types<Ts...,Us...> operator+( types<Us...> const&) const;
  };

  // Concatenate types lists
  template<typename... Ls> using concatenate = decltype( (Ls{} + ...) );

  template<typename... T> struct as_type_list
  {
    using type = types<T...>;
  };

  template<typename... T>
  struct as_type_list<types<T...>>
  {
    using type = types<T...>;
  };

  template<typename T>
  requires requires(T) { typename T::types_list; }
  struct as_type_list<T>
  {
    using type = typename T::types_list;
  };

  template<typename... T>
  using as_type_list_t = typename as_type_list<T...>::type;

  //====================================================================================================================
  /// @brief Encapsulates a single type into a reusable type object
  //====================================================================================================================
  template<typename T> struct type
  {
    friend text to_text(type) { return as_text(typename_<T>); }

    template<_::stream OS>
    friend OS& operator<<(OS& os, type const&)
    {
      return os << typename_<T>;
    }
  };

  /// Types list containing all standard floating point types.
  using real_types        = types < double,float>;

  /// Types list containing all standard signed integer types.
  using int_types         = types < std::int64_t , std::int32_t , std::int16_t , std::int8_t>;

  /// Types list containing all standard unsigned types.
  using uint_types        = types < std::uint64_t , std::uint32_t , std::uint16_t , std::uint8_t>;

  /// Types list containing all standard integer types.
  using integral_types    = types < std::int64_t  , std::int32_t  , std::int16_t  , std::int8_t
                                  , std::uint64_t , std::uint32_t , std::uint16_t , std::uint8_t
                                  >;

  /// Types list containing all standard signed types.
  using signed_types      = types < double,float
                                  , std::int64_t , std::int32_t , std::int16_t , std::int8_t
                                  >;

  /// Types list containing all standard arithmetic types.
  using arithmetic_types  = types < double,float
                                  , std::int64_t  , std::int32_t  , std::int16_t  , std::int8_t
                                  , std::uint64_t , std::uint32_t , std::uint16_t , std::uint8_t
                                  >;

  //====================================================================================================================
  /// @}
  //====================================================================================================================
}
