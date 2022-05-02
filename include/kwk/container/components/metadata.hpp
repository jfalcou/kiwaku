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

    constexpr   metadata(auto const&, rbr::concepts::settings auto const& opts)
              : label_(opts[kwk::label])
    {}

    constexpr Label const& label() const noexcept { return label_; }

    constexpr void  swap( metadata& other ) noexcept { std::swap(label_,other.label_); }
    Label label_;
  };

  template<typename... Data>
  requires (std::same_as<Data, rbr::unknown_key> && ...)
  struct metadata<Data...>
  {
    static constexpr bool has_label = false;
    constexpr metadata(auto const&, rbr::concepts::settings auto const&) noexcept {}

    constexpr void  swap( metadata& ) noexcept {}
  };
}
