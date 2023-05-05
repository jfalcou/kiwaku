//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/utility/container/shape.hpp>
#include <kwk/detail/raberu.hpp>
#include <concepts>

namespace kwk::__
{
  struct size_ : rbr::as_keyword<size_>
  {
    // Single integral -> 1D shape
    template<std::convertible_to<std::ptrdiff_t> T>
    constexpr auto operator=(T s) const noexcept
    {
      return of_size(s);
    }

    // Extent from of_size(...)
    template<auto... S>
    constexpr auto operator=(shape<S...> const& s) const noexcept
    {
      return s;
    }

    // Display
    template<typename Shape> std::ostream& display(std::ostream& os, Shape const& s) const
    {
      return os << "Shape: " << s;
    }
  };
}

namespace kwk
{
  //================================================================================================
  //! @ingroup  settings
  //! @brief    Size setting for kwk::table and kwk::view
  //================================================================================================
  inline constexpr __::size_ size = {};
}
