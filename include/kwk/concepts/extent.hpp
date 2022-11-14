//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/utility/joker.hpp>
#include <type_traits>

namespace kwk::concepts
{
  /// Concept for types acting as a joker value
  template<typename T> concept joker = is_joker_v<T>;

  /// Concept for types usable as shape and stride extent values
  template<typename T, typename S>
  concept extent = std::convertible_to<T,S> || joker<T>;

  /// Concept for shape and stride extent descriptor
  template<typename T>
  concept descriptor = requires(T)
  {
    typename std::remove_cvref_t<T>::contents_type;
  };
}
