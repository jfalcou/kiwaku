//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once
#include <kiwaku/detail/raberu.hpp>
#include <type_traits>
#include <utility>

namespace kwk
{
  template<auto Shape, auto Stride> struct view_access;

  //================================================================================================
  // Rank N hape is fully static: no contents, only computation
  //================================================================================================
  template<auto Shape, auto Stride>
  requires( Shape.is_fully_static )
  struct view_access<Shape,Stride>
  {
    using shape_type  = std::remove_cvref_t<decltype(Shape)>;
    using stride_type = std::remove_cvref_t<decltype(Stride)>;
    static constexpr auto static_nbdims = shape_type::static_nbdims;

    constexpr auto  size()    const noexcept  { return Shape.numel(); }
    constexpr auto  shape()   const noexcept  { return Shape;         }
    constexpr auto  stride()  const noexcept  { return Stride;        }

    template<rbr::concepts::option... Opts>
    constexpr view_access(rbr::settings<Opts...> const&) {}
    constexpr void swap( view_access& ) noexcept {}

    template<std::integral... Is>
    constexpr auto index(Is... is) const noexcept { return Stride.index(is...); }
  };
}
