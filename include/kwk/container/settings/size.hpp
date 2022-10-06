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
#include <kwk/settings/size.hpp>
#include <kwk/settings/source.hpp>

namespace kwk
{
  // The data size is fetched or a default shape is fetched from the source if any
  template<typename Category, rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick(Category, detail::size_, Settings const& opts)
  {
    return opts[size | default_shape(pick(source,opts))];
  }
}
