//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_ALGORITHM_FOR_EACH_HPP_INCLUDED
#define KIWAKU_ALGORITHM_FOR_EACH_HPP_INCLUDED

namespace kwk
{
  namespace kwk
  {
    //================================================================================================
    // n-Dimensionnal for_each algorithm
    //================================================================================================
    template<typename Check, typename Dim0>
    consetxpr void for_each(Check c, Dim0 d0)
    {
      for(Dim0 i0=0;i0<d0;++i0) c(i0);
    }

    template<typename Check, typename Dim, typename... Dims>
    constexpr void for_each(Check c, Dim d0, Dims... ds)
    {
      detail::for_each( [c,d0](auto... is) { for(Dim i0=0;i0<d0;++i0) c(i0,is...); }, ds... );
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

#endif
