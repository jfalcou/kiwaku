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
    concept has_size = requires(T&& t) {
      { t.size() } -> std::convertible_to<std::size_t>;
    };

    template<typename T>
    concept has_cosize = requires(T&& t) {
      {
        t.cosize(std::declval<kumi::result::fill_t<kumi::size_v<T>, std::size_t>>())
      } -> std::convertible_to<std::size_t>;
    };

    template<typename T>
    concept has_ndim = requires {
      { std::remove_cvref_t<T>::ndim } -> std::convertible_to<std::size_t>;
    };

    template<typename T>
    concept layout_like = requires(T&& t) {
      t.shape();
      t.stride();
    };
  }

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type can be used as a shape.

    A type `T` models `kwk::concepts::shape` if it is a product type with a `ndim` static constant
  representing the number of components of the product type and an internal .size() function
  returning the total size the shape represents.

    ## Example types:
    + kwk::shape
  **/
  //====================================================================================================================
  template<typename T>
  concept shape = kumi::concepts::product_type<T> && kwk::concepts::__::has_ndim<T> &&
                  kwk::concepts::__::has_size<T> && !kwk::concepts::__::has_cosize<T>;

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type can be used as a stride.

    A type `T` models `kwk::concepts::stride` if it is a product type with a `ndim` static constant
  representing the number of components of the product type and an internal .cosize(shape) function
  returning the total cosize of the given shape, ie : what is the number of elements that this
  stride can represent.

    ## Example types:
    + kwk::stride
  **/
  //====================================================================================================================
  template<typename T>
  concept stride = kumi::concepts::product_type<T> && kwk::concepts::__::has_ndim<T> &&
                   kwk::concepts::__::has_cosize<T>;

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type can be used as a layout.

    A type `T` models `kwk::concepts::layout` if it is a type containing a `kwk::shape` and a
  `kwk::stride` on which we can do arithmetic.

    ## Example types:
    + kwk::layout
  **/
  //====================================================================================================================
  template<typename T>
  concept layout = __::layout_like<T> && requires(T&& a, T&& b) {
    { a + b } -> __::layout_like;
    { a - b } -> __::layout_like;
    { a * b } -> __::layout_like;
    { a / b } -> __::layout_like;
    { a % b } -> __::layout_like;
    { a && b } -> __::layout_like;
    { a || b } -> __::layout_like;
  };
}
