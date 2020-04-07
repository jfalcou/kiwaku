//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_CONTAINER_LINEARIZE_HPP_INCLUDED
#define KIWAKU_DETAIL_CONTAINER_LINEARIZE_HPP_INCLUDED

#include <utility>
#include <cstddef>

namespace kwk::detail
{
  //================================================================================================
  // Recurring linearize pattern: used y stride and other components
  //================================================================================================
  template<typename Data, std::size_t... Idx, typename... Is>
  constexpr auto linearize( std::index_sequence<Idx...> const&, Data const& d, Is... is ) noexcept
  {
    using std::get;
    if constexpr(sizeof...(Idx) > 0)  return ((is * get<Idx>(d)) + ...);
    else                              return 0;
  }
}

#endif
