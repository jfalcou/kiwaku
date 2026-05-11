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

  //====================================================================================================================
  /**
    @ingroup utility
    @brief Checks if a type is the wildcard type

    @tparam T Type to analyze.
  **/
  //====================================================================================================================
  template<typename T> struct is_wildcard : std::is_same<std::remove_cvref_t<T>, __::wildcard_t>
  {
  };

  template<typename T> constexpr bool is_wildcard_v = is_wildcard<T>::value;
}
