//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>
#include <kiwaku/traits/wildcard.hpp>
#include <kiwaku/concepts/value.hpp>

namespace kwk::concepts
{
  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent an extent.

    A type `T` models `kwk::concepts::extent` if it models : `kwk::static_constant` with inner type
    modeling `std::integral` or is simply models `std::integral`.

    ## Example types:
    + std::integral_constant<std::size_t,1>;
    + kwk::fixed<1>;
    + kwk::_
    + int
  **/
  //====================================================================================================================
  template<typename T>
  concept extent = kwk::is_wildcard_v<std::remove_cvref_t<T>> || integral_arithmetic_value<T>;

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent an extent.

    A type `T` models `kwk::concepts::dynamic_extent` if it models : `kwk::static_constant` with
  inner type modeling `std::integral` or is simply models `std::integral`.

    ## Example types:
    + kwk::_
    + int
  **/
  //====================================================================================================================
  template<typename T>
  concept dynamic_extent =
      kwk::is_wildcard_v<std::remove_cvref_t<T>> || std::integral<std::remove_cvref_t<T>>;

  template<typename T> inline constexpr bool is_nested_extent_v = extent<T>;

  template<typename... Ts>
  using nested_extent = std::bool_constant<(is_nested_extent_v<Ts> && ...)>;

  template<kumi::concepts::product_type T>
  inline constexpr bool is_nested_extent_v<T> =
      kumi::concepts::empty_product_type<T> ? false : kumi::apply_traits_t<nested_extent, T>::value;
  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent a potentially nested extent.

    A type `T` models `kwk::concepts::deep_extent` if it models : `kwk::extent` or is a nested
  product type of `extents`

    ## Example types:
    + kwk::_
    + int
    + kumi::tuple<int, int, kumi::tuple<int>>;
  **/
  //====================================================================================================================
  template<typename T>
  concept deep_extent = is_nested_extent_v<T>;
}
