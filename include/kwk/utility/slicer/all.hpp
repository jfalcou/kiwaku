//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/utility/joker.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/utility/traits/extent.hpp>

namespace kwk
{
  template<auto Desc> struct shape;

  template<auto Desc, std::size_t N>
  constexpr auto reshape(shape<Desc> const& sh, joker const&, kumi::index_t<N> const&) noexcept
  {
    if constexpr(is_dynamic_extent_v<N,Desc>) return get<N>(sh);
    else return fixed<get<N>(Desc)>;
  }
}
