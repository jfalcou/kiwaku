//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_CT_HELPERS_HPP_INCLUDED
#define KIWAKU_DETAIL_CT_HELPERS_HPP_INCLUDED

#include <type_traits>

namespace kwk::detail
{
  //================================================================================================
  // Find the value type of anything with a .data()
  //================================================================================================
  template<typename C>
  using value_type_of = std::remove_cvref_t<decltype(*std::declval<C>().data())>;

  //================================================================================================
  // Select type for inheritance purpose
  //================================================================================================
  template<bool Cond, typename Yes, typename No>
  using inherits_if = std::conditional_t<Cond, Yes, No>;

  //================================================================================================
  // Empty struct for EBO cases
  //================================================================================================
  struct empty {};

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
}

#endif
