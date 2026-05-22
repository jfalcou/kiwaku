//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <raberu/raberu.hpp>
#include <concepts>

namespace kwk::__
{
  struct label_ : rbr::as_keyword<label_>
  {
    template<std::convertible_to<std::string> T> auto operator=(T s) const noexcept
    {
      return rbr::option<label_,T>{std::move(s)};
    }

    template<typename Label> std::ostream& show(std::ostream& os, Label const& l) const
    {
      if constexpr( !std::same_as<Label,rbr::unknown_key> )
        return os << "Label: '" << l << '\'';
      else return os;
    }
  };
}

namespace kwk
{
  //================================================================================================
  //! @ingroup  settings
  //! @brief    Label setting for kwk::table and kwk::view
  //================================================================================================
  inline constexpr __::label_ label = {};
}
