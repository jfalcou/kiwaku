//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <raberu/raberu.hpp>
#include <kwk/settings/size.hpp>
#include <kwk/settings/source.hpp>

namespace kwk::__
{
  // The data size is fetched or a default shape is fetched from the source if any
  template<typename Category, rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick_(KWK_DELAY(), Category, size_, Settings const& opts)
  {
    return opts[size | default_shape(pick(source,opts))];
  }
}
