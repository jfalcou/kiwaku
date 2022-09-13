//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <kwk/container/options/forward.hpp>
#include <kwk/options.hpp>

namespace kwk::tag { struct view_ {}; }

namespace kwk::options
{
  // For view, we compute the data_block from the source and base_index
  template<rbr::concepts::settings Settings>
  constexpr auto block(tag::view_ const& m, Settings const& p) noexcept
  {
    auto src = source(m,p);
    return src.as_block(offset(m,p));
  }

  // For view, we infer the type from the source that must be present
  template<rbr::concepts::settings Settings>
  struct element<tag::view_, Settings>
  {
    using type = typename data<tag::view_,Settings>::type::base_type;
  };
}
