//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_CT_HELPERS_HPP_INCLUDED
#define KIWAKU_DETAIL_CT_HELPERS_HPP_INCLUDED

#include <type_traits>

namespace kwk::detail
{
  //================================================================================================
  // Check for static content option
  //================================================================================================
  template<auto Option, typename Yes, typename No>
  using has_contents = std::conditional_t<Option.is_dynamic_option, Yes, No>;

  //================================================================================================
  // Empty struct for EBO cases
  //================================================================================================
  template<int N = -1> struct empty {};
}

#endif
