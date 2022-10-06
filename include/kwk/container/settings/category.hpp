//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/settings/category.hpp>

// Pre-made categories
namespace kwk::detail
{
  struct view_
  {
    using stored_value_type       = view_;
    using keyword_type            = category_;
    using kwk_container_category  = void;
    KWK_FORCEINLINE constexpr auto operator()(keyword_type const&) const noexcept { return *this; }
  };

  struct table_
  {
    using stored_value_type       = table_;
    using keyword_type            = category_;
    using kwk_container_category  = void;
    KWK_FORCEINLINE constexpr auto operator()(keyword_type const&) const noexcept { return *this; }
  };
}

namespace kwk
{
  /// kwk::view category setting
  inline constexpr detail::view_ view_    = {};

  /// kwk::table category setting
  inline constexpr detail::table_ table_  = {};
}
