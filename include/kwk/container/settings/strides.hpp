//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/extent.hpp>
#include <kwk/container/settings/category.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/settings/strides.hpp>
#include <kwk/settings/size.hpp>

namespace kwk
{
  // The strides are fetched or computed from the size
  template<typename Category, rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick(Category c, detail::strides_, Settings const& opts)
  {
    using shape_type = rbr::result::fetch_t<size,Settings>;

    if constexpr(!concepts::descriptor<shape_type>)
      return opts[strides | as_stride(pick(c,size,opts))];
  }
}
