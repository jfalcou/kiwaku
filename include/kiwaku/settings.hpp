//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_SETTINGS_HPP_INCLUDED
#define KIWAKU_SETTINGS_HPP_INCLUDED

#include <kiwaku/misc/settings.hpp>
#include <kiwaku/detail/options/shape_option.hpp>
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
    return extract_settings<detail::shape_tag>( s, detail::shape_option<2>{} );
  }

  //================================================================================================
  // Retrieve the stride from the settings. Use shape.as_stride() by default.
  //================================================================================================
  template<typename Settings> constexpr auto stride(Settings const& s) noexcept
  {
    return extract_settings<detail::stride_tag>( s, options::shape(s).as_stride() );
  }

  //================================================================================================
  // Retrieve the storage from the settings. Use dynamic_/stack_ by default depending on the shape
  // dynamic or constexpr status
  //================================================================================================
  template<typename Settings> constexpr auto storage(Settings const& s) noexcept
  {
    using shape_type = decltype(options::shape(s));
    if constexpr(shape_type::is_dynamic)
    {
      return extract_settings<detail::storage_tag>( s, dynamic_ );
    }
    else
    {
      return extract_settings<detail::storage_tag>( s, stack_ );
    }
  }
}

#endif
