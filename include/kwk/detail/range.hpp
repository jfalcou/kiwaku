//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <type_traits>
#include <cstddef>
#include <array>

namespace kwk::detail
{
  //================================================================================================
  // Find the static_size of static array like types
  //================================================================================================
  template<typename> struct static_size : std::false_type {};

  template<typename T, std::size_t N> struct static_size<std::array<T, N>> : std::true_type
  {
    static constexpr std::size_t size_value = N;
  };

  template<typename T, std::size_t N> struct static_size<T[N]> : std::true_type
  {
    static constexpr std::size_t size_value = N;
  };

  template<typename T>
  inline constexpr std::size_t static_size_v = static_size<std::remove_cvref_t<T>>::size_value;

  //================================================================================================
  // Find the value type of anything with a .data()
  //================================================================================================
  template<typename C>
  using value_type_of = std::remove_reference_t<decltype(*std::begin(std::declval<C&>()))>;
}
