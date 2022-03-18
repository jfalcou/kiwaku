//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/raberu.hpp>
#include <kiwaku/options/label.hpp>

namespace kwk
{
  template<typename Label> struct view_metadata
  {
    static constexpr bool has_label = !std::same_as<Label, rbr::unknown_key>;

    template<rbr::concepts::option... Opts>
    constexpr   view_metadata(rbr::settings<Opts...> const& opts)
              : label_( opts[kwk::label])
    {}

    constexpr Label const& label() const requires( has_label ) { return label_; }

    Label label_;
  };
}
