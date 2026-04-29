#pragma once

namespace kwk
{
  template<typename N, typename D> struct ratio
  {
    constexpr ratio();

    constexpr ratio(N n, D d) : num(n), denom(d) {}

    constexpr ratio(std::integral auto i) : num(i), denom(1) {}

    template<typename M, typename E> explicit constexpr ratio(ratio<M, E> const& r) : num(r.num), denom(r.denom) {}

    N num;
    D denom;

    KWK_TRIVIAL constexpr auto operator-() const noexcept { return ratio{-num, denom}; }

    KWK_TRIVIAL constexpr auto operator+() const noexcept { return *this; }

    template<typename N2, typename D2>
    KWK_TRIVIAL friend constexpr auto operator+(ratio const& r, ratio<N2, D2> const& o) noexcept
    {
      return ratio{r.num * o.denom + o.num * r.denom, r.denom * o.denom};
    }

    template<typename N2, typename D2>
    KWK_TRIVIAL friend constexpr auto operator-(ratio const& r, ratio<N2, D2> const& o) noexcept
    {
      return ratio{r.num * o.denom - o.num * r.denom, r.denom * o.denom};
    }

    template<typename N2, typename D2>
    KWK_TRIVIAL friend constexpr auto operator*(ratio const& r, ratio<N2, D2> const& o) noexcept
    {
      return ratio{r.num * o.num, r.denom * o.denom};
    }

    template<typename N2, typename D2>
    KWK_TRIVIAL friend constexpr auto operator/(ratio const& r, ratio<N2, D2> const& o) noexcept
    {
      return ratio{r.num * o.denom, r.denom * o.num};
    }

    template<typename N2, typename D2> friend constexpr bool operator==(ratio const& r, ratio<N2, D2> const& o) noexcept
    {
      return (r.num * o.denom) == (r.denom * o.num);
    }
  };
}
