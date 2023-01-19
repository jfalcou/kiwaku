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
  //! @brief Axis concept
  //!
  //! A **KIWAKU** Axis is a type is usable as a shape descriptor component
  //================================================================================================
  template<typename T>
  concept axis = requires(T)
  {
    typename T::axis_kind;
    { std::integral_constant<std::ptrdiff_t, T::static_index>{} };
  };

  //================================================================================================
  //! @brief Dynamically-sized axis concept
  //!
  //! A **KIWAKU** Dynamic Axis is an Axis type which size will be set at runtime.
  //================================================================================================
  template<typename T>
  concept dynamic_axis = axis<T> && !std::integral<typename T::content_type>;

  //================================================================================================
  //! @brief Statically-sized axis concept
  //!
  //! A **KIWAKU** Static Axis is an Axis type which size will be set at compile-time.
  //================================================================================================
  template<typename T>
  concept static_axis = axis<T> && std::integral<typename T::content_type>;
}
