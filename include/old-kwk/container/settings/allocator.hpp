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
#include <kwk/settings/allocator.hpp>
#include <kwk/utility/memory/heap_allocator.hpp>

namespace kwk::__
{
  //  Table use heap_allocator by default
  template<rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick_(KWK_DELAY(), table_, allocator_, Settings const& o)
  {
    return o[allocator | heap_allocator{}];
  }
}
