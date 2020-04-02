//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_SHAPE_HPP_INCLUDED
#define KIWAKU_SHAPE_HPP_INCLUDED

#include <kiwaku/misc/shape.hpp>
#include <kiwaku/detail/dynamic_shape.hpp>
#include <kiwaku/detail/dynamic_stride.hpp>

namespace kwk
{
  //================================================================================================
  // NTTP options
  //================================================================================================
  inline constexpr auto _0D = detail::dynamic_shape_option<0>{};
  inline constexpr auto _1D = detail::dynamic_shape_option<1>{};
  inline constexpr auto _2D = detail::dynamic_shape_option<2>{};
  inline constexpr auto _3D = detail::dynamic_shape_option<3>{};
  inline constexpr auto _4D = detail::dynamic_shape_option<4>{};

  template<std::size_t N>
  inline constexpr auto _nD = detail::dynamic_shape_option<N>{};
}

#endif
