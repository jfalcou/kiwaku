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
  template<typename Pointer> struct restricted
  {
    constexpr auto get() noexcept
    {
      that;
    }

    Pointer that;
  };
}
