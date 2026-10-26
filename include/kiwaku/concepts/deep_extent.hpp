//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>
#include <kiwaku/concepts/extent.hpp>

namespace kwk::concepts
{
  template<typename T> consteval auto is_nested_extent()
  {
    if constexpr(kumi::concepts::empty_product_type<T>) return std::bool_constant<false> {};
    else if constexpr(kumi::concepts::product_type<T>)
    {
      constexpr auto count = kumi::size_v<T>;
      return []<std::size_t... I>(std::index_sequence<I...>)
      {
        return std::bool_constant<(is_nested_extent<kumi::element_t<I, T>>() && ...)> {};
      }(std::make_index_sequence<count> {});
    }
    else return std::bool_constant<extent<T>> {};
  }

  template<typename T>
  inline constexpr bool is_nested_extent_v = decltype(is_nested_extent<T>())::value;

  template<typename T>
  concept deep_extent = is_nested_extent_v<T>;
}
