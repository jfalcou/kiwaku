//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/container/view/view_span.hpp>
#include <kiwaku/container/view/view_access.hpp>
#include <kiwaku/detail/raberu.hpp>
#include <kiwaku/options.hpp>

namespace kwk::detail
{
  template<typename Type, auto... Options>
  struct  view_builder
  {
    static constexpr auto opts    = rbr::settings(Options...);

    // Computes view_span type
    static constexpr auto source  = opts[kwk::source];
    using span_type = view_span< typename decltype(source)::span_type >;

    // Computes view_access type
    static constexpr auto shape  = opts[kwk::size     | source.default_shape()];
    static constexpr auto stride = shape.as_stride();
    //static constexpr auto stride = opts[kwk::strides  | shape.as_stride() ];
    using access_type = view_access< shape, stride >;
  };
}

