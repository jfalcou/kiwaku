//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

namespace kwk
{
  template<typename Pointer> struct direct
  {
    constexpr auto get() noexcept { return that; }
    Pointer that;
  };
}
