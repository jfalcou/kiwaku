//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <bit>
#include <concepts>

namespace std
{
#if ( __cpp_lib_int_pow2 < 202002L ) && ( STDFIX_BIT_bit_width < 202002L )
  constexpr auto bit_width( auto value ) noexcept
  {
    return ( value == 0 ) ? 0 : std::__bit_log2( value ) + 1;
  }
#endif

#if __cpp_lib_concepts < 202002L
#if STDFIX_CONCEPT_convertible_to < 202002L
  template<typename From, typename To>
  concept convertible_to  =   is_convertible_v<From, To>
                          &&  requires { static_cast<To>(declval<From>()); };
#endif

#if STDFIX_CONCEPT_derived_from < 202002L
  template<typename Dp, typename Bp>
  concept derived_from  =   is_base_of_v<Bp, Dp>
                        &&  is_convertible_v<const volatile Dp*, const volatile Bp*>;
#endif

#if STDFIX_CONCEPT_destructible < 202002L
  template<typename Tp>
  concept destructible = is_nothrow_destructible_v<Tp>;
#endif

#if STDFIX_CONCEPT_constructible_from < 202002L
  template<typename Tp, typename... Args>
  concept constructible_from = destructible<Tp> && is_constructible_v<Tp, Args...>;
#endif

#if STDFIX_CONCEPT_integral < 202002L
  template<typename Tp>
  concept integral = is_integral_v<Tp>;
#endif
#endif // __cpp_lib_concepts < 202002L
}
