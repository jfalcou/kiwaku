//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk
{
  namespace __
  {
    struct wildcard_t
    {
      static constexpr int value = 0;

      consteval operator int() const noexcept { return value; }

      KWK_TRIVIAL friend constexpr bool operator==(wildcard_t, wildcard_t) { return true; }

      KWK_TRIVIAL friend constexpr bool operator==(wildcard_t, auto) { return false; }

      KWK_TRIVIAL friend constexpr bool operator==(auto, wildcard_t) { return false; }

      KWK_TRIVIAL friend constexpr wildcard_t operator*(wildcard_t, wildcard_t) noexcept { return {}; }

      KWK_TRIVIAL friend constexpr wildcard_t operator*(wildcard_t, auto) noexcept { return {}; }

      KWK_TRIVIAL friend constexpr wildcard_t operator*(auto, wildcard_t) noexcept { return {}; }
    };
  }

  //====================================================================================================================
  /**
    @ingroup utility
    @brief Global wildcard instance
  **/
  //====================================================================================================================
  inline constexpr __::wildcard_t _{};

  //====================================================================================================================
  /**
    @ingroup utility
    @brief Checks if a type is the wildcard type

    @tparam T Type to analyze.
  **/
  //====================================================================================================================
  template<typename T> inline constexpr bool is_wildcard = std::same_as<std::remove_cvref_t<T>, __::wildcard_t>;
}
