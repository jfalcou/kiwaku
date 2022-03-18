//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/container/stride.hpp>
#include <kiwaku/detail/raberu.hpp>
#include <concepts>

namespace kwk
{
#if !defined(KWK_USE_DOXYGEN)
  struct strides_ : rbr::as_keyword<strides_>
  {
    // Extent from of_size(...)
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
#endif

  /**
    @ingroup  options
    @brief    Keyword for specifying strides options
   **/
  inline constexpr strides_ strides = {};
}
