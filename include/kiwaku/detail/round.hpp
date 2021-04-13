//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
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
