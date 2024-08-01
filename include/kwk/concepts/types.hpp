//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <limits>

namespace kwk::concepts
{
  //================================================================================================
  //! @brief Size Type concept
  //!
  //! A **KIWAKU** Size Type is a type usable as a safe size value.
  //================================================================================================
  template<typename T>
  concept size_type = requires(T const&)
  {
    typename T::value_type;
    typename T::kwk_size_type;
    { T::max() };
  };

  //================================================================================================
  //! @brief Bounded Size Type concept
  //!
  //! A **KIWAKU** BoundedSize Type is a safe size value with a non-trivial upper bound.
  //================================================================================================
  template<typename T>
  concept bounded_size_type = size_type<T> && (T::max() != std::numeric_limits<typename T::value_type>::max());
}
