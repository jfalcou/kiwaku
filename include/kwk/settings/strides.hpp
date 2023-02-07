//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/utility/container/stride.hpp>
#include <kwk/detail/raberu.hpp>
#include <concepts>

namespace kwk::__
{
  struct strides_ : rbr::as_keyword<strides_>
  {
    template<auto Strider> auto operator=(stride<Strider> const& s) const noexcept
    {
      return s;
    }

    // Display
    template<typename Strides> std::ostream& show(std::ostream& os, Strides const& s) const
    {
      return os << "Strides: " << s;
    }
  };
}

namespace kwk
{
  //================================================================================================
  //! @ingroup  settings
  //! @brief    Stride setting for kwk::table and kwk::view
  //================================================================================================
  inline constexpr __::strides_ strides = {};
}
