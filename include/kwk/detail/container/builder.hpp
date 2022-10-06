//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/container/pick.hpp>
#include <kwk/detail/container/metadata.hpp>
#include <kwk/detail/container/accessor.hpp>
#include <kwk/detail/memory/block.hpp>

namespace kwk::detail
{
  template<rbr::concepts::settings auto Settings>
  struct  builder
  {
    using opts_t  = decltype(Settings);

    // Computes view_access type
    static constexpr auto shape   = pick(size, Settings);
    static constexpr auto stride  = pick(strides, Settings);
    using accessor                = detail::accessor< shape, stride >;

    // Computes data_block type
    using data_block  = block_type_t<opts_t>;

    // Computes metadata type;
    using metadata    = detail::metadata<result::pick_t<label, opts_t>>;
  };
}
