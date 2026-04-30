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
  template<typename N, typename D> struct ratio
  {
    N num;
    D denom;

    KWK_TRIVIAL constexpr auto operator-() const noexcept { return kwk::ratio{-num, denom}; }

    KWK_TRIVIAL constexpr auto operator+() const noexcept { return *this; }

    template<typename N2, typename D2> KWK_TRIVIAL friend constexpr auto operator+(ratio r, ratio<N2, D2> o) noexcept
    {
      return kwk::ratio{r.num * o.denom + o.num * r.denom, r.denom * o.denom};
    }

    template<typename N2, typename D2> KWK_TRIVIAL friend constexpr auto operator-(ratio r, ratio<N2, D2> o) noexcept
    {
      return kwk::ratio{r.num * o.denom - o.num * r.denom, r.denom * o.denom};
    }

    template<typename N2, typename D2> KWK_TRIVIAL friend constexpr auto operator*(ratio r, ratio<N2, D2> o) noexcept
    {
      return kwk::ratio{r.num * o.num, r.denom * o.denom};
    }

    template<typename N2, typename D2> KWK_TRIVIAL friend constexpr auto operator/(ratio r, ratio<N2, D2> o) noexcept
    {
      return kwk::ratio{r.num * o.denom, r.denom * o.num};
    }

    template<typename N2, typename D2> KWK_TRIVIAL friend constexpr bool operator==(ratio r, ratio<N2, D2> o) noexcept
    {
      return (r.num * o.denom) == (r.denom * o.num);
    }

    template<typename N2, typename D2>
    KWK_TRIVIAL friend constexpr std::strong_ordering operator<=>(ratio r, ratio<N2, D2> o) noexcept
    {
      auto lhs = r.num * o.denom;
      auto rhs = r.denom * o.num;
      if (lhs < rhs) return std::strong_ordering::less;
      else if (lhs > rhs) return std::strong_ordering::greater;
      else return std::strong_ordering::equal;
    }
  };

  template<typename N, typename D, kwk::concepts::integral_arithmetic_value V>
  KWK_TRIVIAL constexpr auto operator+(ratio<N, D> r, V v) noexcept
  {
    return kwk::ratio{r.num + v * r.denom, r.denom};
  }

  template<typename N, typename D, kwk::concepts::integral_arithmetic_value V>
  KWK_TRIVIAL constexpr auto operator+(V v, ratio<N, D> r) noexcept
  {
    return r + v;
  }

  template<typename N, typename D, kwk::concepts::integral_arithmetic_value V>
  KWK_TRIVIAL constexpr auto operator-(ratio<N, D> r, V v) noexcept
  {
    return kwk::ratio{r.num - v * r.denom, r.denom};
  }

  template<typename N, typename D, kwk::concepts::integral_arithmetic_value V>
  KWK_TRIVIAL constexpr auto operator-(V v, ratio<N, D> r) noexcept
  {
    return kwk::ratio{v * r.denom - r.num, r.denom};
  }

  template<typename N, typename D, kwk::concepts::integral_arithmetic_value V>
  KWK_TRIVIAL constexpr auto operator*(ratio<N, D> r, V v) noexcept
  {
    return kwk::ratio{r.num * v, r.denom};
  }

  template<typename N, typename D, kwk::concepts::integral_arithmetic_value V>
  KWK_TRIVIAL constexpr auto operator*(V v, ratio<N, D> r) noexcept
  {
    return r * v;
  }

  template<typename N, typename D, kwk::concepts::integral_arithmetic_value V>
  KWK_TRIVIAL constexpr auto operator/(ratio<N, D> r, V v) noexcept
  {
    return kwk::ratio{r.num, r.denom * v};
  }

  template<typename N, typename D>
  requires(!kwk::concepts::integral_arithmetic_value<N> || !kwk::concepts::integral_arithmetic_value<D>)
  struct ratio<N, D>
  {
    static_assert(kwk::concepts::integral_arithmetic_value<N>, "Numerator type must be an integral type");
    static_assert(kwk::concepts::integral_arithmetic_value<D>, "Denominator type must be an integral type");

    ratio(N&&, D&&) = delete;
  };

  // @brief Deduction guides
  template<class I> ratio(I&& i) -> ratio<std::unwrap_ref_decay_t<I>, constant<1>>;
  template<class N, class D> ratio(N&&, D&&) -> ratio<std::unwrap_ref_decay_t<N>, std::unwrap_ref_decay_t<D>>;
}
