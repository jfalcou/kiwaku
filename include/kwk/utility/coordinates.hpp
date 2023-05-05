//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/detail/assert.hpp>
#include <kwk/utility/container/shape.hpp>
#include <kwk/utility/container/stride.hpp>
#include <array>
#include <concepts>
#include <type_traits>

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
  //! @return A std::array<shape<Desc>::value_type, shape<Desc>::static_order> containing the
  //! multi-dimensional position corresponding to idx
  //================================================================================================
  template<std::integral Idx,auto... Desc>
  KWK_CONST constexpr
  auto coordinates( Idx idx, shape<Desc...> const shp) noexcept
  {
    KIWAKU_ASSERT ( idx < shp.numel()
                  ,   "Converting index " << idx
                  <<  " to an out of bounds coordinates for shape " << shp
                  );

    /* leftmost/outer dimension does not require the modulo operation, example for 4 dimensions:
         idx / (shp[1] * shp[2] * shp[3])          ,
        (idx / (         shp[2] * shp[3])) % shp[1],
        (idx /                    shp[3] ) % shp[2],
         idx                               % shp[3],
    */
    return [=]<int...i>(std::integer_sequence<int, i...>)
    {
      auto const strides{as_stride(shp)};
      return std::array<Idx, shape<Desc...>::static_order>
      {
        static_cast<Idx>( idx / get<0  >(strides)                 ),
        static_cast<Idx>((idx / get<i+1>(strides)) % get<i+1>(shp))...
      };
    }(std::make_integer_sequence<int, shape<Desc...>::static_order - 1 >{});
  }
}
