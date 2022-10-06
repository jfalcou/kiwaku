//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/container/pick.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/detail/memory/heap_block.hpp>
#include <kwk/detail/memory/stack_block.hpp>

namespace kwk::detail
{
  // For view, we compute the data_block from the source
  template<rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto block(view_ const& m, Settings const& p) noexcept
  {
    return pick(source,p);
  }

  // For view, we compute the data_block from the source
  template<rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto block(table_ const& m, Settings const& p) noexcept
  {
    auto  shp   = pick(kwk::size, p);
    using shp_t = decltype(shp);
    using type  = typename pick_type_t<type,Settings>::type;

    // Get the data block
    auto data = [&]()
    {
      if constexpr( shp_t::is_fully_static )
      {
        return detail::stack_block<type, shp_t{}.numel()>{};
      }
      else
      {
        return detail::heap_block<type>{pick(kwk::allocator,p), shp.numel()};
      }
    }();

    if constexpr( Settings::contains(kwk::source) )
    {
      data.assign( storage(pick(kwk::source, p)) );
    }

    return data;
  }
}
