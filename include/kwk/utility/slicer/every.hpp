//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/kumi.hpp>
#include <kwk/utility/fixed.hpp>
#include <ostream>

namespace kwk
{
  template<auto Desc> struct shape;

  template<typename T> struct every
  {
    T every_;
    every(T b) : every_(b)
    {
      // Checks that every is not null
      if constexpr (requires{ T::value; })
      {
        static_assert ( static_cast<ptrdiff_t>(T::value) >= 0
                      , "[kwk] - kwk::every must be strictly positive"
                      );
      }
      else
      {
        KIWAKU_ASSERT ( static_cast<ptrdiff_t>(every_) >= 0
                      , "[kwk] - kwk::every("
                        << every_ << ") must be strictly positive"
                      );
      }
    }

    friend std::ostream& operator<<(std::ostream& os, every b)
    {
      return os << "every(" << +b.every_ << ")";
    }
  };
  
  template<typename T> every(T) -> every<T>;

  template<auto Desc, typename T, std::size_t N>
  constexpr auto reshape( shape<Desc> const& sh
                        , every<T> e
                        , kumi::index_t<N> const&
                        ) noexcept
  {
    // Static dimension and static every
    if constexpr (is_static_extent_v<N,Desc> && requires{ T::value; })
    {
      constexpr auto vn = get<N>(Desc);
      return fixed<(vn == 0) ? 0 : (1 + vn / T::value)>;
    }
    else
    {
      auto vn = get<N>(sh);
      return (vn == 0) ? 0 : (1 + vn / e.every_);
    }
  }
}
