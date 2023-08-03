//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <cstdint>
#include <ostream>

#ifndef KWK_DEFAULT_AXIS_BITNESS
#define KWK_DEFAULT_AXIS_BITNESS -32
#endif

namespace kwk
{
  namespace detail
  {
      template <int bits> struct int_type;
      template <> struct int_type< 8> { using type = std:: int8_t; };
      template <> struct int_type<16> { using type = std::int16_t; };
      template <> struct int_type<32> { using type = std::int32_t; };
      template <> struct int_type<64> { using type = std::int64_t; };

      template <bool, typename T> struct as_signed          { using type = std::make_unsigned_t<T>; };
      template <      typename T> struct as_signed<true, T> { using type = std::  make_signed_t<T>; };
  }

  //================================================================================================
  //! @ingroup utility
  //! @brief  Type of the @ref kwk::_ object
  //================================================================================================
  struct joker
  {
    KWK_TRIVIAL friend std::ostream& operator<<(std::ostream& os, joker const&) { return os << "_"; }

    KWK_TRIVIAL friend constexpr bool   operator==(joker,joker) { return true;  }
    KWK_TRIVIAL friend constexpr bool   operator==(joker,auto ) { return false; }
    KWK_TRIVIAL friend constexpr bool   operator==(auto ,joker) { return false; }

    KWK_TRIVIAL friend constexpr joker  operator*(joker, joker) noexcept { return {}; }
    KWK_TRIVIAL friend constexpr joker  operator*(joker,  auto) noexcept { return {}; }
    KWK_TRIVIAL friend constexpr joker  operator*(auto , joker) noexcept { return {}; }

    std::int8_t integral_value_type_size = KWK_DEFAULT_AXIS_BITNESS;

    template <typename Integral>
    constexpr joker operator[](Integral /*const v*/) const noexcept { /*ct_assert(v == 0)*/ return { std::int8_t(sizeof(Integral)) * (std::is_signed_v<Integral> ? -8 : 8 )}; }

    template <int bitness = KWK_DEFAULT_AXIS_BITNESS>
    using value_type = typename detail::as_signed
    <
        bitness < 0,
        typename detail::int_type<bitness < 0 ? -bitness : bitness>::type
    >::type;
  };

  //================================================================================================
  //! @ingroup traits
  //! @brief Checks a type is equivalent to @ref kwk::joker
  //!
  //! @tparam T Type to check
  //!
  //! **Helper value**
  //!
  //! @code{.cpp}
  //! template<typename T>
  //! inline constexpr bool is_joker_v = is_joker<T>::value;
  //! @endcode
  //!
  //================================================================================================
  template<typename T>  struct is_joker               : std::false_type {};
  template<>            struct is_joker<joker>        : std::true_type  {};
  template<>            struct is_joker<joker&>       : std::true_type  {};
  template<>            struct is_joker<joker const>  : std::true_type  {};
  template<>            struct is_joker<joker const&> : std::true_type  {};

  template<typename T>  inline constexpr bool is_joker_v = is_joker<T>::value;

  //================================================================================================
  //! @ingroup utility
  //! @brief Joker value
  //!
  //! The joker value defined as @ref kwk::_ is used in various context to mean 'just use the
  //! most meaningful value'.
  //================================================================================================
  inline constexpr joker _ = {};
}
