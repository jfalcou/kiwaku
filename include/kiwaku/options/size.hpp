//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/components/shape.hpp>
#include <kiwaku/detail/raberu.hpp>
#include <concepts>

namespace kwk
{
#if !defined(KWK_USE_DOXYGEN)
  struct size_ : rbr::any_keyword<size_>
  {
    // Single integral -> 1D shape
    template<std::convertible_to<std::ptrdiff_t> T>
    auto operator=(T s) const noexcept
    {
      return (*this = of_size(s));
    }

    // Extent from of_size(...)
    template<auto Shaper> auto operator=(shape<Shaper> const& s) const noexcept
    {
      return rbr::option<size_,shape<Shaper>>{s};
    }

    // Display
    template<typename Shape> std::ostream& show(std::ostream& os, Shape const& s) const
    {
      return os << "Shape: " << s;
    }
  };
#endif

  /**
    @ingroup  options
    @brief    Multi-dimensional size for container
   **/
  inline constexpr size_ size = {};
}
