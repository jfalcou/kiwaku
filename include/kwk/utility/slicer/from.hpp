//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/values.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/utility/end.hpp>
#include <kwk/utility/fixed.hpp>
#include <ostream>

namespace kwk
{
  template<auto Desc> struct shape;

  template<typename T> struct from
  {
    T begin;
    from(T b) : begin(b)
    {
      if constexpr( concepts::static_constant<T> )
      {
        static_assert ( static_cast<std::ptrdiff_t>(T::value) >= 0
                      , "[kwk] - Out of bound index for kwk::from"
                      );
      }

      if constexpr( std::integral<T> )
      {
        KIWAKU_ASSERT ( static_cast<std::ptrdiff_t>(begin) >= 0
                      , "[kwk] - Out of bound index for kwk::from("
                        << begin << ")"
                      );
      }
    }

    friend std::ostream& operator<<(std::ostream& os, from f)
    {
      os << "from(";
      if constexpr(std::integral<decltype(f.begin)>)  os << +f.begin;
      else                                            os << f.begin;
      return os<< ")";
    }
  };

  template<typename T> from(T) -> from<T>;

  template<auto Desc, typename T, std::size_t N>
  constexpr auto reshape( shape<Desc> const& sh
                        , from<T> f
                        , kumi::index_t<N> const&
                        ) noexcept
  {
    if constexpr(concepts::extremum<T>)
    {
      // from(end - k) -> k
      const auto o = offset(f.begin);

      KIWAKU_ASSERT ( static_cast<decltype(get<N>(sh))>(o) <= get<N>(sh)
                    , "[kwk] - Out of bound index for: " <<f
                    );

      return o;
    }
    else
    {
      // from(k) -> sh[N] - k
      return sh.template extent<N>() - f.begin;
    }
  }
}
