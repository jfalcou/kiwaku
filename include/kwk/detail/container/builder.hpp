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
#include <kwk/detail/raberu.hpp>

namespace kwk::detail
{
  template<auto Tag, auto... Os>
  struct  builder
  {
    static constexpr auto options  = rbr::settings{Tag,Os...};
    using options_t                = decltype(options);

    // Computes view_access type
    static constexpr auto shape   = pick(size, options);
    static constexpr auto stride  = pick(strides, options);
    using accessor                = detail::accessor< shape, stride >;

    // Computes data_block type
    using memory  = block_type_t<options_t>;

    // Computes metadata type;
    using metadata    = detail::metadata<result::pick_t<label, options_t>>;
  };
}
