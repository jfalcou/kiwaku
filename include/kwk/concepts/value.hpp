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
  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent a compile time static constant.

    A type `T` models `kwk::concepts::static_constant` if it holds a `value_type` alias and a static `value` member.

    ## Example types:
    + std::integral_constant<std::size_t,1>;
    + kwk::fixed<1>
  **/
  //====================================================================================================================
  template<typename T>
  concept static_constant = requires(T) {
    { std::remove_cvref_t<T>::value };
    typename std::remove_cvref_t<T>::value_type;
  };

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent a compile time static arithmetic constant.

    A type `T` models `kwk::concepts::arithmetic_constant` if it models `kwk::static_constant` which internal member
    models either `std::integral` or `std::floating_point`.

    ## Example types:
    + std::integral_constant<std::size_t,1>;
    + std::integral<float, 3.f>;
    + kwk::fixed<1>;
  **/
  //====================================================================================================================
  template<typename T>
  concept arithmetic_constant = static_constant<T> && requires(T) {
    requires std::integral<typename std::remove_cvref_t<T>::value_type> ||
               std::floating_point<typename std::remove_cvref_t<T>::value_type>;
  };

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent an arithmetic value.

    A type `T` models `kwk::concepts::arithmetic_value` if it models : `kwk::arithmetic_constant` or `std::integral` or
    `std::floating_point`.

    ## Example types:
    + std::integral_constant<std::size_t,1>;
    + std::integral<float, 3.f>;
    + kwk::fixed<1>;
    + float
    + int
  **/
  //====================================================================================================================
  template<typename T>
  concept arithmetic_value =
    arithmetic_constant<T> || std::integral<std::remove_cvref_t<T>> || std::floating_point<std::remove_cvref_t<T>>;

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent an integral arithmetic value.

    A type `T` models `kwk::concepts::integral_arithmetic_value` if it models : `kwk::static_constant` with inner type
    modeling `std::integral` or is simply models `std::integral`.

    ## Example types:
    + std::integral_constant<std::size_t,1>;
    + kwk::fixed<1>;
    + int
  **/
  //====================================================================================================================
  template<typename T>
  concept integral_arithmetic_value =
    (static_constant<T> && std::integral<typename std::remove_cvref_t<T>::value_type>) ||
    std::integral<std::remove_cvref_t<T>>;

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent an extent.

    A type `T` models `kwk::concepts::extent` if it models : `kwk::static_constant` with inner type
    modeling `std::integral` or is simply models `std::integral`.

    ## Example types:
    + std::integral_constant<std::size_t,1>;
    + kwk::fixed<1>;
    + kwk::_
    + int
  **/
  //====================================================================================================================
  template<typename T>
  concept extent = kwk::is_wildcard_v<std::remove_cvref_t<T>> || integral_arithmetic_value<T>;

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent an extent.

    A type `T` models `kwk::concepts::dynamic_extent` if it models : `kwk::static_constant` with inner type
    modeling `std::integral` or is simply models `std::integral`.

    ## Example types:
    + kwk::_
    + int
  **/
  //====================================================================================================================
  template<typename T>
  concept dynamic_extent = kwk::is_wildcard_v<std::remove_cvref_t<T>> || integral_arithmetic_value<T>;

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type represent an extremum.

    A type `T` models `kwk::concepts::extremum` if it has a function that can return a value convertible to
  std::ptrdiff_t to be used as an index.

    ## Example types:
    + kwk::extremum

  **/
  //====================================================================================================================
  template<typename T>
  concept extremum = requires(T x, std::ptrdiff_t I) {
    { x(I) } -> std::convertible_to<std::ptrdiff_t>;
  };

  //====================================================================================================================
  /**
    @ingroup concepts
    @brief Concept specifying a type can be used as a slicer.

    A type `T` models `kwk::concepts::slicer` if there exist a free function called `to_slicer` taking that can be found
    via @ref{adl} on the input type.

    ## Example types:
    + kwk::extremum
    + kwk::slicer
    + kwk::fixed<1>
    + int
  **/
  //====================================================================================================================
  template<typename T>
  concept slicer = requires(T const& t) { to_slicer(t); };
}
