//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : EVE Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/detail/assert.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/utility/container/shape.hpp>
#include <kwk/utility/container/stride.hpp>
#include <concepts>

namespace kwk
{
  //================================================================================================
  //! @ingroup utility
  //! @brief  Computes a tuple of coordinates from a linear index and a shape
  //!
  //! Behavior is undefined if idx is out of the index space defined by shp.
  //!
  //! @param idx  Linear index to convert into a nD coordinate set
  //! @param shp  Shape used as a reference
  //! @return A tuple containing the multi-dimensional position corresponding to idx
  //================================================================================================
  template<std::integral T, auto Desc>
  KWK_FORCEINLINE constexpr auto coordinates(T idx, shape<Desc> const& shp) noexcept
  {
    KIWAKU_ASSERT ( idx < shp.numel()
                  ,   "Converting index " << idx
                  <<  " to an out of bounds coordinates for shape " << shp
                  );

    constexpr auto nbdims = shape<Desc>::size();

    // This save some 1-200ms of compile-time for those very common cases
    if      constexpr(nbdims == 1)  return kumi::tuple{idx};
    else if constexpr(nbdims == 2)  return kumi::tuple{idx/shp[1],idx%shp[1]};
    else
    {
      return  kumi::fold_right( [&](auto acc, auto m)
                                {
                                  auto that = kumi::push_back(acc,idx/m);
                                  idx %= static_cast<T>(m);
                                  return that;
                                }
                              , as_stride(shp)
                              , kumi::tuple<>{}
                              );
    }
  }
}
