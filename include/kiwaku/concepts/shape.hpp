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
  namespace __
  {
    template<typename T>
    concept has_size = requires(T && t)
    {
      { t.size() } -> std::convertible_to<std::size_t>;
    };

    template<typename T>
    concept has_cosize = requires(T && t)
    {
      { t.cosize(std::declval<kumi::result::fill_t<kumi::size_v<T>, std::size_t>>()) } -> std::convertible_to<std::size_t>;
    };

    template<typename T>
    concept has_ndim = requires
    {
      { std::remove_cvref_t<T>::ndim } -> std::convertible_to<std::size_t>;
    };
  }

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type can be used as a shape.

    A type `T` models `kwk::concepts::shape` if it is a product type with a `ndim` static constant representing the
    number of components of the product type and an internal .size() function returning the total size the shape
    represents.

    ## Example types:
    + kwk::shape
  **/
  //====================================================================================================================
  template<typename T>
  concept shape = kumi::concepts::product_type<T> && kwk::concepts::__::has_ndim<T>
              && kwk::concepts::__::has_size<T> && !kwk::concepts::__::has_cosize<T>;

  template<typename T>
  concept layout = requires(T && t) { t.shape(); t.stride(); };
}
