//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>

namespace kwk::options
{
  // For view, we infer the block type from the source that must be present
  template<typename Tag, rbr::concepts::settings Settings>
  struct data
  {
    using type = decltype( block(Tag{}, std::declval<Settings>()) );
  };
}
