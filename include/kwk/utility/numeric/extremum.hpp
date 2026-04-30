//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/concepts.hpp>

namespace kwk
{
  template<typename Offset, typename Ratio> struct extremum
  {
    Offset o;
    Ratio r;

    KWK_TRIVIAL constexpr auto offset() const noexcept { return o; }

    KWK_TRIVIAL constexpr auto ratio() const noexcept { return r; }

    KWK_TRIVIAL constexpr auto divisor() const noexcept { return r.denom; }

    KWK_TRIVIAL constexpr auto factor() const noexcept { return r.num; }

    KWK_TRIVIAL constexpr auto operator()(auto n) const noexcept { return (n * factor() + offset()) / divisor(); }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, extremum t)
    {
      if (t.factor() != 1) os << t.factor() << " * end";
      else os << "end";
      if (t.divisor() != 1) os << '/' << t.divisor();
      if (t.offset() != 0) os << t.offset();
      return os;
    }

    KWK_TRIVIAL constexpr auto operator-() const noexcept { return kwk::extremum{o, -r}; }

    KWK_TRIVIAL constexpr auto operator+() const noexcept { return *this; }

    template<typename Of, typename Ra> KWK_TRIVIAL friend constexpr bool operator==(extremum lhs, extremum<Of, Ra> rhs)
    {
      return (lhs.offset() == rhs.offset()) && (lhs.ratio() == rhs.ratio());
    }
  };

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  constexpr auto operator+(extremum<O, R> e, V v) noexcept
  {
    return extremum{e.offset() + v * e.divisor(), e.ratio()};
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  constexpr auto operator+(V v, extremum<O, R> e) noexcept
  {
    return e + v;
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  constexpr auto operator-(extremum<O, R> e, V v) noexcept
  {
    // Prevent overflow when end - large_unsigned occurs
    return extremum{e.offset() - v * e.divisor(), e.ratio()};
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  constexpr auto operator-(V v, extremum<O, R> e) noexcept
  {
    return extremum{v * e.divisor() - e.offset(), -e.ratio()};
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  constexpr auto operator*(extremum<O, R> e, V v) noexcept
  {
    return extremum{e.offset() * v, e.ratio() * v};
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  constexpr auto operator*(V v, extremum<O, R> e) noexcept
  {
    return e * v;
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  constexpr auto operator/(extremum<O, R> e, V v) noexcept
  {
    return extremum{e.offset(), e.ratio() / v};
  }

  // @brief Deduction guide
  template<typename O, typename R>
  extremum(O&&, R&&) -> extremum<std::unwrap_ref_decay_t<O>, std::unwrap_ref_decay_t<R>>;

  inline constexpr extremum end{fixed<0>, ratio{fixed<1>, fixed<1>}};
  inline constexpr extremum begin{fixed<0>, ratio{fixed<0>, fixed<1>}};
}
