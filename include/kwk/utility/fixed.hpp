//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/configuration.hpp>
#include <bit>
#include <cstdint>
#include <iosfwd>

namespace kwk
{
  namespace __
  {
    template<auto Value> constexpr auto clamp()
    {
      if constexpr (Value < 0)
      {
        if constexpr (Value >= -128) return static_cast<std::int8_t>(Value);
        else if constexpr (Value >= -32768) return static_cast<std::int16_t>(Value);
        else if constexpr (Value >= -2147483648LL) return static_cast<std::int32_t>(Value);
        else return Value;
      }
      else
      {
        if constexpr (Value <= 255) return static_cast<std::uint8_t>(Value);
        else if constexpr (Value <= 65535) return static_cast<std::uint16_t>(Value);
        else if constexpr (Value <= 4294967295ULL) return static_cast<std::uint32_t>(Value);
        else return Value;
      }
    }

    template<char... c> constexpr auto b10()
    {
      std::uint64_t value{0};
      ((value = value * 10 + (c - '0')), ...);
      return value;
    }
  }

  //====================================================================================================================
  /**
    @ingroup utility
    @brief Compile-time constant wrapper

    This type is a compile-time constant wrapper that provides some additional features over `std::integral_constant`:
    - A user-friendly output operator that prints the value followed by `_c` to indicate it's a compile-time constant.
    - Unary plus and minus operators to allow for easy negation and identity of constants, with proper handling of
      unsigned types to avoid underflow issues.
    - Binary arithmetic operators (`+`, `-`, `*`, `/`) that allow for compile-time arithmetic between constants,
      with results automatically clamped to the appropriate type to prevent overflow or underflow.
  **/
  //====================================================================================================================
  template<auto N> struct constant
  {
    static constexpr auto value = __::clamp<N>();
    using value_type = decltype(value);

    constexpr operator value_type() const noexcept { return value; }

    constexpr value_type operator()() const noexcept { return value; }

    template<typename C, typename Ct> friend auto& operator<<(std::basic_ostream<C, Ct>& os, constant)
    {
      return os << '\'' << +N << '\'';
    }

    /// @brief Unary minus operator that negates the constant value.
    KWK_TRIVIAL constexpr auto operator-() const noexcept
    {
      if constexpr (std::is_unsigned_v<decltype(N)>) return constant<-static_cast<std::int64_t>(N)>{};
      else return constant<-N>{};
    }

    /// @brief Unary plus operator that returns the constant value unchanged.
    KWK_TRIVIAL constexpr auto operator+() const noexcept { return *this; }

    /// @brief Binary addition operator that computes the sum of two constants at compile time.
    template<auto M> KWK_TRIVIAL friend constexpr auto operator+(constant, constant<M>) noexcept
    {
      return constant<N + M>{};
    }

    /// @brief Binary subtraction operator that computes the difference of two constants at compile time.
    template<auto M> KWK_TRIVIAL friend constexpr auto operator-(constant, constant<M>) noexcept
    {
      return constant<N - M>{};
    }

    /// @brief Binary multiplication operator that computes the product of two constants at compile time.
    template<auto M> KWK_TRIVIAL friend constexpr auto operator*(constant, constant<M>) noexcept
    {
      return constant<N * M>{};
    }

    /// @brief Binary division operator that computes the quotient of two constants at compile time.
    template<auto M> KWK_TRIVIAL friend constexpr auto operator/(constant, constant<M>) noexcept
    {
      return constant<N / M>{};
    }

    /// @brief Equality operator that compares the values of two constants at compile time.
    template<auto M> friend constexpr bool operator==(constant, constant<M>) noexcept { return N == M; }
  };

  //================================================================================================
  /**
    @ingroup utility
    @brief Provides a short-cut to define a compile-time constant value from a literal integer
    @related constant
  **/
  //================================================================================================
  template<auto N> inline constexpr auto fixed = constant<__::clamp<N>()>{};

  inline namespace literals
  {
    //==============================================================================================
    /**
      @ingroup utility
      @brief User-defined literal suffix for compile-time constant

      The underlying type is computed from the actual value to be the smallest fitting type.
      This means, for example, that `123_c` is an instance of `kwk::constant<unsigned char,123>`.
      @related kwk::constant
    **/
    //==============================================================================================
    template<char... c> KWK_TRIVIAL constexpr auto operator""_c() noexcept
    {
      return fixed<__::clamp<__::b10<c...>()>()>;
    }
  }
}
