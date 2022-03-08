//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <concepts>

namespace kwk::concepts
{
  template<typename Range>
  concept contiguous_range = requires(Range&& r)
  {
    { r.begin() };
    { r.end()   };
    { r.data()  };
  };
}
