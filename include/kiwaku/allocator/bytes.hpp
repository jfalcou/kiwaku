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
  // bytes is a signed integral representing a size in bytes
  //================================================================================================
  enum class bytes : std::ptrdiff_t {};

  //================================================================================================
  // Integral conversion
  //================================================================================================
  inline constexpr std::ptrdiff_t to_integer(bytes b) noexcept
  {
    return static_cast<std::ptrdiff_t>(b);
  }

  //================================================================================================
  // Size related arithmetic operations
  //================================================================================================
  inline constexpr auto operator+(bytes a, bytes b) noexcept
  {
    return bytes{ to_integer(a) + to_integer(b) };
  }

  template<std::integral T> inline constexpr auto operator+(bytes b, T n) noexcept
  {
    return bytes{ to_integer(b) + static_cast<std::ptrdiff_t>(n) };
  }

  template<std::integral T> inline constexpr auto operator+(T n, bytes b) noexcept
  {
    return bytes{ to_integer(b) + static_cast<std::ptrdiff_t>(n) };
  }

  inline constexpr bytes& operator+=(bytes& a, bytes b) noexcept
  {
    a = a+b;
    return a;
  }

  template<std::integral T>
  inline constexpr bytes& operator+=(bytes& a, T b) noexcept
  {
    a = a+b;
    return a;
  }

  template<std::integral T> inline constexpr auto operator*(bytes b, T n) noexcept
  {
    return bytes{ to_integer(b) * static_cast<std::ptrdiff_t>(n) };
  }

  template<std::integral T> inline constexpr auto operator*(T n, bytes b) noexcept
  {
    return bytes{ to_integer(b) * static_cast<std::ptrdiff_t>(n) };
  }

  template<std::integral T>
  inline constexpr bytes& operator*=(bytes& a, T b) noexcept
  {
    a = a*b;
    return a;
  }

  inline constexpr auto operator-(bytes a, bytes b) noexcept
  {
    return bytes{ to_integer(a) - to_integer(b) };
  }

  inline constexpr bytes& operator-=(bytes& a, bytes b) noexcept
  {
    a = a-b;
    return a;
  }

  template<std::integral T>
  inline constexpr bytes& operator-=(bytes& a, T b) noexcept
  {
    a = a-b;
    return a;
  }

  template<std::integral T> inline constexpr auto operator-(bytes b, T n) noexcept
  {
    return bytes{ to_integer(b) - static_cast<std::ptrdiff_t>(n) };
  }

  template<std::integral T> inline constexpr auto operator/(bytes b, T n) noexcept
  {
    return bytes{ to_integer(b) / static_cast<std::ptrdiff_t>(n) };
  }

  template<std::integral T> inline constexpr bytes& operator/=(bytes& a, T b) noexcept
  {
    a = a/b;
    return a;
  }

  //================================================================================================
  // Interaction with pointer
  //================================================================================================
  template<typename T> inline constexpr auto operator+(T* ptr, bytes b) noexcept
  {
    return ptr + to_integer(b)*sizeof(T);
  }

  template<typename T> inline constexpr auto operator-(T* ptr, bytes b) noexcept
  {
    return ptr - to_integer(b)*sizeof(T);
  }

  template<typename T> inline constexpr auto operator+=(T*& ptr, bytes b) noexcept
  {
    ptr = ptr + b;
    return ptr;
  }

  template<typename T> inline constexpr auto operator-=(T*& ptr, bytes b) noexcept
  {
    ptr = ptr - b;
    return ptr;
  }

  //================================================================================================
  // Comparisons
  //================================================================================================
  template<std::integral T> inline constexpr bool operator==(T n, bytes b) noexcept
  {
    return to_integer(b) ==static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator==(bytes b, T n) noexcept
  {
    return to_integer(b) == static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator!=(T n, bytes b) noexcept
  {
    return to_integer(b) != static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator!=(bytes b, T n) noexcept
  {
    return to_integer(b) != static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator<(T n, bytes b) noexcept
  {
    return to_integer(b) < static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator<(bytes b, T n) noexcept
  {
    return to_integer(b) < static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator>(T n, bytes b) noexcept
  {
    return to_integer(b) > static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator>(bytes b, T n) noexcept
  {
    return to_integer(b) > static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator<=(T n, bytes b) noexcept
  {
    return to_integer(b) <= static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator<=(bytes b, T n) noexcept
  {
    return to_integer(b) <= static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator>=(T n, bytes b) noexcept
  {
    return to_integer(b) >= static_cast<std::ptrdiff_t>(n);
  }

  template<std::integral T> inline constexpr bool operator>=(bytes b, T n) noexcept
  {
    return to_integer(b) >= static_cast<std::ptrdiff_t>(n);
  }

  inline constexpr bool operator!(bytes b) noexcept
  {
    return to_integer(b) == 0;
  }

  //================================================================================================
  // Alignment computation
  //================================================================================================
  [[nodiscard]] inline constexpr bytes round_to(bytes value, std::ptrdiff_t alg) noexcept
  {
    return bytes{ detail::round_to(to_integer(value),alg)};
  }

  inline constexpr bool is_rounded(bytes value, std::ptrdiff_t alg) noexcept
  {
    return detail::is_rounded(to_integer(value),alg);
  }

  //================================================================================================
  // Stream interaction
  //================================================================================================
  inline std::ostream& operator<<(std::ostream& os, bytes b)
  {
    return os << to_integer(b);
  }

  //================================================================================================
  // Literal constructor
  //================================================================================================
  namespace literals
  {
    inline constexpr auto operator""_B(unsigned long long int d) noexcept
    {
      return bytes{ static_cast<std::ptrdiff_t>(d) };
    }
  }
}
