//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/category.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/detail/raberu.hpp>

namespace kwk::__
{
  struct category_ : rbr::as_keyword<category_>
  {
    constexpr auto operator=(concepts::category auto const& s) const noexcept { return s; }

    template<typename Category>
    std::ostream& display(std::ostream& os, Category const& s) const
    {
      return os << "Category: " << s;
    }
  };
}

namespace kwk
{
  //================================================================================================
  //! @ingroup  settings
  //! @brief    Category setting discriminating **KIWAKU** containers
  //================================================================================================
  inline constexpr __::category_ category = {};
}
