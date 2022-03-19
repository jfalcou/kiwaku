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
    template<typename Func, typename Dim0, typename Container>
    constexpr void for_each(Func f, Container&& c, Dim0 d0)
    {
      for(std::ptrdiff_t i0=0;i0<d0;++i0) f(KWK_FWD(c), i0);
    }

    template<typename Func, typename Container, typename Dim, typename... Dims>
    constexpr void for_each(Func f, Container&& c, Dim d0, Dims... ds)
    {
      detail::for_each( [f,d0](auto&& x, auto... is)
                        {
                          for(std::ptrdiff_t i0=0;i0<d0;++i0)
                            f(KWK_FWD(x), i0,is...);
                        }
                      , KWK_FWD(c)
                      , ds...
                      );
    }
  }

  //================================================================================================
  // Rank N for_each algorithm
  //================================================================================================
  template<typename Func, typename Container>
  constexpr void for_each(Func f, Container&& c)
  {
    return [&]<std::size_t... N>(std::index_sequence<N...> const&)
    {
      auto shp = c.shape();
      return detail::for_each(f, KWK_FWD(c), get<N>(shp)... );
    }( std::make_index_sequence<std::remove_cvref_t<Container>::static_rank>{});
  }
}
