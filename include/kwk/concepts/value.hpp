//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk::concepts
{
  template<typename T>
  concept static_constant = requires(T) {
    { T::value };
    typename T::value_type;
  };

  template<typename T>
  concept arithmetic_constant = requires(T) {
    { T::value };
    typename T::value_type;
    std::integral<typename T::value_type> || std::floating_point<typename T::value_type>;
  };

  template<typename T>
  concept integral_arithmetic_value = requires(T) {
    { T::value };
    typename T::value_type;
    std::integral<typename T::value_type>;
  } || std::integral<T>;

  template<typename T>
  concept arithmetic_value = arithmetic_constant<T> || std::integral<T> || std::floating_point<T>;

  template<typename T>
  concept extremum = requires(T x, std::ptrdiff_t I) {
    { x(I) } -> std::convertible_to<std::ptrdiff_t>;
  };

  template<typename T>
  concept slicer = requires(T const& t) { to_slicer(t); };
}
