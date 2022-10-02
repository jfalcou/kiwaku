//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <cstddef>

namespace kwk::detail
{
  inline constexpr auto round_to(std::ptrdiff_t value, std::ptrdiff_t alg) noexcept
  {
    return std::ptrdiff_t{(value + alg - 1) & ~(alg - 1)};
  }

  inline constexpr bool is_rounded(std::ptrdiff_t value, std::ptrdiff_t alg) noexcept
  {
    return  (value & (alg - 1)) == 0;
  }
}