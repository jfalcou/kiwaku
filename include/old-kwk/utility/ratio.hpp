//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/utility/fixed.hpp>
#include <type_traits>
#include <ostream>

namespace kwk
{
  template<typename N, typename D>
  struct ratio
  {
    constexpr ratio(N n, D d) : num(n), denum(d) {}

    friend std::ostream& operator<<(std::ostream& os, ratio r)
    {
      return os << r.num << "/" << r.denum;
    }

    N num;
    D denum;
  };

  template<typename N, typename D>
  constexpr auto operator-(ratio<N, D> r) noexcept { return ratio{-r.num, r.denum}; }

  template<typename N1, typename D1,typename N2, typename D2>
  constexpr auto operator+(ratio<N1,D1> a, ratio<N2,D2> b ) noexcept
  {
    auto[n1,d1] = a;
    auto[n2,d2] = b;
    return ratio{n1*d2+n2*d1, d1*d2};
  }

  template<typename N1, typename D1,typename N2, typename D2>
  constexpr auto operator-(ratio<N1,D1> a, ratio<N2,D2> b ) noexcept
  {
    auto[n1,d1] = a;
    auto[n2,d2] = b;
    return ratio{n1*d2-n2*d1, d1*d2};
  }

  template<typename N1, typename D1,typename N2, typename D2>
  constexpr auto operator*(ratio<N1,D1> a, ratio<N2,D2> b ) noexcept
  {
    return ratio{a.num*b.num, a.denum*b.denum};
  }

  template<typename N1, typename D1>
  constexpr auto operator*(ratio<N1,D1> a, auto c ) noexcept
  {
    return ratio{a.num * c, a.denum};
  }

  template<typename N1, typename D1>
  constexpr auto operator*(auto c, ratio<N1,D1> a) noexcept
  {
    return a * c;
  }

  template<typename N1, typename D1,typename N2, typename D2>
  constexpr auto operator/(ratio<N1,D1> a, ratio<N2,D2> b ) noexcept
  {
    return ratio{a.num*b.denum, a.denum*b.num};
  }

  template<typename N1, typename D1>
  constexpr auto operator/(ratio<N1,D1> a, auto b ) noexcept
  {
    return ratio{a.num, a.denum*b};
  }

  template<typename N1, typename D1,typename N2, typename D2>
  constexpr bool operator==(ratio<N1,D1> a, ratio<N2,D2> b ) noexcept
  {
    return a.num*b.denum == a.denum*b.num;
  }

  template<typename N1, typename D1,typename N2, typename D2>
  constexpr bool operator!=(ratio<N1,D1> a, ratio<N2,D2> b ) noexcept
  {
    return !(a == b);
  }
};