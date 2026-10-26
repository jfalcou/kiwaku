//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <cstdlib>

namespace kwk
{
  template<std::size_t Size, std::size_t Alignment = alignof(std::max_align_t)>
  struct stack_allocator
  {
  };

  //====================================================================================================================
  /**
    @ingroup memory
    @brief Stack based allocator

    Propagates informations for a stack allocation consisting of a Size and an Alignment
  */
  //====================================================================================================================
  template<std::size_t Size, std::size_t Alignment>
  inline constexpr stack_allocator<Size, Alignment> stack = {};
}
