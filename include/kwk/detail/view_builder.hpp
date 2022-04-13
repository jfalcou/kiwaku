//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/container/view/view_metadata.hpp>
#include <kwk/container/view/view_span.hpp>
#include <kwk/container/view/view_access.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/options.hpp>

namespace kwk::detail
{
  template<typename Type, auto... Options>
  struct  view_builder
  {
    static constexpr auto opts  = rbr::settings(Options...);

    // Computes view_span type
    static constexpr auto src = opts[source | ptr_source<Type>{} ];
    using span_type           = detail::view_span< typename decltype(src)::span_type >;

    // Computes view_access type
    static constexpr auto shape   = opts[size     | src.default_shape() ];
    static constexpr auto stride  = opts[strides  | shape.as_stride()   ];
    using access_type             = detail::view_access< shape, stride >;

    // Computes view_metadata type;
    using label_t   = rbr::result::fetch_t<label, decltype(opts)>;
    using meta_type = detail::view_metadata<label_t>;
  };
}

