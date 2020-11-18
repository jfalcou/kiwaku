//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/round.hpp>
#include <concepts>

namespace kwk
{
  //================================================================================================
  // elements is a signed integral representing a size in elements
  //================================================================================================
  enum class elements : std::ptrdiff_t {};

  //================================================================================================
  // Integral conversion
  //================================================================================================
  inline constexpr std::ptrdiff_t to_integer(elements b) noexcept
  {
    return static_cast<std::ptrdiff_t>(b);
  }

  //================================================================================================
  // Size related arithmetic operations
  //================================================================================================
  inline constexpr auto operator+(elements a, elements b) noexcept
  {
    return elements{ to_integer(a) + to_integer(b) };
  }

  template<std::integral T> inline constexpr auto operator+(elements b, T n) noexcept
  {
    return elements{ to_integer(b) + static_cast<std::ptrdiff_t>(n) };
  }

  template<std::integral T> inline constexpr auto operator+(T n, elements b) noexcept
  {
    return elements{ to_integer(b) + static_cast<std::ptrdiff_t>(n) };
  }

  inline constexpr elements& operator+=(elements& a, elements b) noexcept
  {
    a = a+b;
    return a;
  }

  template<std::integral T>
  inline constexpr elements& operator+=(elements& a, T b) noexcept
  {
    a = a+b;
    return a;
  }

  template<std::integral T> inline constexpr auto operator*(elements b, T n) noexcept
  {
    return elements{ to_integer(b) * static_cast<std::ptrdiff_t>(n) };
  }

  template<std::integral T> inline constexpr auto operator*(T n, elements b) noexcept
  {
    return elements{ to_integer(b) * static_cast<std::ptrdiff_t>(n) };
  }

  template<std::integral T>
  inline constexpr elements& operator*=(elements& a, T b) noexcept
  {
    a = a*b;
    return a;
  }

  inline constexpr auto operator-(elements a, elements b) noexcept
  {
    return elements{ to_integer(a) - to_integer(b) };
  }

  inline constexpr elements& operator-=(elements& a, elements b) noexcept
  {
    a = a-b;
    return a;
  }

  template<std::integral T>
  inline constexpr elements& operator-=(elements& a, T b) noexcept
  {
    a = a-b;
    return a;
  }

  template<std::integral T> inline constexpr auto operator-(elements b, T n) noexcept
  {
    return elements{ to_integer(b) - static_cast<std::ptrdiff_t>(n) };
  }

  template<std::integral T> inline constexpr auto operator/(elements b, T n) noexcept
  {
    return elements{ to_integer(b) / static_cast<std::ptrdiff_t>(n) };
  }

  template<std::integral T> inline constexpr elements& operator/=(elements& a, T b) noexcept
  {
    a = a/b;
    return a;
  }

  //================================================================================================
  // Interaction with pointer
  //================================================================================================
  template<typename T> inline constexpr auto operator+(T* ptr, elements b) noexcept
  {
    return ptr + to_integer(b);
  }

  template<typename T> inline constexpr auto operator-(T* ptr, elements b) noexcept
  {
    return ptr - to_integer(b);
  }

  template<typename T> inline constexpr auto operator+=(T*& ptr, elements b) noexcept
  {
    ptr = ptr + b;
    return ptr;
  }

  template<typename T> inline constexpr auto operator-=(T*& ptr, elements b) noexcept
  {
    ptr = ptr - b;
    return ptr;
  }

  //================================================================================================
  // Comparisons
  //================================================================================================
  template<std::integral T> inline constexpr bool operator==(T n, elements b) noexcept
  {
    return to_integer(b) ==static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator==(elements b, T n) noexcept
  {
    return to_integer(b) == static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator!=(T n, elements b) noexcept
  {
    return to_integer(b) != static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator!=(elements b, T n) noexcept
  {
    return to_integer(b) != static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator<(T n, elements b) noexcept
  {
    return to_integer(b) < static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator<(elements b, T n) noexcept
  {
    return to_integer(b) < static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator>(T n, elements b) noexcept
  {
    return to_integer(b) > static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator>(elements b, T n) noexcept
  {
    return to_integer(b) > static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator<=(T n, elements b) noexcept
  {
    return to_integer(b) <= static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator<=(elements b, T n) noexcept
  {
    return to_integer(b) <= static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator>=(T n, elements b) noexcept
  {
    return to_integer(b) >= static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator>=(elements b, T n) noexcept
  {
    return to_integer(b) >= static_cast<std::ptrdiff_t>(n);
  }

  inline constexpr bool operator!(elements b) noexcept
  {
    return to_integer(b) == 0;
  }

  //================================================================================================
  // Alignment computation
  //================================================================================================
  [[nodiscard]] inline constexpr elements round_to(elements value, std::ptrdiff_t alg) noexcept
  {
    return elements{ detail::round_to(to_integer(value),alg)};
  }

  inline constexpr bool is_rounded(elements value, std::ptrdiff_t alg) noexcept
  {
    return detail::is_rounded(to_integer(value),alg);
  }

  //================================================================================================
  // Stream interaction
  //================================================================================================
  inline std::ostream& operator<<(std::ostream& os, elements b)
  {
    return os << to_integer(b);
  }

  //================================================================================================
  // Literal constructor
  //================================================================================================
  namespace literals
  {
    inline constexpr auto operator""_N(unsigned long long int d) noexcept
    {
      return elements{ static_cast<std::ptrdiff_t>(d) };
    }
  }
}
