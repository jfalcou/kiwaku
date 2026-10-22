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
    @brief Concept specifying a type represent an extremum.

    A type `T` models `kwk::concepts::extremum` if it has a function that can return a value convertible to
  std::ptrdiff_t to be used as an index.

    ## Example types:
    + kwk::extremum

  **/
  //====================================================================================================================
  template<typename T>
  concept extremum = requires(T x, std::ptrdiff_t I) {
    { x(I) } -> std::convertible_to<std::ptrdiff_t>;
  };
}
