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
#include <kwk/settings/type.hpp>

namespace kwk
{
  // The data source is fetched or a ptr_source is returned using the type information if any
  template<typename Category, rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick(Category, detail::source_, Settings const& opts)
  {
    auto value = opts[type];
    return opts[source | ptr_source<typename decltype(value)::type>{}];
  }
}
