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

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, wildcard_t) noexcept
      {
        return os << "_";
      }
    };
  }

  //====================================================================================================================
  /**
    @ingroup utility
    @brief Global wildcard instance
  **/
  //====================================================================================================================
  inline constexpr __::wildcard_t _{};
}
