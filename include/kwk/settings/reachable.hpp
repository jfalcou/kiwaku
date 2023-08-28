//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>

namespace kwk
{
  namespace __ { struct unreachable_ {}; }

  //================================================================================================
  //! @ingroup  settings
  //! @brief    Data reachability option
  //================================================================================================
  inline constexpr auto unreachable = rbr::flag(__::unreachable_{});
}
