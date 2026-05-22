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
    using storage_type = std::byte;
    using ptr_t = std::add_pointer_t<storage_type>;

    struct deleter
    {
      any_allocator allocator;

      constexpr deleter() {}

      template<typename A> constexpr deleter(A a) : allocator(a) {}

      void operator()(void* p) { deallocate(allocator, p); }
    };

    constexpr heap_blob() noexcept : handler(heap_allocator{}), data(nullptr) {}

    template<typename Allocator>
    constexpr heap_blob(Allocator a, std::size_t size)
      : handler(a), data(reinterpret_cast<ptr_t>(reinterpret_cast<std::uintptr_t>(allocate(a, size))))
    {
    }

    constexpr void swap(heap_blob& other) noexcept
    {
      std::swap(handler, other.handler);
      std::swap(data, other.data);
    }

    constexpr friend void swap(heap_blob& a, heap_blob& b) noexcept { a.swap(b); }

    KWK_TRIVIAL constexpr decltype(auto) get() noexcept { return data; }

    deleter handler;
    ptr_t data;
  };
}
