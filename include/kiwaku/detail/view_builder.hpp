//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/components/view/view_span.hpp>
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
  };
}

