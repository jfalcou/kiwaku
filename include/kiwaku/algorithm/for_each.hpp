//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

namespace kwk
{
  namespace detail
  {
    //================================================================================================
    // n-Dimensional for_each algorithm
    //================================================================================================
    template<typename Check, typename Dim0>
    consetxpr void for_each(Check c, Dim0 d0)
    {
      for(std::ptrdiff_t i0=0;i0<d0;++i0) c(i0);
    }

    template<typename Check, typename Dim, typename... Dims>
    constexpr void for_each(Check c, Dim d0, Dims... ds)
    {
      detail::for_each( [c,d0](auto... is) { for(std::ptrdiff_t i0=0;i0<d0;++i0) c(i0,is...); }
                      , ds...
                      );
    }
  }

  //================================================================================================
  // n-Dimensionnal for_each algorithm
  //================================================================================================
  template<typename Check, std::size_t Dimensions>
  constexpr void for_each(Check c, shape<Dimensions> const& shp)
  {
    return [&]<std::size_t... N>(std::index_sequence<N...> const&)
    {
      return detail::for_each(c, get<N>(shp)... );
    }( std::make_index_sequence<Dimensions>{});
  }
}
