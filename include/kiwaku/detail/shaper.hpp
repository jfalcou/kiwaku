//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kiwaku/detail/ct_helpers.hpp>
#include <type_traits>
#include <cstddef>
#include <array>

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

    template<typename SizeType2,typename... Ops2>
    constexpr bool is_compatible(shaper<SizeType2,Ops2...> other) const noexcept
    {
      if constexpr((std::same_as<Ops,Ops2> && ...))
      {
        for(std::size_t i=0;i<data_.size();++i)
        {
          if(data_[i] != other.data_[i]) return false;
        }

        return true;
      }
      else  return false;
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
  //!  #include<kiwaku/container/shape.hpp>
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //! <hr/>
  //!
  //! kwk::extent is used to define the layout of a kwk::shape in terms of rank and potential
  //! compile-time sizes.
  //!
  //! kwk::extent supports two member functions:
  //!   - `operator()()`        that specifies that a given dimension is known at runtime
  //!   - `operator[](auto N)`  that specifies that a given dimension has a compile-time value of N
  //!
  //! Both of these operators can be chained as needed to describe a given shape.
  //!
  //! Usage of kwk::extent is advised when one wants to properly type a given shape.
  //! Other usage of shape descriptors are usually abstracted away via kwk::of_size.
  //!
  //! @groupheader{Associated variables}
  //!
  //! Some pre-made descriptor are provided for common cases:
  //!
  //!  - `kwk::_1D`     : Dynamic rank 1 kwk::shape descriptor
  //!  - `kwk::_2D`     : Dynamic rank 2 kwk::shape descriptor
  //!  - `kwk::_3D`     : Dynamic rank 3 kwk::shape descriptor
  //!  - `kwk::_4D`     : Dynamic rank 4 kwk::shape descriptor
  //!  - `kwk::_nD<N>`  : Dynamic rank N kwk::shape descriptor
  //!
  //! @groupheader{Examples}
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
  //! #include<kiwaku/container/shape.hpp>
  //!
  //! // Shape descriptors with runtime sizes
  //! kwk::shape< kwk::extent()   > d1(6, 8); // Rank 1 shape with runtime size 6 x 8
  //! kwk::shape< kwk::extent()() > d2;       // Rank 2 shape with runtime size 0 x 1
  //!
  //! // Shape descriptors with compile-time sizes
  //! kwk::shape< kwk::extent[4]    > s1;   // Rank 1 shape with compile-time size 4
  //! kwk::shape< kwk::extent[2][2] > s2;   // Rank 2 shape with compile-time sizes 2 x 2
  //!
  //! // Shape descriptor mixing runtime and compile-time sizes
  //! kwk::shape< kwk::extent[3]()[3]() > h;  // Rank 4 shape with size 3 x 1 x 3 x 1
  //!
  //! // Pre-made descriptors
  //! kwk::shape< kwk::_2D    > p1; // Rank 2 shape with default runtime size
  //! kwk::shape< kwk::_nD<7> > p2; // Rank 7 shape with default runtime size
  //! kwk::shape< kwk::_3D[3] > h;  // Rank 4 shape with size 0 x 1 x 1 x 3
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //!
  //================================================================================================
  inline constexpr detail::shaper<std::ptrdiff_t> extent  = {};

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
