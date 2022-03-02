//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <type_traits>
#include <utility>

namespace kwk::detail
{
  //================================================================================================
  // Find the value type of anything with a .data()
  //================================================================================================
  template<typename C>
  using value_type_of = std::remove_cvref_t<decltype(*std::declval<C>().data())>;

  //================================================================================================
  // for_each_args abstraction
  //================================================================================================
  template<typename Callable, typename... Args>
  constexpr void for_each_args(Callable c, Args&&... args) noexcept
  {
    (c(std::forward<Args>(args)),...);
  }

  //================================================================================================
  // constexpr_for abstraction
  //================================================================================================
  template<std::size_t Begin, std::size_t End, typename Callable>
  constexpr auto constexpr_for(Callable c) noexcept
  {
    return  []<typename Func,std::ptrdiff_t... Idx>
            (std::integer_sequence<std::ptrdiff_t,Idx...> const&, Func f)
            {
              ((f( std::integral_constant<std::ptrdiff_t,Begin+Idx>{})),...);
              return f;
            }( std::make_integer_sequence<std::ptrdiff_t, End-Begin>(), c );
  }

  template<std::size_t Count, typename Callable>
  constexpr auto constexpr_for(Callable c) noexcept
  {
    return constexpr_for<0,Count>(c);
  }

  //================================================================================================
  // Augmented static integer list for mapping known unit values
  //================================================================================================
  template<std::size_t... I> struct index_list
  {
    // How many indexes ?
    static constexpr std::size_t size = sizeof...(I);

    // is N in the set of known unit indexes ?
    static constexpr bool contains(std::size_t N) noexcept
    {
      if constexpr(sizeof...(I) > 0)
      {
        bool found[] = { (I==N)... };
        for(auto f : found) if(f) return true;
        return false;
      }
      else return false;
    }

    // Aggregation of an index into an existing index_list
    template<std::size_t J> using append = index_list<I...,J>;

    // Find the actual dynamic index of a non-unit index
    template<std::size_t Size> static constexpr std::size_t locate(std::size_t N) noexcept
    {
      // Build a bitmap of where the known values are
      std::size_t idx[Size] = {};
      ((idx[I] = 1), ...);

      // Count how far you need to go to find an unknown
      std::size_t pos = 0;
      for(std::size_t i=0;i<N;++i) pos += (1-idx[i]);

      return pos;
    }
  };

  //================================================================================================
  // Build a list of index in a type list where a target type is found
  //================================================================================================
  template<typename Target, std::size_t Index, typename Current, typename... Vs>
  struct type_map_impl;

  template<typename Target, std::size_t Index, typename Current>
  struct type_map_impl<Target, Index, Current, Target>
  {
    using type = typename Current::template append<Index>;
  };

  template<typename Target, std::size_t Index, typename Current, typename Head>
  struct type_map_impl<Target, Index, Current, Head>
  {
    using type = Current;
  };

  template<typename Target, std::size_t Index, typename Current, typename Head, typename... Tail>
  struct  type_map_impl<Target, Index  , Current, Head, Tail...>
        : type_map_impl<Target, Index+1,Current , Tail...>
  {
  };

  template<typename Target, std::size_t Index, typename Current, typename... Tail>
  struct  type_map_impl<Target, Index, Current, Target, Tail...>
        : type_map_impl<Target, Index+1,typename Current::template append<Index>,Tail...>
  {
  };

  template<typename Target, typename... Vs>
  struct type_map : type_map_impl<Target, 0, index_list<>, Vs...>
  {};
}
