//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/ct_helpers.hpp>
#include <cstddef>

namespace kwk
{
  namespace detail
  {
    template<typename Check, typename Dim0>
    constexpr bool all(Check c, Dim0 d0)
    {
      for(std::ptrdiff_t i0=0;i0<d0;++i0)
      {
        if(!c(i0))
          return false;
      }

      return true;
    }

    template<typename Check, typename Dim, typename... Dims>
    constexpr bool all(Check c, Dim d0, Dims... ds)
    {
      return detail::all( [c,d0](auto... is)
                          {
                            for(std::ptrdiff_t i0=0;i0<d0;++i0) if(!c(i0,is...)) return false;
                            return true;
                          }
                          , ds...
                        );
    }
  }

  //================================================================================================
  // n-Dimensionnal all algorithm
  //================================================================================================
  template<typename Check, auto Shaper>
  constexpr bool all(Check c, shape<Shaper> const& shp)
  {
    return [&]<std::size_t... N>(std::index_sequence<N...> const&)
    {
      return detail::all(c, get<N>(shp)... );
    }( std::make_index_sequence<shape<Shaper>::static_size>{});
  }
}
