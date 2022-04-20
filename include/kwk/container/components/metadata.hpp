//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <kwk/options/label.hpp>

namespace kwk::detail
{
  template<typename... Data> struct metadata;

  template<typename Label, typename... Data>
  requires (!std::same_as<Label, rbr::unknown_key>)
  struct metadata<Label, Data...>
  {
    static constexpr bool has_label = true;

    template<rbr::concepts::option... Opts>
    constexpr   metadata(auto const&, rbr::settings<Opts...> const& opts)
              : label_(opts[kwk::label])
    {}

    constexpr Label const& label() const requires( has_label ) { return label_; }

    Label label_;
  };

  template<typename... Data>
  requires (std::same_as<Data, rbr::unknown_key> && ...)
  struct metadata<Data...>
  {
    static constexpr bool has_label = false;

    template<rbr::concepts::option... Opts>
    constexpr metadata(auto const&, rbr::settings<Opts...> const&) {}
  };
}
