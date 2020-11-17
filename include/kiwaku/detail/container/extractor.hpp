//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/container/view_span.hpp>
#include <kiwaku/detail/container/view_access.hpp>
#include <kiwaku/stride.hpp>
#include <kiwaku/shape.hpp>

namespace kwk::detail
{
  template<typename Type, auto... Settings>
  struct  settings_extractor
  {
    using shaper_t = detail::shaper<detail::dynamic_size,detail::dynamic_size>;

    static constexpr auto opt_    = rbr::settings(Settings...);

    static constexpr auto shape_  = kwk::shape<opt_[ option::shape | shaper_t{} ] >{};
    static constexpr auto stride_ = opt_[ option::stride | shape_.as_stride() ];

    static constexpr bool is_dynamic      = shape_.is_dynamic;
    static constexpr bool is_fully_static = shape_.is_fully_static;

    using span_base       = detail::view_span<Type*>;
    using access_base     = detail::view_access<shape_, stride_>;
  };
}
