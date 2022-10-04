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

namespace kwk
{
  template<auto Desc> struct shape;

  template<auto Desc, std::size_t N>
  constexpr auto reshape(shape<Desc> const&, std::integral auto, kumi::index_t<N> const&) noexcept
  {
    return fixed<1>;
  }
}
