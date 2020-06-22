//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/misc/settings.hpp>
#include <kiwaku/detail/container/shape_helpers.hpp>
#include <kiwaku/detail/options/stride_option.hpp>
#include <kiwaku/detail/options/storage_option.hpp>

namespace kwk::options
{
  //================================================================================================
  // Retrieve the shape from the settings. Use _2D by default.
  //================================================================================================
  template<typename Settings>
  constexpr auto shape(Settings const& s) noexcept
  {
    using shaper_t = detail::shaper<detail::dynamic_size,detail::dynamic_size>;
    return extract_settings<detail::shape_tag>( s, shaper_t{} );
  }

  //================================================================================================
  // Retrieve the stride from the settings. Use shape.as_stride() by default.
  //================================================================================================
  template<typename Settings, typename Default>
  constexpr auto stride(Settings const& s, Default const& st) noexcept
  {
    return extract_settings<detail::stride_tag>( s, st.as_stride() );
  }

  //================================================================================================
  // Retrieve the storage from the settings. Use dynamic_/stack_ by default depending on the shape
  // dynamic or constexpr status
  //================================================================================================
  template<typename Settings> constexpr auto storage(Settings const& s) noexcept
  {
    using shape_type = decltype(options::shape(s));
    if constexpr(!shape_type::is_fully_static)
    {
      return extract_settings<detail::storage_tag>( s, dynamic_ );
    }
    else
    {
      return extract_settings<detail::storage_tag>( s, stack_ );
    }
  }
}
