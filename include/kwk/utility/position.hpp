//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <type_traits>
#include <iomanip>
#include <ostream>
#include <kwk/detail/kumi.hpp>

namespace kwk
{
  template<std::size_t Size>
  struct position
  {
    using position_type = int;
    position_type pos[Size];

    constexpr position_type& operator[](std::size_t index)
    {
      return pos[index];
    }

    constexpr position_type const& operator[](std::size_t index) const
    {
      return pos[index];
    }

    constexpr bool operator==(position<Size> const& other) const
    {
      for (std::size_t i = 0; i < Size; ++i)
      {
        if (this->operator[](i) != other[i])
          return false;
      }
      return true;
    }

    template<std::size_t Size_>
    friend std::ostream& operator<<(std::ostream& os, position<Size_> const& p)
    {
      os << "position<" << Size_ << ">(";
      for (std::size_t i = 0; i < Size_; ++i)
      {
        os << p.pos[i];
        if (i + 1 != Size_) os << ", ";
      }
      return os << ")";
    }
  };
}
