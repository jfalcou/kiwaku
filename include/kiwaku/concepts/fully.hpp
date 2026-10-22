//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>
#include <kiwaku/concepts/static_constant.hpp>
#include <kiwaku/concepts/extent.hpp>

namespace kwk::concepts
{
  template<typename... Ts>
  using static_trait = std::bool_constant<(kwk::concepts::static_constant<Ts> && ...)>;

  template<typename... Ts>
  using dynamic_trait = std::bool_constant<(kwk::concepts::dynamic_extent<Ts> && ...)>;

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept checking that a product type only holds dynamic integral values.

    A type `T` models `kwk::concepts::fully_dynamic` if it is a product type with every members modeling
    `kwk::concepts::dynamic_extent` once flattened.
  **/
  //====================================================================================================================
  template<typename T>
  concept fully_dynamic = kumi::concepts::product_type<T> && kumi::apply_traits_t<dynamic_trait, kumi::result::flatten_all_t<T>>::value;

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept checking that a product type only holds static integral values.

    A type `T` models `kwk::concepts::fully_static` if it is a product type with every members modeling
    `kwk::concepts::static_constant` one flattened.
  **/
  //====================================================================================================================
  template<typename T>
  concept fully_static = kumi::concepts::product_type<T> && kumi::apply_traits_t<static_trait, kumi::result::flatten_all_t<T>>::value;
}
