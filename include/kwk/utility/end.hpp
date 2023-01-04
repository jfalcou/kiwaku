//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/utility/fixed.hpp>
#include <kwk/utility/ratio.hpp>
#include <type_traits>
#include <iomanip>
#include <ostream>

#if !defined(_MSC_VER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#else
#pragma warning( push )
#pragma warning( disable : 4146 )
#endif

namespace kwk
{
  template<typename O, typename R>
  struct end_t
  {
    // Exact final formula : (end * factor + offset) / divisor
    O shift;
    R frac;

    constexpr auto offset()   const noexcept { return shift;      }
    constexpr auto ratio()    const noexcept { return frac;       }
    constexpr auto divisor()  const noexcept { return frac.denum; }
    constexpr auto factor()   const noexcept { return frac.num;   }

    constexpr end_t(O o, R f) noexcept : shift(o), frac(f)
    {}

    friend
    std::ostream& operator<<(std::ostream& os, end_t t)
    {
      if(t.factor()  != 1)  os << t.factor() << " * end"; else os << "end";
      if(t.divisor() != 1)  os << '/' << t.divisor();
      if(t.offset()    != 0)  os << std::showpos << t.offset() << std::noshowpos;
      return os;
    }

    constexpr auto size(auto n) const noexcept
    {
      return (n * factor() + offset()) / divisor();
    }
  };

  /// Deduction guide
  template<typename O, typename R> end_t(O, R) -> end_t<O, R>;

  template<typename O, typename R>
  constexpr auto operator+(end_t<O,R> e, auto o) noexcept
  {
    return end_t{e.offset() + o * e.divisor(), e.frac};
  }

  template<typename O, typename R>
  constexpr auto operator+(auto o, end_t<O,R> e) noexcept
  {
    return e+o;
  }

  template<typename O, typename R>
  constexpr auto operator-(end_t<O,R> e, auto o) noexcept
  {
    // Prevent overflow when end - large_unsigned occurs
    return end_t{e.offset() - o * e.divisor(), e.frac};
  }

  template<typename O, typename R>
  constexpr auto operator-(auto o, end_t<O,R> e) noexcept
  {
    return end_t{o * e.divisor() - e.offset(), - e.frac};
  }

  template<typename O, typename R>
  constexpr auto operator*(end_t<O,R> e, auto o) noexcept
  {
    return end_t{e.offset() * o, e.ratio() * o};
  }

  template<typename O, typename R>
  constexpr auto operator*(auto o, end_t<O,R> e) noexcept
  {
    return e * o;
  }

  template<typename O, typename R>
  constexpr auto operator/(end_t<O,R> e, auto o) noexcept
  {
    return end_t{e.offset(), e.ratio() / o};
  }

  /// Object representing a whole dimension in slicing
  inline constexpr end_t end = {fixed<0>, ratio{fixed<1>,fixed<1>}};
};

#if !defined(_MSC_VER)
#pragma GCC diagnostic pop
#else
#pragma warning( pop )
#endif
