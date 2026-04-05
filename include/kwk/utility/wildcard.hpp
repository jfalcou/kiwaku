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

      consteval operator int() const noexcept { return 0; }
    };
  }

  /// Global wildcard instance
  inline constexpr __::wildcard_t _{};

  template<typename T> inline constexpr bool is_wildcard = std::same_as<std::remove_cvref_t<T>, __::wildcard_t>;
}
