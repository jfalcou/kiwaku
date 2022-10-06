//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/container/settings/category.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/settings/strides.hpp>
#include <kwk/settings/size.hpp>

namespace kwk
{
  // The strides are fetched or computed from the size
  template<typename Category, rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick(Category, detail::strides_, Settings const& opts)
  {
    return opts[strides | as_stride(pick(size,opts))];
  }
}
