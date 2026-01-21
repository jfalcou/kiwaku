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
#include <kwk/settings/reachable.hpp>
#include <kwk/detail/memory/block.hpp>
#include <raberu/raberu.hpp>

namespace kwk::__
{
  template<rbr::concepts::settings auto Options>
  struct  builder
  {
    static constexpr auto options   = Options;
    static constexpr auto kind      = Options[category];
    using options_t                 = decltype(options);
    using shape_t                   = result::pick_t<size, options_t>;

    // Computes shape & stride type
    static constexpr auto shape   = pick(size, options);
    static constexpr auto stride  = pick(strides, options);

    // Retrieve reachability status
    static constexpr auto preserve_reachability = !Options[unreachable];

    // Builds all elements of a container: accessor, metadata, memory block
    using accessor    = __::accessor<shape, stride, options_t::contains(strides)>;
    using metadata    = __::metadata<result::pick_t<label, options_t>>;
    using value_type  = typename result::pick_t<kwk::value_type,options_t>::type;
    using memory      = block_t<kind, shape, value_type, options_t>;
  };
}
