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
  // clang-format off
  template<std::size_t N>
  using sized_unsigned_t =
  std::conditional_t<N == 1, std::uint8_t,
  std::conditional_t<N == 2, std::uint16_t,
  std::conditional_t<N == 4, std::uint32_t,
  std::conditional_t<N == 8, std::uint64_t, void>>>>;
  // clang-format on

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
    tts::sweep and tts::sample take the values of an argument from the ranks of its type.

    Specialized for every integer type of at most 8 bytes, and for every floating-point type of 4 or
    8 bytes. A specialization for another type, with `last`, `index`, and `value`, makes that type
    usable with tts::sweep and tts::sample.

    @groupheader{Example}
    @snippet doc/enumeration.cpp snippet

    @tparam T Type whose values are enumerated
  **/
  //====================================================================================================================
#if defined(TTS_DOXYGEN_INVOKED)
  template<typename T> struct enumeration
  {
    /// @brief Largest rank
    static constexpr std::uint64_t last;

    /**
      @brief Returns the rank of a value

      The rank of a value is the number of values of `T` that precede it in increasing order. The
      smallest value of `T` has rank 0, the largest has rank `last`, and the value that follows `v`
      has rank `index(v) + 1`.

      @param v Value of `T`, any but a NaN, which has no rank
      @return The rank of `v`, from 0 to `last`
    **/
    static constexpr std::uint64_t index(T v) noexcept;

    /**
      @brief Returns the value of a rank

      The inverse of `index`: `value(index(v))` is `v`, and `index(value(i))` is `i`. `value(0)` is
      the smallest value of `T`, `value(last)` is the largest, and `value(i + 1)` is the value that
      follows `value(i)`.

      @param i Rank, from 0 to `last`
      @return The value of `T` whose rank is `i`
    **/
    static constexpr T value(std::uint64_t i) noexcept;
  };
#else
  template<typename T> struct enumeration;
#endif

  template<std::integral T>
    requires(!std::same_as<T, bool>)
  struct enumeration<T>
  {
  private:
    using u_t                 = _::sized_integer_t<T>;

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

  template<> struct enumeration<bool>
  {
    static constexpr std::uint64_t last = 1;

    static constexpr std::uint64_t index(bool v) noexcept
    {
      return v ? 1 : 0;
    }
    static constexpr bool value(std::uint64_t i) noexcept
    {
      return i != 0;
    }
  };

  template<std::floating_point T>
    requires(sizeof(T) == 4 || sizeof(T) == 8)
  struct enumeration<T>
  {
  private:
    using u_t                 = _::sized_integer_t<T>;

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
