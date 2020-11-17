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
    template<typename Func, typename Dim0>
    constexpr void for_each(Func c, Dim0 d0)
    {
      for(std::ptrdiff_t i0=0;i0<d0;++i0) c(i0);
    }

    template<typename Func, typename Dim, typename... Dims>
    constexpr void for_each(Func c, Dim d0, Dims... ds)
    {

      detail::for_each( [c,d0](auto... is) { for(std::ptrdiff_t i0=0;i0<d0;++i0) c(i0,is...); }
                      , ds...
                      );
    }
  }

  //================================================================================================
  // n-Dimensionnal for_each algorithm
  //================================================================================================
  template<typename Func, auto Shaper>
  constexpr void for_each(Func c, shape<Shaper> const& shp)
  {
    return [&]<std::size_t... N>(std::index_sequence<N...> const&)
    {
      return detail::for_each(c, get<N>(shp)... );
    }( std::make_index_sequence<shape<Shaper>::static_size>{});
  }
}
