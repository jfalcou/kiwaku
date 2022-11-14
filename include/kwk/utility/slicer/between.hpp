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

  template<typename B, typename E> struct between
  {
    B begin;
    E end;
    between(B b, E e) : begin(b), end(e)
    {
      // Asserts that begin > end.
      
      if constexpr (requires{ B::value; } && requires{ E::value; })
      {
        auto bv = fixed<static_cast<ptrdiff_t>(B::value)>;
        const auto ev = static_cast<ptrdiff_t>(E::value);
        // TODO: const or auto seems to work

        static_assert ( bv < ev
                      , "[kwk] - Out of bound index for kwk::between (begin >= end)"
                      );
      }
      else
      {
        auto bv = static_cast<ptrdiff_t>(begin);
        auto ev = static_cast<ptrdiff_t>(end);

        KIWAKU_ASSERT ( bv < ev
                      , "[kwk] - Out of bound index for kwk::between (begin("
                        << +begin << ") >= end("
                        << +end << "))"
                      );
      }
    }

    friend std::ostream& operator<<(std::ostream& os, between b)
    {
      return os << "between(" << +b.begin << ", " << +b.end << ")";
    }
  };
  
  template<typename B, typename E> between(B, E) -> between<B, E>;

  template<auto Desc, typename B, typename E, std::size_t N>
  constexpr auto reshape( [[maybe_unused]] shape<Desc> const& sh
                        , between<B, E> b
                        , kumi::index_t<N> const&
                        ) noexcept
  {
    // Static dimension
    if constexpr (is_static_extent_v<N,Desc>)
    {
      const auto dim_size = fixed<static_cast<ptrdiff_t>(get<N>(Desc))>;

      // If begin is static too, checks validity
      if constexpr (requires{ B::value; })
      {
        auto bv = fixed<static_cast<ptrdiff_t>(B::value)>;

        static_assert ( (bv >= 0) && (bv < dim_size)
                      , "[kwk] - Out of bound index for kwk::between (begin)"
                      );
      }
      else 
      {
        auto bv = static_cast<ptrdiff_t>(b.begin);

        KIWAKU_ASSERT ( bv < dim_size // ?? static_cast<ptrdiff_t>(get<N>(sh))
                      , "[kwk] - Out of bound index for kwk::between: begin("
                        << b.begin << ") exceeds or equals dim size: " << dim_size
                      );
      }

      // If end is static too, checks validity
      // TODO : factorize this code (begin & end)
      if constexpr (requires{ E::value; })
      {
        auto ev = fixed<static_cast<ptrdiff_t>(E::value)>;

        static_assert ( (ev >= 0) && (ev <= dim_size)
                      , "[kwk] - Out of bound index for kwk::between (end)"
                      );
      }
      else 
      {
        auto ev = static_cast<ptrdiff_t>(b.end);

        KIWAKU_ASSERT ( ev <= dim_size
                      , "[kwk] - Out of bound index for kwk::between: end("
                        << b.end << ") strictly exceeds dim size: " << dim_size
                      );
      }
    }
    else // Dynamic dimension, runtime evaluation
    {
      auto dim_size = static_cast<ptrdiff_t>(get<N>(sh));

      auto bv = static_cast<ptrdiff_t>(b.begin);
      KIWAKU_ASSERT ( bv < dim_size
                    , "[kwk] - Out of bound index for kwk::between: begin("
                      << b.begin << ") exceeds or equals dim size: " << dim_size
                    );

      auto ev = static_cast<ptrdiff_t>(b.end);
      KIWAKU_ASSERT ( (ev >= 0) && (ev <= dim_size)
                    , "[kwk] - Out of bound index for kwk::between: begin("
                      << b.begin << ") exceeds or equals dim size: " << dim_size
                    );
    }

    if constexpr (requires{ B::value; } && requires{ E::value; })
    {
      return fixed<E::value - B::value>;
    }
    else
    {
      return b.end - b.begin;
    }
  }
}
