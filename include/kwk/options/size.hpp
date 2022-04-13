//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kwk/container/shape.hpp>
#include <kwk/detail/raberu.hpp>
#include <concepts>

namespace kwk
{
#if !defined(KWK_USE_DOXYGEN)
  struct size_ : rbr::as_keyword<size_>
  {
    // Single integral -> 1D shape
    template<std::convertible_to<std::ptrdiff_t> T>
    constexpr auto operator=(T s) const noexcept
    {
      return of_size(s);
    }

    // Extent from of_size(...)
    template<auto Shaper>
    constexpr auto operator=(shape<Shaper> const& s) const noexcept
    {
      return s;
    }

    // Display
    template<typename Shape> std::ostream& display(std::ostream& os, Shape const& s) const
    {
      return os << "Shape: " << s;
    }
  };
#endif

  /**
    @ingroup  options
    @brief    Keyword for specifying shape options
   **/
  inline constexpr size_ size = {};
}
