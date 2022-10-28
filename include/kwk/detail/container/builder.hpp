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

    // Computes shape type
    static constexpr auto shape   = []()
    {
      if constexpr(!concepts::descriptor<result::pick_t<size, options_t>>)
        return pick(size, options);
      else
        return kwk::shape<pick(size, options)>{};
    }();

    // Computes stride type
    static constexpr auto stride  =  []()
    {
      if constexpr(!concepts::descriptor<result::pick_t<size, options_t>>)
        return pick(strides, options);
      else
        return pick(strides, rbr::settings(Tag, shape));
    }();

    // Builds accessor
    using accessor  = detail::accessor<shape, stride>;

    // Builds metadata type;
    using metadata  = detail::metadata<result::pick_t<label, options_t>>;

    // Computes value type
    using value_type  = typename result::pick_t<kwk::type,options_t>::type;

    // Builds data_block type
    using memory  = block_t<Tag, shape, value_type, options_t>;
  };
}
