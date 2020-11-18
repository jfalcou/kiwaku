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
#include <kiwaku/allocator/size.hpp>
#include <cstring>
#include <array>

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

    [[nodiscard]] block allocate(bytes n) noexcept
    {
      // empty allocation return empty blocks
      if(!n) { return {nullptr, bytes{0}}; }

      auto const aligned_n = round_to(n,Alignment);
      auto const end       = data_ + max_capacity;

      // If there is not enough space, return empty block
      if(aligned_n > (end - current_)) { return {nullptr, bytes{0}}; }

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

    bool expand(block& b, std::ptrdiff_t delta) noexcept
    {
      if(!delta) return true;

      bytes sz{b.length+delta};

      if( is_last(b) )
      {
        //  When expanding the last block in the chain, we just re-adjust pointers
        //  and don't perform copy of the old data
        b.length = round_to(sz,Alignment);
        current_ = static_cast<std::byte*>(b.data) + b.length;
        return true;
      }
      else
      {
        //  If we reallocate for more space and we end up elsewhere, we reallocate with a copy
        auto new_block = allocate(sz);

        if(new_block)
        {
          ::memcpy(new_block.data, b.data, to_integer(b.length));
          deallocate(b);
          b = new_block;
          return true;
        }

        return false;
      }
    }

    bool reallocate(block & b, bytes n) noexcept
    {
      // Reallocating to 0 size is trivial
      if(!n)
      {
        deallocate(b);
        b.reset();
        return true;
      }

      // If required, use expand
      return (n >= b.length) ? expand(b, to_integer(n-b.length) ) : true;
    }

    inline void   reset()           noexcept { current_ = data_; }
    inline bytes  capacity() const  noexcept { return bytes{data_ + max_capacity - current_}; }

    inline bool owns(block const& b) const noexcept
    {
      auto begin = data_;
      return b.data >= begin && b.data < (begin+max_capacity);
    }

    private:

    inline bool is_last(block const& b) const noexcept
    {
      auto const ptr = static_cast<std::byte*>(b.data);
      return (ptr + round_to(b.length,Alignment)) == current_;
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
