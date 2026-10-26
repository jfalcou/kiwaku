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
#include <kiwaku/concepts/arithmetic_value.hpp>

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
}
