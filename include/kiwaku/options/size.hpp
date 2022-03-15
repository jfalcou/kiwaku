//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/container/shape.hpp>
#include <kiwaku/detail/raberu.hpp>
#include <concepts>

namespace kwk
{
#if !defined(KWK_USE_DOXYGEN)
  struct size_ : rbr::as_keyword<size_>
  {
    // Single integral -> 1D shape
    template<std::convertible_to<std::ptrdiff_t> T>
    auto operator=(T s) const noexcept
    {
      return of_size(s);
    }

    // Extent from of_size(...)
    template<auto Shaper> auto operator=(shape<Shaper> const& s) const noexcept
    {
      return s;
    }

    // Display
    template<typename Shape> std::ostream& show(std::ostream& os, Shape const& s) const
    {
      return os << "Shape: " << s;
    }

    // template<typename Type>
    // constexpr auto operator|(Type&& v) const noexcept requires( accept<Type>() )
    // {
    //   return rbr::detail::type_or_<size_,std::remove_cvref_t<Type>>{KWK_FWD(v)};
    // }

    // template<typename Func> constexpr auto operator|(rbr::call<Func>&& v) const noexcept
    // {
    //   return rbr::detail::type_or_<size_,rbr::call<Func>>{KWK_FWD(v)};
    // }
  };
#endif

  /**
    @ingroup  options
    @brief    Keyword for accessing shape options
   **/
  inline constexpr size_ size = {};
}
