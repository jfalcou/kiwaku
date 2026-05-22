//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/axis.hpp>
#include <kwk/detail/stdfix.hpp>
#include <raberu/raberu.hpp>
#include <kwk/utility/joker.hpp>
#include <type_traits>

namespace kwk::concepts
{
  /// Concept for types acting as a joker value
  template<typename T> concept joker = is_joker_v<T>;

  //====================================================================================================================
  //! @brief Shape and stride named values
  //!
  //! Named extents are options which keyword models kwk::concepts::axis
  //====================================================================================================================
  template<typename T>
  concept named_extent = rbr::concepts::option<T> && axis<typename T::keyword_type>;

  //====================================================================================================================
  //! @brief Shape and stride numerical values
  //!
  //! Numerical extents are joker, axis or values convertible to `std::size_t`
  //====================================================================================================================
  template<typename T>
  concept numeric_extent = std::convertible_to<T,std::size_t> || axis<T> || joker<T>;

  //====================================================================================================================
  //! @brief Shape and stride valid values
  //!
  //! kwk::shape and kwk::stride can only be constructed from types satisfying the extent concepts, i.e kwk::_, an axis
  //! based option or any type convertible to integer.
  //====================================================================================================================
  template<typename T>
  concept extent = numeric_extent<T> || named_extent<T>;
}
