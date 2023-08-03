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

#ifndef KWK_DEFAULT_JOKER_UNDERLYING_TYPE
#define KWK_DEFAULT_JOKER_UNDERLYING_TYPE int;
#endif

namespace kwk
{
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

    using default_type = KWK_DEFAULT_JOKER_UNDERLYING_TYPE;
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

  inline constexpr auto extent(joker) noexcept { return joker::default_type{0}; } // use zero as a numeric (compile time) value to represent dynamic/runtime axes?

  namespace __
  {
      template<typename T>  struct  to_int;
      template<>            struct  to_int<joker> { using type = joker::default_type; };
  }
}
