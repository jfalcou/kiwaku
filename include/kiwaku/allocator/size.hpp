//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/allocator/bytes.hpp>
#include <kiwaku/allocator/elements.hpp>
#include <kiwaku/assert.hpp>

namespace kwk
{
  //================================================================================================
  // bytes <=> elements conversions
  //================================================================================================
  template<typename T> [[nodiscard]] constexpr bytes as_bytes(elements n) noexcept
  {
    return bytes{static_cast<std::ptrdiff_t>(sizeof(T) * n)};
  }

  template<typename T> [[nodiscard]] constexpr elements as_elements(bytes n) noexcept
  {
    KIWAKU_ASSERT ( to_integer(n) % sizeof(T) == 0
                  , "bytes size must be a multiple of sizeof(T)"
                  );

    return elements{static_cast<std::ptrdiff_t>(n / sizeof(T))};
  }

}
