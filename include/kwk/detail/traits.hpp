//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace kwk::__
{
  //================================================================================================
  // Check if a given list of types is a set
  // To do so, we build a type that linearly inherits from box<N,T>... and we check that
  // there is no ambiguity when trying to convert a bunch of Ts... into a block of inheritance
  //================================================================================================
  template<typename T> struct box {};
  template <std::size_t, typename T> struct unique { operator T(); };

  template <typename Ints, typename... Ts>
  struct all_unique_impl;

  template <>
  struct all_unique_impl<std::index_sequence<>> { using type = std::true_type; };

  template <std::size_t... Ints, typename... Ts>
  struct all_unique_impl<std::index_sequence<Ints...>, Ts...>
  {
    static inline std::true_type detect(...);
    struct all_uniques : unique<Ints, Ts>... {};

    template <typename... Us>
    static auto check(Us...) -> decltype(detect(static_cast<Us>(all_uniques())...));
    static std::false_type check(...);

    using type = decltype(check(Ts{}...));
  };

  template<typename... Ts>
  concept all_unique = all_unique_impl<std::index_sequence_for<Ts...>, box<Ts>...>::type::value;

  //================================================================================================
  // Check two axis are of the same kind
  //================================================================================================
  template<typename T, typename U>
  inline constexpr bool is_along_v  = std::same_as< typename T::axis_kind
                                                  , typename U::axis_kind
                                                  >;
}
