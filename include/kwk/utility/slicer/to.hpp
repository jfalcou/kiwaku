//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/assert.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/utility/fixed.hpp>
#include <ostream>

namespace kwk
{
  template<auto Desc> struct shape;

  template<typename T> struct to
  {
    T end;
    to(T e) : end(e) {}

    friend std::ostream& operator<<(std::ostream& os, to t)
    {
      return os << "to(" << +t.end << ")";
    }
  };
  
  template<typename T> to(T) -> to<T>;

  template<auto Desc, typename T, std::size_t N>
  constexpr auto reshape( [[maybe_unused]] shape<Desc> const& sh
                        , to<T> f
                        , kumi::index_t<N> const&
                        ) noexcept
  {
    if constexpr (is_static_extent_v<N,Desc> && requires{ T::value; })
    {
      static_assert ( static_cast<ptrdiff_t>(T::value) <= static_cast<ptrdiff_t>(get<N>(Desc))
                    , "[kwk] - Out of bound index for kwk::to"
                    );
    }
    else
    {
      KIWAKU_ASSERT ( static_cast<ptrdiff_t>(f.end) <= static_cast<ptrdiff_t>(get<N>(sh))
                    , "[kwk] - Out of bound index for kwk::to("
                      << f.end << ") for size: " << get<N>(sh)
                    );
    }

    return f.end;
  }
}
