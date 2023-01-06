//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/range.hpp>
#include <kwk/detail/assert.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/utility/container/stride.hpp>
#include <utility>

namespace kwk
{
  //================================================================================================
  //! @ingroup utility
  //! @brief  Computes a linear index from a order N list of indexes
  //!
  //! @param sh   Shape used as a reference
  //! @param idx  Individual list of indexes representing the order N index to linearize
  //! @return The linear index equivalent to (idx...) for the current shape
  //================================================================================================
  template<auto Shaper, std::integral... Index>
  constexpr auto linear_index( shape<Shaper> const& sh, Index... idx ) noexcept
  {
    KIWAKU_ASSERT ( sh.strictly_contains(idx...)
                  , "Linearizing out of bounds indexes "
                    << kumi::tuple{idx...} << " within shape " << sh
                  );
    return as_stride(sh).linearize(idx...);
  }

  //================================================================================================
  //! @ingroup utility
  //! @brief  Computes a linear index from a tuple of N indexes
  //!
  //! @param sh   Shape used as a reference
  //! @param idx  Individual list of indexes representing the order N index to linearize
  //! @return The linear index equivalent to (idx...) for the current shape
  //================================================================================================
  template<auto Shaper, kumi::sized_product_type<Shaper.size()> Indexes>
  constexpr auto linear_index( shape<Shaper> const& sh, Indexes idx ) noexcept
  {
    return kumi::apply( [&](auto... i) { return linear_index(sh,i...); }, idx);
  }

  //================================================================================================
  //! @ingroup utility
  //! @brief  Computes a linear index from a range of N indexes
  //!
  //! @param sh      Shape used as a reference
  //! @param indexes Range of integer representing the order N index to linearize
  //! @return The linear index equivalent to (indexes[0], ...) for the current shape
  //================================================================================================
  template<auto Shaper, kwk::concepts::range Indexes>
  requires( !kumi::sized_product_type<Indexes,Shaper.size()> )
  constexpr auto linear_index( shape<Shaper> const& sh, Indexes const& indexes) noexcept
  {
    return [&]<std::size_t... I>(std::index_sequence<I...>  const&)
    {
      auto            b     = indexes.begin();
      auto constexpr  deref = [](auto,auto& p) { return *p++; };

      return linear_index(sh, kumi::tuple{deref(I,b)...} );
    }(std::make_index_sequence<Shaper.size()>{});
  }
}
