//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/container/components/metadata.hpp>
#include <kwk/container/components/data_block.hpp>
#include <kwk/container/components/accessor.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/options.hpp>

namespace kwk::detail
{
  template<typename Tag, typename Type, auto... Options>
  struct  builder
  {
    static constexpr Tag  tag   = {};
    static constexpr auto opts  = rbr::settings(Options...);

    // Computes view_access type
    static constexpr auto shape   = options::shape(tag, opts);
    static constexpr auto stride  = options::stride(tag, opts);
    using accessor                = detail::accessor< shape, stride >;

    // Computes data_block type
    using data_block  = detail::data_block<typename options::data<Tag,decltype(opts)>::type>;

    // Computes metadata type;
    using metadata    = detail::metadata<rbr::result::fetch_t<label, decltype(opts)>>;
  };
}
