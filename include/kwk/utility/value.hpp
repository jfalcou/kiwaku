//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/value.hpp>
#include <concepts>

namespace kwk
{
  constexpr auto value(std::integral auto v)                  noexcept { return v; }
  constexpr auto value(kwk::concepts::static_constant auto v) noexcept { return v; }
}