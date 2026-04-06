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
    @defgroup utility Miscellaneous Helpers
    @brief Quality of Life helpers

    This module defines various functions, types and variables for improving Quality of Life when using **KIWAKU**.
  **/
  //====================================================================================================================
}

#include <kwk/detail/detail.hpp>
#include <kwk/utility/memory.hpp>
#include <kwk/utility/shape.hpp>

namespace kwk
{
  inline namespace literals
  {
#if !defined(KWK_DOXYGEN_INVOKED)
    using kumi::operator""_c;
#else
    //==================================================================================================================
    /**
      @ingroup utility
      @brief Forms a integral constant literal of the desired value.
      @return An implementation-defined integral constant for the specified integral value
    **/
    //==================================================================================================================
    template<char... c> constexpr _::_ operator""_c() noexcept;
#endif
  }
}
