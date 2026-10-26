//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/traits.hpp>
#include <kiwaku/concepts.hpp>

namespace kwk
{
  template<typename Offset, typename Ratio>
  struct extremum : private __::compressed_tuple<Offset, Ratio>
  {
    using parent         = __::compressed_tuple<Offset, Ratio>;

    constexpr extremum() = default;

    constexpr extremum(Offset o, Ratio r)
        : parent {o, r}
    {
    }

    KWK_ABI constexpr auto offset() const noexcept
    {
      return get<0>(static_cast<parent const&>(*this));
    }

    KWK_ABI constexpr auto ratio() const noexcept
    {
      return get<1>(static_cast<parent const&>(*this));
    }

    KWK_ABI constexpr auto divisor() const noexcept { return ratio().denom(); }

    KWK_ABI constexpr auto factor() const noexcept { return ratio().num(); }

    KWK_ABI constexpr auto operator()(auto n) const noexcept
    {
      return (n * factor() + offset()) / divisor();
    }

    KWK_ABI constexpr auto operator-() const noexcept { return kwk::extremum {offset(), -ratio()}; }

    KWK_ABI constexpr auto operator+() const noexcept { return *this; }

    template<typename O, typename R>
    KWK_ABI friend constexpr bool operator==(extremum lhs, extremum<O, R> rhs)
    {
      return (lhs.factor() * rhs.divisor() == rhs.factor() * lhs.divisor()) &&
             (lhs.offset() * rhs.divisor() == rhs.offset() * lhs.divisor());
    }

    template<kwk::concepts::arithmetic_value V>
    KWK_ABI friend constexpr bool operator==(extremum lhs, V v)
    {
      return lhs == kwk::extremum {
                        v, kwk::ratio {fixed<0>, fixed<1>}
      };
    }

    template<typename O, typename R>
    KWK_ABI friend constexpr auto operator<=>(extremum lhs, extremum<O, R> rhs)
    {
      auto const cmp = lhs.ratio() <=> rhs.ratio();
      return cmp != 0 ? cmp : (lhs.offset() * rhs.divisor() <=> rhs.offset() * lhs.divisor());
    }

    template<kwk::concepts::arithmetic_value V>
    KWK_ABI friend constexpr auto operator<=>(extremum lhs, V v)
    {
      return lhs <=> kwk::extremum {
                         v, kwk::ratio {0, fixed<1>}
      };
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         extremum                           t)
    {
      if(t.factor() != 1) os << t.factor() << " * end";
      else os << "end";
      if(t.divisor() != 1) os << '/' << t.divisor();
      if(t.offset() != 0) os << t.offset();
      return os;
    }
  };

  // @brief Deduction guide
  template<typename O, typename R>
  extremum(O&&, R&&) -> extremum<std::unwrap_ref_decay_t<O>, std::unwrap_ref_decay_t<R>>;

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  KWK_ABI constexpr auto operator+(extremum<O, R> e, V v) noexcept
  {
    return extremum {e.offset() + v * e.divisor(), e.ratio()};
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  KWK_ABI constexpr auto operator+(V v, extremum<O, R> e) noexcept
  {
    return e + v;
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  KWK_ABI constexpr auto operator-(extremum<O, R> e, V v) noexcept
  {
    // Prevent overflow when end - large_unsigned occurs
    return extremum {e.offset() - v * e.divisor(), e.ratio()};
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  KWK_ABI constexpr auto operator-(V v, extremum<O, R> e) noexcept
  {
    return extremum {v * e.divisor() - e.offset(), -e.ratio()};
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  KWK_ABI constexpr auto operator*(extremum<O, R> e, V v) noexcept
  {
    return extremum {e.offset() * v, e.ratio() * v};
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  KWK_ABI constexpr auto operator*(V v, extremum<O, R> e) noexcept
  {
    return e * v;
  }

  template<typename O, typename R, kwk::concepts::arithmetic_value V>
  KWK_ABI constexpr auto operator/(extremum<O, R> e, V v) noexcept
  {
    return extremum {e.offset(), e.ratio() / v};
  }

  inline constexpr extremum end {
      fixed<0>,
      ratio {fixed<1>, fixed<1>}
  };
  inline constexpr extremum begin {
      fixed<0>,
      ratio {fixed<0>, fixed<1>}
  };
}
