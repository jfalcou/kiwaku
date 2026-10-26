//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>
#include <kiwaku/concepts/shape.hpp>

namespace kwk::concepts
{
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
}
