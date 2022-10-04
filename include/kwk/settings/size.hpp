//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/utility/container/shape.hpp>
#include <kwk/detail/raberu.hpp>
#include <concepts>

namespace kwk::detail
{
  struct size_ : rbr::as_keyword<size_>
  {
    // Single integral -> 1D shape
    template<std::convertible_to<std::ptrdiff_t> T>
    constexpr auto operator=(T s) const noexcept
    {
      return of_size(s);
    }

    // Extent -> shape<Extent>{}
    template<typename SizeType, typename... Ops>
    constexpr auto operator=(detail::combo<SizeType,Ops...> const&) const noexcept
    {
      return shape<detail::combo<SizeType,Ops...>{}>{};
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
}

namespace kwk
{
  //================================================================================================
  //! @ingroup  settings
  //! @brief    Size setting for kwk::table and kwk::view
  //================================================================================================
  inline constexpr detail::size_ size = {};
}
