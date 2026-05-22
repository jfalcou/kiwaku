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
  template<std::size_t Alignment, std::ptrdiff_t Size> struct stack_blob
  {
    using storage_type = std::byte;

    constexpr stack_blob() = default;

    KWK_TRIVIAL constexpr auto get() noexcept { return &data[0]; }

    KWK_TRIVIAL constexpr auto get() const noexcept { return &data[0]; }

    constexpr void swap(stack_blob& other) noexcept
    {
      for (std::ptrdiff_t i = 0; i < Size; ++i) std::swap(data[i], other.data[i]);
    }

    constexpr friend void swap(stack_blob& a, stack_blob& b) noexcept { a.swap(b); }

    alignas(Alignment) storage_type data[Size];
  };
}
