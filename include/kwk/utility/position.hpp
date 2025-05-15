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

    // === Kumi interface ===
    using is_product_type = void;

    template<std::size_t I>
    friend constexpr auto const& get(position const& p) noexcept
    {
      return p.pos[I];
    }

    template<std::size_t I>
    friend constexpr auto& get(position& p) noexcept
    {
      return p.pos[I];
    }
    // ^^^ Kumi interface ^^^
  };
}

template<std::size_t Sz> struct std::tuple_size<kwk::position<Sz>> : std::integral_constant<std::size_t, Sz> {};
template<std::size_t Sz> struct std::tuple_element<Sz, kwk::position<Sz>> { using type = typename kwk::position<Sz>::position_type; };