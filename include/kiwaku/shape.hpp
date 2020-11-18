//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/misc/shape.hpp>
#include <kiwaku/detail/ct_helpers.hpp>
#include <type_traits>
#include <cstddef>
#include <array>

namespace kwk
{
  namespace detail
  {

    //==============================================================================================
    // Discrimating types for shaper building
    //==============================================================================================
    struct dynamic_size { dynamic_size(auto) {} };
    struct static_size  {};

    //==============================================================================================
    // The type of a shape remembering the sequence of operations it was made from
    //==============================================================================================
    template <typename... Ops> struct shaper
    {
      using type = shaper<Ops...>;

      static constexpr auto size_map()    { return typename type_map<static_size, Ops...>::type{}; }
      static constexpr std::size_t size() { return sizeof...(Ops); }
      static constexpr bool is_fully_static = size_map().size == sizeof...(Ops);

      constexpr shaper()  = default;

      constexpr std::ptrdiff_t at(std::size_t i) const  { return data_[i]; }

      template <typename... X> constexpr auto append(X... x) const
      {
        return [&]<std::size_t... I>(std::index_sequence<I...> const&)
        {
          using that_t = std::array<std::ptrdiff_t, sizeof...(Ops) + sizeof...(X)>;
          return that_t{{std::get<I>(data_)..., x...}};
        }(std::index_sequence_for<Ops...>());
      }

      template <typename... Args>
      constexpr shaper(shaper<Args...> other, std::ptrdiff_t i)  : data_(other.append(i))
      {}

      constexpr shaper<Ops...,dynamic_size> operator()()                 const { return {*this, -1}; }
      constexpr shaper<Ops...,static_size>  operator[](std::ptrdiff_t i) const { return {*this,  i}; }

      std::array<std::ptrdiff_t, sizeof...(Ops)> data_;
    };

    template<> struct shaper<>
    {
      using type = shaper<>;

      static constexpr auto         size_map()  { return index_list{}; }
      static constexpr std::size_t  size()      { return 0; }
      static constexpr bool is_fully_static = true;

      constexpr shaper()  = default;

      template <typename... X> constexpr auto append(X... x) const
      {
        using that_t = std::array<std::ptrdiff_t, sizeof...(X)>;
        return that_t{{x...}};
      }

      constexpr shaper<dynamic_size> operator()()                 const { return {*this,-1}; }
      constexpr shaper<static_size>  operator[](std::ptrdiff_t i) const { return {*this, i}; }
    };
  }

  //================================================================================================
  // NTTP options
  //================================================================================================
  inline constexpr detail::shaper extent = {};

  // Dynamic pre-rendered dimension shaper
  inline constexpr auto _0D = extent;
  inline constexpr auto _1D = extent();
  inline constexpr auto _2D = extent()();
  inline constexpr auto _3D = extent()()();
  inline constexpr auto _4D = extent()()()();

  template<std::size_t N>
  inline constexpr auto _nD = []<std::size_t... I>(std::index_sequence<I...> const&)
                              {
                                return detail::shaper<decltype(detail::dynamic_size(I))...>{};
                              }(std::make_index_sequence<N>{});

  //================================================================================================
  // Deduction guides
  //================================================================================================
  template<typename... T> shape(T... s) -> shape< _nD<sizeof...(T)> >;

  //================================================================================================
  // Imperative construction
  //================================================================================================
  template<typename... I> auto of_shape(I... i)
  {
    return shape{i...};
  }
}

//================================================================================================
// Register a RBR keyword
//================================================================================================
namespace kwk
{
  namespace detail { struct shape_tag; }
  namespace option { inline constexpr auto shape  = ::rbr::keyword<kwk::detail::shape_tag>; }
}

namespace rbr
{
  template <typename... Ops>
  struct tag<kwk::detail::shaper<Ops...>> : tag<kwk::detail::shape_tag> {};
}
