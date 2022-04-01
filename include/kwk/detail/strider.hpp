//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kwk/detail/ct_helpers.hpp>
#include <type_traits>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <array>

namespace kwk::detail
{
  //==============================================================================================
  // Build the static storage of a stride depending on # of dimensions and unit dimensions
  //==============================================================================================
  template<typename T, auto UnitIndices>
  using stride_storage = std::array<T, UnitIndices.static_size - UnitIndices.size>;

  //==============================================================================================
  // Type representing a unit stride
  //==============================================================================================
  using unit_type = std::integral_constant<std::uint8_t,1>;

  //==============================================================================================
  // Compute an index_list from a pack of stride value types
  //==============================================================================================
  template<typename T, typename V, typename... Vs> struct index_map
  {
    using size_type                             = T;
    using map_type                              = typename type_map<unit_type, V, Vs...>::type;
    static constexpr std::ptrdiff_t size        = map_type::size;
    static constexpr std::ptrdiff_t static_size = sizeof...(Vs)+1;
    static constexpr bool is_implicit           = false;
    static constexpr bool is_unit               = map_type::contains(0);
  };

  template<typename T,std::size_t Dims> struct implicit_index_map
  {
    using size_type                             = T;
    using map_type                              = detail::index_list<0>;
    static constexpr std::ptrdiff_t size        = map_type::size;
    static constexpr std::ptrdiff_t static_size = Dims;
    static constexpr bool is_implicit           = true;
    static constexpr bool is_unit               = true;
  };

  //==============================================================================================
  // Convert indexes to linear position
  //==============================================================================================
  template<typename Data, std::size_t... Idx, std::integral... Is>
  constexpr auto linearize( std::index_sequence<Idx...> const&, Data const& d, Is... is ) noexcept
  {
    using std::get;
    if constexpr(sizeof...(Idx) > 0)  return ((is * get<Idx>(d)) + ...);
    else                              return 0;
  }
}
