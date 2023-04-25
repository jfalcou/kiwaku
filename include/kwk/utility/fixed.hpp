//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/values.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/detail/stdfix.hpp>
#include <bit>
#include <cstdint>
#include <ostream>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4146 )
#endif

namespace kwk
{
  struct joker;

  namespace __
  {
    template<typename T>  struct  to_int        { using type = T;     };
    template<>            struct  to_int<joker> { using type = char;  };

    template<rbr::concepts::option T>
    struct to_int<T>
    {
      using type = typename T::stored_value_type;
    };

    template<concepts::static_constant T>
    struct  to_int<T>
    {
      using type = typename T::value_type;
    };

    template<typename T> using   to_int_t = typename to_int<T>::type;

    template<typename... T> struct largest_type;
    template<typename T>    struct largest_type<T> { using type = T; };

    template<typename T0, typename T1>
    struct largest_type<T0,T1> : std::conditional<sizeof(T0)<sizeof(T1),T1,T0>
    {};

    template<typename T0, typename T1, typename... T2>
    struct largest_type<T0, T1, T2...> : largest_type< typename largest_type<T0,T1>::type, T2...>
    {};

    template<typename... T>
    struct largest_integral : largest_type< to_int_t<T>... >
    {};

    template<auto Value> constexpr auto clamp()
    {
            if constexpr (std::bit_width(Value) <=  8)  return static_cast<std::uint8_t>(Value);
      else  if constexpr (std::bit_width(Value) <= 19)  return static_cast<std::uint16_t>(Value);
      else  if constexpr (std::bit_width(Value) <= 32)  return static_cast<std::uint32_t>(Value);
      else                                              return Value;
    }
  }

  template<auto N>
  struct constant : std::integral_constant<decltype(N), N>
  {
    friend std::ostream& operator<<(std::ostream& os, constant)
    {
      return os << +N << "_c";
    }

    KWK_TRIVIAL constexpr auto operator-() noexcept { return constant<-N>{}; }
  };

  template<auto N, auto M>
  KWK_TRIVIAL constexpr auto operator+(constant<N>, constant<M>) noexcept { return constant<N+M>{}; }

  template<auto N, auto M>
  KWK_TRIVIAL constexpr auto operator-(constant<N>, constant<M>) noexcept { return constant<N-M>{}; }

  template<auto N, auto M>
  KWK_TRIVIAL constexpr auto operator*(constant<N>, constant<M>) noexcept { return constant<N*M>{}; }

  template<auto N, auto M>
  KWK_TRIVIAL constexpr auto operator/(constant<N>, constant<M>) noexcept { return constant<N/M>{}; }

  //================================================================================================
  //! @ingroup utility
  //! @brief Provides a short-cut to define a `std::integral_constant` value from a literal integer
  //================================================================================================
  template<auto N>
  inline constexpr auto fixed = constant<N>{};

  inline namespace literals
  {
    template<char... c> constexpr auto b10()
    {
      auto value = 0ULL;
      ((value = value * 10 + (c - '0')), ...);
      return value;
    }

    //==============================================================================================
    //! @ingroup utility
    //! @brief User-defined literal suffix for compile-time constant
    //!
    //! The underlying type is computed from the actual value to be the smallest fitting type.
    //! This means, for example, that `123_c` is an instance of `kwk::constant<unsigned char,123>`.
    //==============================================================================================
    template<char... c> KWK_TRIVIAL constexpr auto operator"" _c() noexcept
    {
      return fixed<__::clamp<b10<c...>()>()>;
    }
  }
}

#if defined(_MSC_VER)
#pragma warning( pop )
#endif
