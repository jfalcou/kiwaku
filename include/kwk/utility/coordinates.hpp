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
  namespace __ { template <typename T> struct to_int<type_::info<T>> { using type = T; }; }

  //================================================================================================
  //! @ingroup utility
  //! @brief  Computes a tuple of coordinates from a linear index and a shape
  //!
  //! Behavior is undefined if idx is out of the index space defined by shp.
  //!
  //! @param idx  Linear index to convert into a nD coordinate set
  //! @param shp  Shape used as a reference
  //! @return A std::array<largest_integral<shape<D...>>, shape<D...>::static_order> containing the
  //! multi-dimensional position corresponding to idx
  //================================================================================================
  template<std::integral Idx,auto... D>
  KWK_CONST constexpr
  auto coordinates(Idx const idx, shape<D...> const shp) noexcept
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

    using coord_t = typename __::largest_integral<__::to_int_t<decltype(D)>...>::type;
    return [&]<int... i>(std::integer_sequence<int, i...>)
    {
      auto const strides{as_stride(shp)};

      return std::array { static_cast<coord_t>( idx / get<0  >(strides)                 )
                        , static_cast<coord_t>((idx / get<i+1>(strides)) % get<i+1>(shp))...
                        };
    }(std::make_integer_sequence<int, shape<D...>::static_order - 1>{});
  }
}
