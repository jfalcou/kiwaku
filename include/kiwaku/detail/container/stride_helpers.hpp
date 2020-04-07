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

#include <type_traits>
#include <cstddef>
#include <array>

namespace kwk::detail
{
  //================================================================================================
  // Augmented static integer list for mapping known unit values
  //================================================================================================
  template<std::size_t... I> struct index_list
  {
    // How many indexes ?
    static constexpr std::size_t size = sizeof...(I);

    // is N in the set of known unit indexes ?
    static constexpr bool contains(std::size_t N) noexcept
    {
      bool found[] = { (I==N)... };
      for(auto f : found) if(f) return true;
      return false;
    }

    // Aggregation of an index into an existing index_list
    template<std::size_t J> using append = index_list<I...,J>;

    // Find the actual dynamic index of a non-unit index
    template<std::size_t Size> static constexpr std::size_t locate(std::size_t N) noexcept
    {
      // Build a bitmap of where the known values are
      std::size_t idx[Size] = {};
      ((idx[I] = 1), ...);

      // Count how far you need to go to find an unknown
      std::size_t pos = 0;
      for(std::size_t i=0;i<N;++i) pos += (1-idx[i]);

      return pos;
    }
  };

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
  template<std::size_t Index, typename Current, typename... Vs> struct index_map_impl;

  template<std::size_t Index, typename Current>
  struct index_map_impl<Index, Current, unit_type>
  {
    using type = typename Current::template append<Index>;
  };

  template<std::size_t Index, typename Current, typename Head>
  struct index_map_impl<Index, Current, Head>
  {
    using type = Current;
  };

  template<std::size_t Index, typename Current, typename Head, typename... Tail>
  struct  index_map_impl<Index, Current, Head, Tail...>
        : index_map_impl<Index+1,Current,Tail...>
  {
  };

  template<std::size_t Index, typename Current, typename... Tail>
  struct  index_map_impl<Index, Current, unit_type, Tail...>
        : index_map_impl<Index+1,typename Current::template append<Index>,Tail...>
  {
  };

  template<typename... Vs>
  struct index_map : index_map_impl<0, index_list<>, Vs...>
  {};
}

#endif
