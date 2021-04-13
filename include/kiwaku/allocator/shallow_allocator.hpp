//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/assert.hpp>
#include <kiwaku/allocator/block.hpp>
#include <kiwaku/detail/round.hpp>
#include <cstring>

namespace kwk
{
  template<std::ptrdiff_t Size, std::size_t Alignment>
  struct shallow_allocator
  {
    static constexpr auto max_capacity = Size;

    shallow_allocator ( void* s = nullptr ) noexcept
                      : data_{static_cast<std::byte*>(s)}, current_{data_}
    {}

    shallow_allocator ( shallow_allocator const& o ) noexcept
                      : data_(o.data_), current_(data_)
    {}

    [[nodiscard]] block allocate(std::ptrdiff_t n) noexcept
    {
      // empty allocation return empty blocks
      if(!n) { return {nullptr, 0}; }

      auto const aligned_n = detail::round_to(n,Alignment);
      auto const end       = data_ + max_capacity;

      // If there is not enough space, return empty block
      if(aligned_n > (end - current_)) { return {nullptr, 0}; }

      // Build block then push current pointer to the next slot
      block that{current_, aligned_n};
      current_ += aligned_n;

      return that;
    }

    void deallocate(block & b) noexcept
    {
      // If deallocated block is the last one allocated, retrieve the space
      if( is_last(b) ) { current_ = static_cast<std::byte*>(b.data); }
    }

    inline void           reset()           noexcept { current_ = data_; }
    inline std::ptrdiff_t capacity() const  noexcept { return data_ + max_capacity - current_; }

    inline bool owns(block const& b) const noexcept
    {
      auto begin = data_;
      return b.data >= begin && b.data < (begin+max_capacity);
    }

    private:

    inline bool is_last(block const& b) const noexcept
    {
      auto const ptr = static_cast<std::byte*>(b.data);
      return (ptr + detail::round_to(b.length,Alignment)) == current_;
    }

    std::byte*  data_;
    std::byte*  current_;
  };

  template<std::ptrdiff_t Size, std::ptrdiff_t Align> auto make_shallow( void* data ) noexcept
  {
    return shallow_allocator<Size, Align>(data);
  }

  template<std::ptrdiff_t Size> auto make_shallow( void* data ) noexcept
  {
    return shallow_allocator<Size, 16>(data);
  }
}
