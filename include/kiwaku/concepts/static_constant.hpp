//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>

namespace kwk::concepts
{
  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent a compile time static constant.

    A type `T` models `kwk::concepts::static_constant` if it holds a `value_type` alias and a static `value` member.

    ## Example types:
    + std::integral_constant<std::size_t,1>;
    + kwk::fixed<1>
  **/
  //====================================================================================================================
  template<typename T>
  concept static_constant = requires(T) {
    { std::remove_cvref_t<T>::value };
    typename std::remove_cvref_t<T>::value_type;
  };

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent a compile time static arithmetic constant.

    A type `T` models `kwk::concepts::arithmetic_constant` if it models `kwk::static_constant` which internal member
    models either `std::integral` or `std::floating_point`.

    ## Example types:
    + std::integral_constant<std::size_t,1>;
    + std::integral<float, 3.f>;
    + kwk::fixed<1>;
  **/
  //====================================================================================================================
  template<typename T>
  concept arithmetic_constant = static_constant<T> && requires(T) {
    requires std::integral<typename std::remove_cvref_t<T>::value_type> ||
               std::floating_point<typename std::remove_cvref_t<T>::value_type>;
  };
}
