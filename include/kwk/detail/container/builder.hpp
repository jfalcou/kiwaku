//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/container/components/metadata.hpp>
// #include <kwk/container/components/data_block.hpp>
// #include <kwk/container/components/accessor.hpp>
#include <kwk/container/settings/pick.hpp>
#include <kwk/detail/raberu.hpp>

namespace kwk::detail
{
  template<auto... Options>
  struct  builder
  {
    static constexpr auto opts  = rbr::settings(Options...);

    // Computes view_access type
    static constexpr auto shape   = pick(size, opts);
    static constexpr auto stride  = pick(strides, opts);
    //using accessor                = detail::accessor< shape, stride >;

    // Computes data_block type
    //using data_block  = typename options::data<Tag,decltype(opts)>::type;

    // Computes metadata type;
    // using metadata    = detail::metadata<rbr::result::fetch_t<label, decltype(opts)>>;
  };
}
