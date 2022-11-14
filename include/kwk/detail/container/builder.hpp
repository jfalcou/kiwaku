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
  template<rbr::concepts::settings auto Options>
  struct  builder
  {
    static constexpr auto options   = Options;
    static constexpr auto kind      = Options[category];
    using options_t                 = decltype(options);
    using shape_t                   = result::pick_t<size, options_t>;

    // Computes shape type
    static constexpr auto shape   = []()
    {
      if constexpr(!concepts::descriptor<shape_t>)  return pick(size, options);
      else                                          return kwk::shape<pick(size, options)>{};
    }();

    // Computes stride type
    static constexpr auto stride  =  []()
    {
      if constexpr(!concepts::descriptor<shape_t>)  return pick(strides, options);
      else                                          return pick(strides, rbr::settings(kind,shape));
    }();

    // Builds all elements of a container: accessor, metadata, memory block
    using accessor    = detail::accessor<shape, stride>;
    using metadata    = detail::metadata<result::pick_t<label, options_t>>;
    using value_type  = typename result::pick_t<kwk::type,options_t>::type;
    using memory      = block_t<kind, shape, value_type, options_t>;
  };
}
