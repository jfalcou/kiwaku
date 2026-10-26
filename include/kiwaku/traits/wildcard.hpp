//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>

namespace kwk
{
  //====================================================================================================================
  /**
    @ingroup utility
    @brief Checks if a type is the wildcard type

    @tparam T Type to analyze.
  **/
  //====================================================================================================================
  template<typename T>
  inline constexpr bool is_wildcard_v = std::same_as<std::remove_cvref_t<T>, kwk::wildcard_t>;

  template<typename T> struct is_wildcard
  {
    static constexpr bool value = is_wildcard_v<T>;
  };
}
