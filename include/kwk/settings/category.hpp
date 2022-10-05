//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/category.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/detail/raberu.hpp>
#include <concepts>

namespace kwk::detail
{
  struct category_ : rbr::as_keyword<category_>
  {
    constexpr auto operator=(concepts::category auto const& s) const noexcept { return s; }

    template<typename Category>
    std::ostream& display(std::ostream& os, Category const&) const
    {
      return os << "Category: " << rbr::detail::type_name<Category>();
    }
  };

  // Premade categories
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
  //================================================================================================
  //! @ingroup  settings
  //! @brief    Category setting discriminating **KIWAKU** containers
  //================================================================================================
  inline constexpr detail::category_ category = {};

  /// kwk::view category setting
  inline constexpr detail::view_ view_ = {};

  /// kwk::table category setting
  inline constexpr detail::table_ table_ = {};
}
