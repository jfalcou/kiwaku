//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/misc/shape.hpp>
#include <kiwaku/detail/container/shape_helpers.hpp>

namespace kwk
{
  //================================================================================================
  // NTTP options
  //================================================================================================
  inline constexpr detail::shaper extent = {};

  // Dynamic pre-rendered dimension shaper
  inline constexpr auto _0D = extent;
  inline constexpr auto _1D = extent();
  inline constexpr auto _2D = extent()();
  inline constexpr auto _3D = extent()()();
  inline constexpr auto _4D = extent()()()();

  template<std::size_t N>
  inline constexpr auto _nD = []<std::size_t... I>(std::index_sequence<I...> const&)
                              {
                                return detail::shaper<decltype(detail::dynamic_size(I))...>{};
                              }(std::make_index_sequence<N>{});

  //================================================================================================
  // Deduction guides
  //================================================================================================
  template<typename... T> shape(T... s) -> shape< _nD<sizeof...(T)> >;

  //================================================================================================
  // Imperative construction
  //================================================================================================
  template<typename... I> auto of_shape(I... i)
  {
    return shape{i...};
  }
}
