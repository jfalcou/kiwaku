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
  //! @brief Category concept
  //!
  //! A **KIWAKU** Category is a type usable as a semantic setting.
  //================================================================================================
  template<typename Type>
  concept category = requires(Type const&)
  {
    typename Type::kwk_container_category;
  };
}
