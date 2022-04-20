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
#include <kwk/container/view/options.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/options.hpp>

namespace kwk::detail
{
  template<typename Type, auto... Options>
  struct  view_builder
  {
    static constexpr tag::view_ mode  = {};
    static constexpr auto       opts  = rbr::settings(Options...);

    // Computes view_access type
    static constexpr auto shape   = options::shape(mode, opts);
    static constexpr auto stride  = options::stride(mode, opts);
    using access_type             = detail::view_access< shape, stride >;

    // Computes view_span type
    using span_type           = detail::view_span< typename options::data<tag::view_,decltype(opts)>::type >;

    // Computes view_metadata type;
    using label_t   = rbr::result::fetch_t<label, decltype(opts)>;
    using meta_type = detail::view_metadata<label_t>;
  };
}

