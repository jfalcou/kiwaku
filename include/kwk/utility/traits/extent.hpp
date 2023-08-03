//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/axis.hpp>
#include <kwk/concepts/values.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/detail/kumi.hpp>

#include <concepts>

namespace kwk
{
  template<std::int32_t N, auto Desc>
  inline constexpr bool is_dynamic_extent_v = kumi::element_t<N,decltype(Desc)>::is_dynamic;

  template<std::int32_t N, auto Desc>
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
  //! template<std::int32_t N, auto Desc>
  //! inline constexpr bool is_dynamic_extent_v = is_dynamic_extent<T>::value;
  //! @endcode
  //!
  //================================================================================================
  template<std::int32_t N, auto Desc>
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
  //! template<std::int32_t N, auto Desc>
  //! inline constexpr bool is_static_extent_v = is_static_extent<T>::value;
  //! @endcode
  //!
  //================================================================================================
  template<std::int32_t N, auto Desc>
  struct is_static_extent : std::bool_constant<is_static_extent_v<N,Desc>>
  {};

  // thin as_dimension
  KWK_CONST constexpr auto extent(std::integral             auto const dim) noexcept { return dim; }
  KWK_CONST constexpr auto extent(concepts::axis            auto const dim) noexcept { return dim.value; }
  KWK_CONST constexpr auto extent(concepts::static_constant auto const dim) noexcept { return dim; }
}
