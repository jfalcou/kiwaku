//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/detail/ct_helpers.hpp>
#include <type_traits>
#include <cstddef>
#include <array>
#include <utility>

namespace kwk::detail
{
  // Discriminating types for shaper building
  struct dyn_ { dyn_(auto) {} };
  struct fix_ {};

  // The type of a shape remembering the sequence of operations it was made from
  template <typename SizeType, typename... Ops> struct shaper
  {
    using size_type = SizeType;
    using type      = shaper<Ops...>;

    static constexpr auto size_map()  { return typename type_map<fix_, Ops...>::type{}; }
    static constexpr std::ptrdiff_t size()  { return sizeof...(Ops); }
    static constexpr bool is_fully_static = size_map().size == sizeof...(Ops);

    constexpr shaper() : data_{} {}

    constexpr auto at(std::size_t i) const  { return data_[i]; }

    template <typename... X> constexpr auto append(X... x) const
    {
      return [&]<std::size_t... I>(std::index_sequence<I...> const&)
      {
        using that_t = std::array<SizeType, sizeof...(Ops) + sizeof...(X)>;
        return that_t{{std::get<I>(data_)..., x...}};
      }(std::index_sequence_for<Ops...>());
    }

    template<typename ST2,typename... O2>
    constexpr bool is_compatible(shaper<ST2,O2...> o) const noexcept
    {
      if constexpr(sizeof...(O2) != sizeof...(Ops))  return false;
      else
      {
        return [&]<std::size_t... I>( std::index_sequence<I...>)
        {
          return  ((( std::same_as<Ops,dyn_>
                  ||  (std::same_as<Ops,fix_> && std::same_as<O2,fix_> && data_[I] == o.data_[I])
                  )) && ...);
        }( std::make_index_sequence<sizeof...(Ops)>{});
      }
    }

    template <typename... Args>
    constexpr shaper(shaper<Args...> other, SizeType i)  : data_(other.append(i))
    {}

    constexpr shaper<SizeType,Ops...,dyn_>  operator()()           const { return {*this, 0}; }
    constexpr shaper<SizeType,Ops...,fix_>  operator[](SizeType i) const { return {*this, i}; }

    std::array<SizeType, sizeof...(Ops)> data_;
  };

  // Special case for 0D extent
  template<typename SizeType> struct shaper<SizeType>
  {
    using size_type = SizeType;
    using type      = shaper<SizeType>;

    static constexpr auto             size_map()  { return index_list{}; }
    static constexpr std::ptrdiff_t   size()      { return 0; }
    static constexpr bool is_fully_static = true;

    constexpr shaper() {}

    template <typename... X> constexpr auto append(X... x) const
    {
      using that_t = std::array<SizeType, sizeof...(X)>;
      return that_t{{x...}};
    }

    template<typename SizeType2>
    constexpr bool is_compatible(shaper<SizeType2>) const noexcept { return true; }

    constexpr shaper<SizeType,dyn_> operator()()           const { return {*this, 0}; }
    constexpr shaper<SizeType,fix_> operator[](SizeType i) const { return {*this, i}; }
  };

  template<int U> struct to_dyn_ { using type = detail::dyn_; };
}

namespace kwk
{
  //================================================================================================
  //! @var    extent
  //! @brief  kwk::shape descriptor helper
  //!
  //! <hr/>
  //! **Required header**:
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //!  #include<kwk/container/shape.hpp>
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //! <hr/>
  //!
  //! kwk::extent is used to define the layout of a kwk::shape in terms of order and potential
  //! compile-time sizes.
  //!
  //! kwk::extent supports two member functions:
  //!   - `operator()()`        that specifies that a given dimension is known at runtime
  //!   - `operator[](auto N)`  that specifies that a given dimension has a compile-time value of N
  //!
  //! Both of these operators can be chained as needed to describe a given shape.
  //!
  //! The actual integer type storing the size of each dimensions can be specified by using
  //! kwk::extent_of.
  //!
  //! Usage of kwk::extent is advised when one wants to properly type a given shape.
  //! Other usage of shape descriptors are usually abstracted away via kwk::of_size.
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
  inline constexpr detail::shaper<std::ptrdiff_t> extent  = {};

  template<typename T>
  inline constexpr detail::shaper<T> extent_of  = {};

  inline constexpr auto _1D     = extent();
  inline constexpr auto _2D     = _1D();
  inline constexpr auto _3D     = _2D();
  inline constexpr auto _4D     = _3D();

  template<std::size_t N>
  inline constexpr auto _nD = []<std::size_t... I>(std::index_sequence<I...> const&) constexpr
                              {
                                return detail::shaper < std::ptrdiff_t
                                                      , typename detail::to_dyn_<I>::type...
                                                      >{};
                              }(std::make_index_sequence<N>{});
}
