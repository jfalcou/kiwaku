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
  template<std::ptrdiff_t Size, std::size_t Alignment = 16>
  struct stack_allocator
  {
    static constexpr auto max_capacity = Size;

    stack_allocator () noexcept : data_{}, current_(&data_[0]) {}

    stack_allocator ( stack_allocator const& o) noexcept
                    : data_(o.data_), current_(&data_[0])
    {}

    [[nodiscard]] block allocate(bytes n) noexcept
    {
      // empty allocation return empty blocks
      if(!n) { return {nullptr, bytes{0}}; }

      auto const aligned_n = round_to(n,Alignment);
      auto const end       = &data_[0] + max_capacity;

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

    inline void   reset()           noexcept { current_ = &data_[0]; }
    inline bytes  capacity() const  noexcept { return bytes{&data_[0] + max_capacity - current_}; }

    inline bool owns(block const& b) const noexcept
    {
      auto begin = &data_[0];
      return b.data >= begin && b.data < (begin+max_capacity);
    }

    private:

    inline bool is_last(block const& b) const noexcept
    {
      auto const ptr = static_cast<std::byte*>(b.data);
      return (ptr + round_to(b.length,Alignment)) == current_;
    }

    alignas(Alignment) std::array<std::byte,max_capacity> data_;
    std::byte*                                            current_;
  };
}
