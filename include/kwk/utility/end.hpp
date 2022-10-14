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
  template<typename T>
  struct relative_end
  {
    T offset;
    constexpr relative_end(T o = {}): offset(o) {}

    template<typename U>
    constexpr auto operator-(U o) const noexcept { return relative_end{offset+o}; }

    friend
    std::ostream& operator<<(std::ostream& os, relative_end t) { return os << "end - " << t.offset; }
  };

  template<typename T>
  constexpr auto offset(relative_end<T> v) noexcept { return v.offset; }

  struct end_t
  {
    template<typename U>
    constexpr auto operator-(U o) const noexcept { return relative_end{o}; }

    friend std::ostream& operator<<(std::ostream& os, end_t) { return os << "end"; }
  };

  constexpr inline auto offset(end_t) noexcept { return fixed<0L>; }

  inline constexpr end_t end = {};
};