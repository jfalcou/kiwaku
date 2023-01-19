//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <ostream>

namespace kwk
{
  //================================================================================================
  //! @ingroup utility
  //! @brief  Type of the @ref kwk::_ object
  //================================================================================================
  struct joker
  {
    friend std::ostream& operator<<(std::ostream& os, joker const&) { return os << "_"; }

    friend constexpr joker operator*(joker, joker) noexcept { return {}; }
    friend constexpr joker operator*(joker,  auto) noexcept { return {}; }
    friend constexpr joker operator*(auto , joker) noexcept { return {}; }
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
  template<>            struct is_joker<joker>        : std::true_type {};
  template<>            struct is_joker<joker&>       : std::true_type {};
  template<>            struct is_joker<joker const>  : std::true_type {};
  template<>            struct is_joker<joker const&> : std::true_type {};

  template<typename T>  inline constexpr bool is_joker_v = is_joker<T>::value;

  //================================================================================================
  //! @ingroup utility
  //! @brief Joker value
  //!
  //! The joker value defined as @ref kwk::_ is used in various context to means 'just use the
  //! most meaningful value'.
  //================================================================================================
  inline constexpr joker _ = {};
}
