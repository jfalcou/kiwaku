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
#include <kwk/settings/label.hpp>

namespace kwk
{
  template<typename Category, rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick(Category, __::label_, Settings const& opts)
  {
    return opts[label];
  }
}
