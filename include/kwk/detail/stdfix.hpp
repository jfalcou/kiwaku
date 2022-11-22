//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#if defined( __ANDROID__ )
#include <bit>
#include <concepts>

namespace std
{
  // Android specific bit_width
  constexpr auto bit_width( auto  value  ) noexcept
  {
    return ( value == 0 ) ? 0 : std::__bit_log2( value ) + 1;
  }

  // Android specific missing concepts
  template<typename From, typename To>
  concept convertible_to  =   is_convertible_v<From, To>
                          &&  requires { static_cast<To>(declval<From>()); };

  template<typename Dp, typename Bp>
  concept derived_from  =   is_base_of_v<Bp, Dp>
                        &&  is_convertible_v<const volatile Dp*, const volatile Bp*>;

  template<typename Tp>
  concept destructible = is_nothrow_destructible_v<Tp>;

  template<typename Tp, typename... Args>
  concept constructible_from = destructible<Tp> && is_constructible_v<Tp, Args...>;

  template<typename Tp>
  concept integral = is_integral_v<Tp>;
}
#endif
