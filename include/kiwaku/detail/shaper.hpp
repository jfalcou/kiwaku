//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
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

    template <typename... Args>
    constexpr shaper(shaper<Args...> other, SizeType i)  : data_(other.append(i))
    {}

    constexpr shaper<SizeType,Ops...,dyn_>  operator()()           const { return {*this, 0}; }
    constexpr shaper<SizeType,Ops...,fix_>  operator[](SizeType i) const { return {*this, i}; }

    std::array<SizeType, sizeof...(Ops)> data_;
  };

  // Special case for 0D estent
  template<typename SizeType> struct shaper<SizeType>
  {
    using size_type = SizeType;
    using type = shaper<SizeType>;

    static constexpr auto             size_map()  { return index_list{}; }
    static constexpr std::ptrdiff_t   size()      { return 0; }
    static constexpr bool is_fully_static = true;

    constexpr shaper() {}

    template <typename... X> constexpr auto append(X... x) const
    {
      using that_t = std::array<SizeType, sizeof...(X)>;
      return that_t{{x...}};
    }

    constexpr shaper<SizeType,dyn_> operator()()           const { return {*this, 0}; }
    constexpr shaper<SizeType,fix_> operator[](SizeType i) const { return {*this, i}; }
  };
}

namespace kwk
{
  // Dynamic pre-rendered dimension shaper
  inline constexpr auto shape_  = detail::shaper<std::ptrdiff_t>{};
  inline constexpr auto _0D     = shape_;
  inline constexpr auto _1D     = _0D();
  inline constexpr auto _2D     = _1D();
  inline constexpr auto _3D     = _2D();
  inline constexpr auto _4D     = _3D();

  template<int U> struct to_dyn_ { using type = detail::dyn_; };

  // Dynamic N-dimension shaper
  template<std::size_t N>
  inline constexpr auto _nD = []<std::size_t... I>(std::index_sequence<I...> const&) constexpr
                              {
                                return detail::shaper<std::ptrdiff_t, typename to_dyn_<I>::type...>{};
                              }(std::make_index_sequence<N>{});
}
