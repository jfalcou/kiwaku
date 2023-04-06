//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/stdfix.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/utility/joker.hpp>
#include <type_traits>

namespace kwk::concepts
{
  /// Concept for types acting as a joker value
  template<typename T> concept joker = is_joker_v<T>;

  //====================================================================================================================
  //! @brief Shape and stride valid values
  //!
  //! kwk::shape and kwk::stride can only be constructed from types satisfying the extent concepts, i.e kwk::_, an axis
  //! based option or any type convertible to integer.
  //!
  //====================================================================================================================
  template<typename T>
  concept extent = std::convertible_to<T,std::size_t> || joker<T> || rbr::concepts::option<T>;

  /// Concept for shape and stride extent descriptor
  template<typename T>
  concept descriptor = requires(T)
  {
    typename std::remove_cvref_t<T>::contents_type;
  };
}
