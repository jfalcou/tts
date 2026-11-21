//======================================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace tts::_
{
  // The unsigned integer of N bytes, void for a size no integer has.
  template<std::size_t N>
  using sized_unsigned_t = std::conditional_t<
  N == 1,
  std::uint8_t,
  std::conditional_t<
  N == 2,
  std::uint16_t,
  std::conditional_t<N == 4, std::uint32_t, std::conditional_t<N == 8, std::uint64_t, void>>>>;

  // The integer of the size of T, signed when Signed is.
  template<typename T, bool Signed = false>
  using sized_integer_t = std::conditional_t<Signed,
                                             std::make_signed_t<sized_unsigned_t<sizeof(T)>>,
                                             sized_unsigned_t<sizeof(T)>>;
}

namespace tts
{
  //====================================================================================================================
  /// @addtogroup tools-generators-custom
  /// @{
  //====================================================================================================================

  //====================================================================================================================
  /**
    @brief Enumerates the values of a type in increasing order

    Gives each value of `T` its rank, its position among the values of `T` in increasing order.
    For `std::int8_t`, -128 has rank 0 and 127 has rank 255. The specialization for `T` has three
    static members:

    - `last`, the largest rank, a `std::uint64_t`. The number of values, `last + 1`, does not fit in
      64 bits for a 64-bit integer type.
    - `index(v)`, the rank of `v`, a `std::uint64_t`.
    - `value(i)`, the value of rank `i`, for `i` from 0 to `last`.

    Specialized for every integer type except `bool`, and for `float` and `double`. The ranks of
    `float` and `double` follow the totalOrder of IEEE 754-2019 without NaN. -inf has rank 0, -0
    precedes +0, and +inf has rank `last`. NaN has no rank, and `index` must not receive a NaN.

    @groupheader{Example}
    @snippet doc/enumeration.cpp snippet

    @tparam T Type whose values are enumerated
  **/
  //====================================================================================================================
  template<typename T> struct enumeration;

  template<std::integral T>
    requires(!std::same_as<T, bool>)
  struct enumeration<T>
  {
  private:
    using u_t = _::sized_integer_t<T>;

    // The sign bit flipped puts the negative values first.
    static constexpr u_t flip = std::is_signed_v<T> ? u_t(u_t(~u_t(0)) / 2 + 1) : u_t(0);

  public:
    static constexpr std::uint64_t last = u_t(~u_t(0));

    static constexpr std::uint64_t index(T v) noexcept
    {
      return u_t(u_t(v) ^ flip);
    }
    static constexpr T value(std::uint64_t i) noexcept
    {
      return T(u_t(u_t(i) ^ flip));
    }
  };

  template<std::floating_point T>
    requires(sizeof(T) == 4 || sizeof(T) == 8)
  struct enumeration<T>
  {
  private:
    using u_t = _::sized_integer_t<T>;

    // -inf has rank 0, -0 rank inf, +0 rank inf + 1, +inf rank last.
    static constexpr u_t inf  = std::bit_cast<u_t>(std::numeric_limits<T>::infinity());
    static constexpr u_t sign = u_t(u_t(~u_t(0)) / 2 + 1);

  public:
    static constexpr std::uint64_t last = 2 * std::uint64_t(inf) + 1;

    static constexpr std::uint64_t index(T v) noexcept
    {
      auto b = std::bit_cast<u_t>(v);
      return b & sign ? inf - (b ^ sign) : inf + 1 + std::uint64_t(b);
    }

    static constexpr T value(std::uint64_t i) noexcept
    {
      return std::bit_cast<T>(u_t(i <= inf ? sign | (inf - i) : i - inf - 1));
    }
  };

  //====================================================================================================================
  /// @}
  //====================================================================================================================
}
