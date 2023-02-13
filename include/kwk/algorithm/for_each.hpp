//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/detail/algorithm/for_each.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{
  //================================================================================================
  // Order N for_each algorithm
  //================================================================================================
  template<typename Func, concepts::container Container>
  constexpr auto for_each(Func&& f, Container&& c)
  {
    return [&]<std::size_t... N>(std::index_sequence<N...> const&)
    {
      return __::for_each(KWK_FWD(f), KWK_FWD(c), c.shape() );
    }( std::make_index_sequence<std::remove_cvref_t<Container>::static_order>{} );
  }

  //================================================================================================
  // Order N for_each_index algorithm
  //================================================================================================
  template<typename Func, concepts::container Container>
  constexpr auto for_each_index(Func&& f, Container&& c)
  {
    return [&]<std::size_t... N>(std::index_sequence<N...> const&)
    {
      return __::for_each_index(KWK_FWD(f), KWK_FWD(c), c.shape() );
    }( std::make_index_sequence<std::remove_cvref_t<Container>::static_order>{} );
  }
}
