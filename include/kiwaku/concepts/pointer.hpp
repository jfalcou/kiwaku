//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>

namespace kwk::concepts
{
  /// Concept for types behaving as an actual pointer type.
  template<typename T>
  concept pointer = std::is_pointer_v<std::remove_cvref_t<T>>;
}
