//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_OPTIONS_SETTINGS_HELPERS_HPP_INCLUDED
#define KIWAKU_DETAIL_OPTIONS_SETTINGS_HELPERS_HPP_INCLUDED

#include <type_traits>
#include <cstddef>
#include <cassert>

namespace kwk::detail
{
  //==============================================================================================
  // Type notifying that we can't find a given key
  //==============================================================================================
  struct unknown_tag {};

  //==============================================================================================
  // Turn any type into a RegularType info carrier
  //==============================================================================================
  template<typename T> struct box {};

  //==============================================================================================
  // Aggregate lambdas and give them a operator(Key)-like interface
  //==============================================================================================
  template<typename... Ts> struct aggregator : Ts...
  {
    constexpr aggregator(Ts... t) noexcept : Ts(t)... {}

    using Ts::operator()...;

    template<typename K>
    constexpr decltype(auto) operator()(K const&) const
    {
      return (*this)(detail::box<K>{});
    }

    template<typename K, typename U>
    constexpr decltype(auto) operator()(K const&, U u) const
    {
      return (*this)(detail::box<K>{}, u);
    }

    template<typename K>
    constexpr auto operator()(box<K> const&) const noexcept { return unknown_tag{}; }

    template<typename K, typename U>
    constexpr auto operator()(box<K> const& k, U u) const noexcept
    {
      // If calling without default would return the key, use the default
      using found = decltype(this->operator()(box<K>{}));

      if constexpr( std::is_same_v<found,unknown_tag> ) return u;
      else                                              return this->operator()(k);
    }
  };

  template<typename... Ts> aggregator(Ts...) -> aggregator<Ts...>;

  //==============================================================================================
  // Build the type->value lambda capture
  //==============================================================================================
  template<typename Key, typename Value> constexpr auto link(Value v) noexcept
  {
    return [v](box<Key> const&) constexpr { return v; };
  }

  //==============================================================================================
  // Check if a type is a proper options pack
  //==============================================================================================
  template<typename T>      struct is_option                    : std::false_type {};
  template<typename... Ts>  struct is_option<aggregator<Ts...>> : std::true_type  {};
}

#endif
