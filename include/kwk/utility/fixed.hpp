//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <type_traits>
#include <cstdint>

namespace kwk
{
  struct joker;

  namespace detail
  {
    template<typename T>      struct  to_int                              { using type = T; };
    template<>                struct  to_int<joker>                       { using type = char;  };
    template<typename T, T N> struct  to_int<std::integral_constant<T,N>> { using type = T;     };
    template<typename T>      using   to_int_t = typename to_int<T>::type;

    template<typename... T>   struct largest_type;
    template<typename T>      struct largest_type<T> { using type = T; };

    template<typename T0, typename T1>
    struct largest_type<T0,T1> : std::conditional<sizeof(T0)<sizeof(T1),T1,T0>
    {};

    template<typename T0, typename T1, typename... T2>
    struct largest_type<T0, T1, T2...> : largest_type< typename largest_type<T0,T1>::type, T2...>
    {};

    template<auto Value> constexpr auto clamp()
    {
      if constexpr(Value >= 0)
      {
              if constexpr (Value<0x100      )  return static_cast<std::uint8_t>(Value);
        else  if constexpr (Value<0x10000    )  return static_cast<std::uint16_t>(Value);
        else  if constexpr (Value<0x100000000)  return static_cast<std::uint32_t>(Value);
        else                                    return Value;
      }
      else
      {
              if constexpr (-Value<0x7F      )  return static_cast<std::int8_t>(Value);
        else  if constexpr (-Value<0x7FFF    )  return static_cast<std::int16_t>(Value);
        else  if constexpr (-Value<0x7FFFFFFF)  return static_cast<std::int32_t>(Value);
        else                                    return Value;
      }
    }
  }

  /**
    @ingroup utility
    @brief Provides a short-cut to define a `std::integral_constant` instance from a literal integer

    The underlying type is computed from the actual value to be the smallest fitting type.
    This means, for example, that kwk::fixed<123> is an instance of
	  `std::integral_constant<unsigned char,123>`. When negative values are used, signed integral
    types are selected, i.e kwk::fixed<-999> is an instance of
    `std::integral_constant<signed short,-999>`.
  **/
  template<auto N>
  inline constexpr auto fixed = std::integral_constant<decltype(N), N>{};

  namespace literals
  {
    template<char... c> constexpr auto b10()
    {
      auto value = 0LL;
      ((value = value * 10 + (c - '0')), ...);
      return value;
    }

    /**
      @ingroup utility
      @brief User-defined literal suffix for compile-time constant
    **/
    template<char... c> constexpr auto operator"" _c() noexcept
    {
      return fixed<detail::clamp<b10<c...>()>()>;
    }
  }
}
