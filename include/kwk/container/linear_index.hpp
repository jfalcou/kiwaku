//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kwk/container/shape.hpp>
#include <kwk/concept/range.hpp>
#include <kwk/detail/ct_helpers.hpp>
#include <kwk/detail/kumi.hpp>
#include <utility>

namespace kwk
{
  //================================================================================================
  //! @ingroup containers
  //! @brief  Computes a linear index from a order N list of indexes
  //!
  //! @param sh   Shape used as a reference
  //! @param idx  Individual list of indexes representing the order N index to linearise
  //! @return The linear index equivalent to (idx...) for the current shape
  //================================================================================================
  template<auto Shaper, std::integral... Index>
  auto linear_index( shape<Shaper> const& sh, Index... idx )
  {
    return sh.as_stride().index(idx...);
  }

  //================================================================================================
  //! @ingroup containers
  //! @brief  Computes a linear index from a tuple of N indexes
  //!
  //! @param sh   Shape used as a reference
  //! @param idx  Individual list of indexes representing the order N index to linearise
  //! @return The linear index equivalent to (idx...) for the current shape
  //================================================================================================
  template<auto Shaper, kumi::sized_product_type<Shaper.size()> Indexes>
  auto linear_index( shape<Shaper> const& sh, Indexes idx )
  {
    return kumi::apply( [&](auto... i) { return linear_index(sh,i...); }, idx);
  }

  //================================================================================================
  //! @ingroup containers
  //! @brief  Computes a linear index from a range of N  indexes
  //!
  //! @param sh   Shape used as a reference
  //! @param indexes  Range of integer representing the order N index to linearise
  //! @return The linear index equivalent to (indexes[0], ...) for the current shape
  //================================================================================================
  template<auto Shaper, kwk::concepts::range Indexes>
  requires( !kumi::sized_product_type<Indexes,Shaper.size()> )
  auto linear_index( shape<Shaper> const& sh, Indexes const& indexes)
  {
    return [&]<std::size_t... I>(std::index_sequence<I...>  const&)
    {
      auto            b     = indexes.begin();
      auto constexpr  deref = [](auto,auto& p) { return *p++; };

      return linear_index(sh, kumi::tuple{deref(I,b)...} );
    }(std::make_index_sequence<Shaper.size()>{});
  }
}
