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
#include <kwk/settings/allocator.hpp>
#include <kwk/utility/memory/heap_allocator.hpp>

namespace kwk
{
  //  Table use heap_allocator by default
  template<rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick(detail::table_, detail::allocator_, Settings const& opts)
  {
    return opts[allocator | heap_allocator{}];
  }

  //  View can't have allocator - hard error
  template<rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick(detail::view_, detail::allocator_, Settings const&) =delete;
}
