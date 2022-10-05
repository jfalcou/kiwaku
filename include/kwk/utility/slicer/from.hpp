//==================================================================================================
/**
  KIWAQOU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/kumi.hpp>
#include <kwk/utility/fixed.hpp>

namespace kwk
{
  template<auto Desc> struct shape;

  template<typename T> struct from
  {
    T begin;
    from(T b) : begin(b)
    {
      if constexpr (requires{ T::value; })
      {
        static_assert ( static_cast<ptrdiff_t>(T::value) >= 0
                      , "[kwk] - Out of bound index for kwk::from"
                      );
      }
      else
      {
        KIWAKU_ASSERT ( static_cast<ptrdiff_t>(begin) >= 0
                      , "[kwk] - Out of bound index for kwk::from("
                        << begin << ")"
                      );
      }
    }
  };
  
  template<typename T> from(T) -> from<T>;

  template<auto Desc, typename T, std::size_t N>
  constexpr auto reshape( shape<Desc> const& sh
                        , from<T> f
                        , kumi::index_t<N> const&
                        ) noexcept
  {
    if constexpr (is_static_extent_v<N,Desc> && requires{ T::value; })
    {
      return fixed<get<N>(Desc) - T::value>;
    }
    else
    {
      return get<N>(sh) - f.begin;
    }
  }
}
