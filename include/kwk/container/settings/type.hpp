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
#include <kwk/settings/type.hpp>
#include <kwk/settings/source.hpp>

namespace kwk::__
{
  // Views find their type from either their source or their type if present
  // source >> type however has we can't just cast the data on the fly
  // Views can't have BOTH source and type
  template<rbr::concepts::settings Settings>
  requires(Settings::contains(source) != Settings::contains(type))
  KWK_FORCEINLINE constexpr auto pick_(KWK_DELAY(), view_, type_, Settings const& opts)
  {
    if constexpr( Settings::contains(source) )
    {
      return as<typename source_traits<decltype(kwk::pick(source,opts))>::value_type>;
    }
    else
    {
      return opts[type];
    }
  }

  // Tables find their type int he type option OR the source if no type is used
  // Both options can coexist as table will just copy the source data in a block of type
  template<rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto pick_(KWK_DELAY(), table_, type_, Settings const& opts)
  {
    if constexpr( Settings::contains(type) )  return opts[type];
    else return as<typename source_traits<decltype(kwk::pick(source,opts))>::value_type>;
  }
}
