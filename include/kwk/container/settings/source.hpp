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
#include <kwk/settings/source.hpp>

namespace kwk
{
  // The data source is fetched or an empty ptr_source is returned
  template<typename Category, rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick(Category, detail::source_, Settings const& opts)
  {
    return opts[source | ptr_source<rbr::unknown_key>{}];
  }
}
