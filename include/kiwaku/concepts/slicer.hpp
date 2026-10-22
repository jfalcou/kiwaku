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
    @brief Concept specifying a type can be used as a slicer.

    A type `T` models `kwk::concepts::slicer` if there exist a free function called `to_slicer` taking that can be found
    via @ref{adl} on the input type.

    ## Example types:
    + kwk::extremum
    + kwk::slicer
    + kwk::fixed<1>
    + int
  **/
  //====================================================================================================================
  template<typename T>
  concept slicer = requires(T const& t) { to_slicer(t); };
}
