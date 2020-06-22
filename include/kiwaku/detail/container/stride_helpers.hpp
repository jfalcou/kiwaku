//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_CONTAINER_STRIDE_HELPERS_HPP_INCLUDED
#define KIWAKU_DETAIL_CONTAINER_STRIDE_HELPERS_HPP_INCLUDED

#include <kiwaku/detail/ct_helpers.hpp>
#include <type_traits>
#include <cstddef>
#include <array>

namespace kwk::detail
{
  //================================================================================================
  // Build the static storage of a stride depending on # of dimensions and unit dimensions
  //================================================================================================
  template<std::size_t Dimensions, typename UnitIndices>
  using stride_storage = std::array<std::ptrdiff_t, Dimensions-UnitIndices::size>;

  //================================================================================================
  // Type representing a unit stride
  //================================================================================================
  using unit_type = std::integral_constant<std::ptrdiff_t,1>;

  //================================================================================================
  // Compute an index_list from a pack of stride value types
  //================================================================================================
  template<typename... Vs> struct index_map : type_map<unit_type, Vs...> {};
}

#endif
