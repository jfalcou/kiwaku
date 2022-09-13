//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <cstddef>
#include <utility>

namespace kwk
{
  namespace detail
  {
    //================================================================================================
    // n-Dimensional for_each algorithm
    //================================================================================================
    template<typename Func, typename Dim, typename Container>
    constexpr void for_each(Func f, Container&& c, Dim d)
    {
      for(std::ptrdiff_t i=get<0>(d);i<get<1>(d);++i) f(KWK_FWD(c), i);
    }

    template<typename Func, typename Container, typename Dim, typename... Dims>
    constexpr void for_each(Func f, Container&& c, Dim d0, Dims... ds)
    {
      detail::for_each( [f,d0](auto&& x, auto... is)
                        {
                          for(std::ptrdiff_t i=get<0>(d0);i<get<1>(d0);++i)
                            f(KWK_FWD(x), i,is...);
                        }
                      , KWK_FWD(c)
                      , ds...
                      );
    }
  }

  //================================================================================================
  // Order N for_each algorithm
  //================================================================================================
  template<typename Func, typename Container>
  constexpr void for_each(Func f, Container&& c)
  {
    return [&]<std::size_t... N>(std::index_sequence<N...> const&)
    {
      return detail::for_each(f, KWK_FWD(c), kumi::tuple{first<N>(c),last<N>(c)}... );
    }( std::make_index_sequence<std::remove_cvref_t<Container>::static_order>{} );
  }
}
