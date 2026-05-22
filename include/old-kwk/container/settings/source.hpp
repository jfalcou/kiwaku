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
#include <kwk/settings/source.hpp>
#include <kwk/settings/type.hpp>

namespace kwk::__
{
  // The data source is fetched or a ptr_source is returned using the type information if any
  template<rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick_(KWK_DELAY(), view_, source_, Settings const& opts)
  {
    return opts[source | ptr_source<typename decltype(opts[value_type])::type>{}];
  }

  template<rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick_(KWK_DELAY(), table_, source_, Settings const& opts)
  {
    auto value = opts[value_type];
    return opts[source | ptr_source<typename decltype(value)::type>{}];
  }
}
