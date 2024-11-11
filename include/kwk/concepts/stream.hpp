//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <concepts>

namespace kwk::concepts
{
  //================================================================================================
  //! @brief Output stream concept
  //================================================================================================
  template<typename T>
  concept output_stream = requires(T& os)
  {
    { os.fill() };
    { os << 0.  };
  };
}
