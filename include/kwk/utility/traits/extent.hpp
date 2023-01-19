//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/axis.hpp>
#include <kwk/detail/kumi.hpp>

namespace kwk
{
  template<int N, auto Desc>
  inline constexpr bool is_dynamic_extent_v = concepts::dynamic_axis<kumi::element_t<N,decltype(Desc)>>;

  template<int N, auto Desc>
  inline constexpr bool is_static_extent_v = !is_dynamic_extent_v<N,Desc>;

  //================================================================================================
  //! @ingroup traits
  //! @brief Checks if the Nth extent of a shape descriptor is only known at runtime
  //!
  //! @tparam N Index of the extent to check
  //! @tparam Desc Extent descriptor to check
  //!
  //! **Helper value**
  //!
  //! @code{.cpp}
  //! template<int N, auto Desc>
  //! inline constexpr bool is_dynamic_extent_v = is_dynamic_extent<T>::value;
  //! @endcode
  //!
  //================================================================================================
  template<int N, auto Desc>
  struct is_dynamic_extent : std::bool_constant<is_dynamic_extent_v<N,Desc>>
  {};

  //================================================================================================
  //! @ingroup traits
  //! @brief Checks if the Nth extent of a shape descriptor is known at compile-time
  //!
  //! @tparam N Index of the extent to check
  //! @tparam Desc Extent descriptor to check
  //!
  //! **Helper value**
  //!
  //! @code{.cpp}
  //! template<int N, auto Desc>
  //! inline constexpr bool is_static_extent_v = is_static_extent<T>::value;
  //! @endcode
  //!
  //================================================================================================
  template<int N, auto Desc>
  struct is_static_extent : std::bool_constant<is_static_extent_v<N,Desc>>
  {};
}
