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
    using storage_type = std::byte;
    using ptr_t = std::add_pointer_t<storage_type>;

    constexpr shallow_blob() : data(nullptr) {}

    template<typename T>
    constexpr shallow_blob(T* ptr) : data(reinterpret_cast<ptr_t>(reinterpret_cast<std::uintptr_t>(ptr)))
    {
    }

    constexpr void swap(shallow_blob& other) noexcept { std::swap(data, other.data); }

    constexpr friend void swap(shallow_blob& a, shallow_blob& b) noexcept { a.swap(b); }

    KWK_TRIVIAL constexpr decltype(auto) get() noexcept { return data; }

    ptr_t data;
  };
}
