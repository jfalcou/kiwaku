//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/sequence/combo.hpp>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace kwk
{
  //================================================================================================
  //! @ingroup settings
  //! @var    extent
  //! @brief  Constexpr generator for extent lists
  //!
  //! <hr/>
  //! **Required header**:
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!  #include<kwk/settings/extent.hpp>
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //! <hr/>
  //!
  //! kwk::extent is used to define the layout of a kwk::shape or kwk::stride in terms of order and
  //! potential compile-time sizes.
  //!
  //! kwk::extent supports two member functions:
  //!   - `operator()()`        that specifies that a given dimension is known at runtime
  //!   - `operator[](auto N)`  that specifies that a given dimension has a compile-time value of N
  //!
  //! Both of these operators can be chained as needed to describe a given shape.
  //!
  //! The actual integer type storing the size or stride of each dimensions can be specified by
  //! using kwk::extent_of.
  //!
  //! Usage of @ref kwk::extent is advised when one wants to properly type a given shape or stride.
  //! Other usage of @ref kwk::shape descriptors are usually abstracted away via @ref kwk::of_size
  //! or @ref kwk::stride.
  //!
  //! @groupheader{Associated variables}
  //!
  //! Some pre-made descriptor are provided for common cases:
  //!
  //!  - `kwk::_1D`     : Dynamic order 1 kwk::shape descriptor
  //!  - `kwk::_2D`     : Dynamic order 2 kwk::shape descriptor
  //!  - `kwk::_3D`     : Dynamic order 3 kwk::shape descriptor
  //!  - `kwk::_4D`     : Dynamic order 4 kwk::shape descriptor
  //!  - `kwk::_nD<N>`  : Dynamic order N kwk::shape descriptor
  //!
  //! @groupheader{Examples}
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //! #include<kwk/container/shape.hpp>
  //!
  //! // Shape descriptors with runtime sizes
  //! kwk::shape< kwk::extent()() > d1(6, 8); // Order 1 shape with runtime size 6 x 8
  //! kwk::shape< kwk::extent()() > d2;       // Order 2 shape with runtime size 0 x 1
  //!
  //! kwk::shape< kwk::extent_of<short>()() > s1(6, 8); // Order 1 shape with runtime size 6 x 8 stored as short
  //!
  //! // Shape descriptors with compile-time sizes
  //! kwk::shape< kwk::extent[4]    > s1;   // Order 1 shape with compile-time size 4
  //! kwk::shape< kwk::extent[2][2] > s2;   // Order 2 shape with compile-time sizes 2 x 2
  //!
  //! // Shape descriptor mixing runtime and compile-time sizes
  //! kwk::shape< kwk::extent[3]()[3]() > h;  // Order 4 shape with size 3 x 1 x 3 x 1
  //!
  //! // Pre-made descriptors
  //! kwk::shape< kwk::_2D    > p1; // Order 2 shape with default runtime size
  //! kwk::shape< kwk::_nD<7> > p2; // Order 7 shape with default runtime size
  //! kwk::shape< kwk::_3D[3] > h;  // Order 4 shape with size 0 x 1 x 1 x 3
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //!
  //================================================================================================
  inline constexpr __::combo<std::int32_t> extent  = {};

  template<typename T>
  inline constexpr __::combo<T> extent_of  = {};

  inline constexpr auto _1D = extent();
  inline constexpr auto _2D = _1D();
  inline constexpr auto _3D = _2D();
  inline constexpr auto _4D = _3D();

  template<std::size_t N>
  inline constexpr auto _nD = []<std::size_t... I>(std::index_sequence<I...>)
  {
    return make_combo<std::int32_t>(along<N-I-1>...);
  }(std::make_index_sequence<N>{});
}
