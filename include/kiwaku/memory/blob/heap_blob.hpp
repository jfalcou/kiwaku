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
  struct heap_blob
  {
    any_allocator allocator;
    std::byte*    base_   = nullptr;

    constexpr heap_blob() = default;

    template<kwk::concepts::allocator A>
    constexpr heap_blob(A a, std::size_t size)
        : allocator(std::move(a))
        , base_ {static_cast<std::byte*>(allocate(allocator, size))}
    {
    }

    // Copy is deleted, that is the role of the collection as the blob does not know it's size
    heap_blob(heap_blob const&)            = delete;
    heap_blob& operator=(heap_blob const&) = delete;

    constexpr heap_blob(heap_blob&& other) noexcept
        : allocator(std::move(other.allocator))
        , base_(std::exchange(other.base_, nullptr))
    {
    }

    constexpr heap_blob& operator=(heap_blob&& other) noexcept
    {
      if(this != &other)
      {
        release();
        allocator = std::move(other.allocator);
        base_     = std::exchange(other.base_, nullptr);
      }
      return *this;
    }

    constexpr ~heap_blob() { release(); }

    std::byte*       get() noexcept { return base_; }
    std::byte const* get() const noexcept { return base_; }

    void             release()
    {
      if(base_)
      {
        deallocate(allocator, base_);
        base_ = nullptr;
      }
    }

    constexpr void swap(heap_blob& other) noexcept
    {
      if(this != &other)
      {
        allocator = std::exchange(other.allocator, std::move(allocator));
        base_     = std::exchange(other.base_, base_);
      }
    }

    friend constexpr void swap(heap_blob& self, heap_blob& other) noexcept
    {
      return self.swap(other);
    }
  };
}
