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
  struct wildcard_t
  {
    using value_type = std::size_t;
    static constexpr value_type value = 0;

    consteval operator value_type() const noexcept { return value; }

    KWK_ABI friend constexpr bool operator==(wildcard_t, wildcard_t) { return true; }

    KWK_ABI friend constexpr bool operator==(wildcard_t, auto) { return false; }

    KWK_ABI friend constexpr bool operator==(auto, wildcard_t) { return false; }

    KWK_ABI friend constexpr wildcard_t operator*(wildcard_t, wildcard_t) noexcept { return {}; }

    KWK_ABI friend constexpr wildcard_t operator*(wildcard_t, auto) noexcept { return {}; }

    KWK_ABI friend constexpr wildcard_t operator*(auto, wildcard_t) noexcept { return {}; }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, wildcard_t) noexcept
    {
      return os << "_";
    }
  };

  //====================================================================================================================
  /**
    @ingroup utility
    @brief Global wildcard instance
  **/
  //====================================================================================================================
  inline constexpr wildcard_t _{};
}
