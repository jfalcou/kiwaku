//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/container/shape.hpp>
#include <kiwaku/detail/raberu.hpp>
#include <concepts>

namespace kwk
{
#if !defined(KWK_USE_DOXYGEN)
  struct label_ : rbr::as_keyword<label_>
  {
    template<std::convertible_to<std::string> T> auto operator=(T s) const noexcept
    {
      return rbr::option<label_,T>{std::move(s)};
    }

    // Small lie for optimization purpose
    auto operator=(rbr::unknown_key) const noexcept
    {
      return rbr::option<label_,rbr::unknown_key>{};
    }

    template<typename Label> std::ostream& show(std::ostream& os, Label const& l) const
    {
      os << "Label: ";
      if constexpr( std::same_as<Label,rbr::unknown_key> )  return os << "None";
      else                                                  return os << '\'' << l << '\'';
    }
  };
#endif

  /**
    @ingroup  options
    @brief    Keyword for specifying label
   **/
  inline constexpr label_ label = {};
}
