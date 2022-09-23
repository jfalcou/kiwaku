//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <ostream>

namespace kwk::detail
{
  struct axis
  {
    axis& operator=(std::size_t s) { size = s; return *this;}
    std::size_t     dims;
    std::ptrdiff_t  size;
  };

  struct value_joker
  {
    friend std::ostream& operator<<(std::ostream& os, value_joker const&)
    {
      return os << "_";
    }

    constexpr auto operator[](std::size_t d) const noexcept
    {
      return axis{d,0};
    }
  };
}

namespace kwk
{
  inline constexpr detail::value_joker _ = {};
}
