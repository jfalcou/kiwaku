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
#include <kumi/kumi.hpp>
#include <kwk/detail/stdfix.hpp>
#include <kwk/utility/container/stride.hpp>
#include <utility>

namespace kwk
{
  template<auto... S> struct shape;

  //================================================================================================
  //! @ingroup utility
  //! @brief  Computes a linear index from a order N list of indexes
  //!
  //! @param sh   Shape used as a reference
  //! @param idx  Individual list of indexes representing the order N index to linearize
  //! @return The linear index equivalent to (idx...) for the current shape
  //================================================================================================
  template<auto... S, std::integral... Index>
  KWK_CONST constexpr auto linear_index( shape<S...> sh, Index... idx ) noexcept
  {
    KIWAKU_ASSERT ( sh.contains(idx...)
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
  template<auto... S, kumi::sized_product_type<shape<S...>::static_order> Indexes>
  KWK_CONST constexpr auto linear_index( shape<S...> sh, Indexes idx ) noexcept
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
  template<auto... S, kwk::concepts::range Indexes>
  requires( !kumi::sized_product_type<Indexes,shape<S...>::static_order> )
  constexpr auto linear_index( shape<S...> const& sh, Indexes const& indexes) noexcept
  {
    return [&]<std::size_t... I>(std::index_sequence<I...>  const&)
    {
      auto            b     = indexes.begin();
      auto constexpr  deref = [](auto,auto& p) { return *p++; };

      return linear_index(sh, kumi::tuple{deref(I,b)...} );
    }(std::make_index_sequence<shape<S...>::static_order>{});
  }
}
