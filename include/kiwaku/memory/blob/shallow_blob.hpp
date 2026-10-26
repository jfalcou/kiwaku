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
  struct shallow_blob
  {
    std::byte*            base_ = nullptr;

    std::byte*            get() noexcept { return base_; }
    std::byte const*      get() const noexcept { return base_; }

    constexpr void        swap(shallow_blob& other) noexcept { std::swap(base_, other.base_); }

    constexpr friend void swap(shallow_blob& self, shallow_blob& other) noexcept
    {
      self.swap(other);
    }
  };
}
