//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_MISC_SETTINGS_HPP_INCLUDED
#define KIWAKU_MISC_SETTINGS_HPP_INCLUDED

#include <kiwaku/detail/options/settings_helpers.hpp>

namespace kwk
{
  //================================================================================================
  // Build an option pack from a list of settings
  //================================================================================================
  template<typename... Os> constexpr auto settings(Os... opts) noexcept
  {
    return detail::aggregator{detail::link<typename Os::option_tag>(opts)...};
  }

  //================================================================================================
  // Retrieve an option from a settings or a single option
  //================================================================================================
  template<typename OptionTag, typename S, typename Default>
  constexpr auto extract_settings(S const& s, Default const& d) noexcept
  {
    if constexpr(detail::is_option<S>::value)  return s(OptionTag{}, d);
    else                                       return extract_settings<OptionTag>(settings(s), d);
  }
}

#endif
