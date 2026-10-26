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
  template<std::ptrdiff_t Size, std::size_t Alignment = alignof(std::max_align_t)> struct stack_blob
  {
    alignas(Alignment) std::byte data_[ Size ];

    std::byte*       get() noexcept { return &data_[ 0 ]; }
    std::byte const* get() const noexcept { return &data_[ 0 ]; }

    constexpr void   swap(stack_blob& other) noexcept
    {
      for(std::ptrdiff_t i = 0; i < Size; ++i) std::swap(data_[ i ], other.data_[ i ]);
    }

    constexpr friend void swap(stack_blob& a, stack_blob& b) noexcept { a.swap(b); }
  };
}
