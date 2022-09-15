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
  // Discriminating types for hybrid_sequence building
  struct dyn_ { dyn_(auto) {} };
  struct fix_ {};

  // A type remembering the sequence of operations it was made from
  template <typename SizeType, typename... Ops> struct hybrid_sequence
  {
    using size_type = SizeType;
    using type      = hybrid_sequence<Ops...>;

    static constexpr auto size_map()  { return typename type_map<fix_, Ops...>::type{}; }
    static constexpr std::ptrdiff_t size()  { return sizeof...(Ops); }
    static constexpr bool is_fully_static = size_map().size == sizeof...(Ops);

    constexpr hybrid_sequence() : data_{} {}

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
    constexpr bool is_compatible(hybrid_sequence<ST2,O2...> o) const noexcept
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
    constexpr hybrid_sequence(hybrid_sequence<Args...> other, SizeType i)  : data_(other.append(i))
    {}

    constexpr hybrid_sequence<SizeType,Ops...,dyn_>  operator()()           const { return {*this, 0}; }
    constexpr hybrid_sequence<SizeType,Ops...,fix_>  operator[](SizeType i) const { return {*this, i}; }

    std::array<SizeType, sizeof...(Ops)> data_;
  };

  // Special case for empty hybrid_sequence
  template<typename SizeType> struct hybrid_sequence<SizeType>
  {
    using size_type = SizeType;
    using type      = hybrid_sequence<SizeType>;

    static constexpr auto             size_map()  { return index_list{}; }
    static constexpr std::ptrdiff_t   size()      { return 0; }
    static constexpr bool is_fully_static = true;

    constexpr hybrid_sequence() {}

    template <typename... X> constexpr auto append(X... x) const
    {
      using that_t = std::array<SizeType, sizeof...(X)>;
      return that_t{{x...}};
    }

    template<typename SizeType2>
    constexpr bool is_compatible(hybrid_sequence<SizeType2>) const noexcept { return true; }

    constexpr hybrid_sequence<SizeType,dyn_> operator()()           const { return {*this, 0}; }
    constexpr hybrid_sequence<SizeType,fix_> operator[](SizeType i) const { return {*this, i}; }
  };

  template<int U> struct to_dyn_ { using type = detail::dyn_; };
}
