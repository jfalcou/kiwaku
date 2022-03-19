//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/detail/assert.hpp>
#include <kwk/allocator/block.hpp>
#include <kwk/detail/round.hpp>
#include <cstdlib>
#include <cstring>

namespace kwk
{
  //================================================================================================
  /**
    @ingroup memory
    @brief External ressources block allocator

    Model of the kwk::concepts::allocator concept. kwk::shallow_allocator allocates and deallocates
    memory block using a pre-existing, externally owned memory block source.

    @tparam Size      Total capacity a kwk::shallow_allocator can allocate during its lifetime
    @tparam Alignment Alignment of memory block allcoated by kwk::shallow_allocator
  **/
  //================================================================================================
  template<std::ptrdiff_t Size, std::size_t Alignment>
  struct shallow_allocator
  {
    /// Total capacity a kwk::shallow_allocator can allocate during its lifetime
    static constexpr auto max_capacity = Size;

    /**
      @brief Default constructor
      Constructs a kwk::shallow_allocator referencing no source of memory blocks.
    **/
    shallow_allocator() noexcept : shallow_allocator(nullptr) {}

    /**
      @brief Ressource-binding constructor
      Constructs a kwk::shallow_allocator referencing an existent source of memory blocks.

      @param source Pointer to a memory region containing `Size` byte worth of memory blocks.
    **/
    shallow_allocator ( void* source ) noexcept
                      : data_{static_cast<std::byte*>(source)}, current_{data_}
    {}

    /**
      @brief Copy constructor
      Constructs a kwk::shallow_allocator by referencing the same source of memory block
      than another one.

      Note that copied kwk::shallow_allocator will use both the same source of memory block
      and the current state of allocations.

      @param other kwk::shallow_allocator to copy
    **/
    shallow_allocator ( shallow_allocator const& other ) noexcept
                      : data_(other.data_), current_(other.data_)
    {}

    /**
      @brief  Allocates data in the memory source

      Retrieve an free block of memory from the memory source and mark it as occupied.

      @param  n Number of bytes to allocate
      @return A kwk::block wrapping the newly allocated memory and its size. If zero byte was
              requested or if no space is left in the memory source, the returned kwk::block is
              empty.
    **/
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

    /**
      @brief  Deallocates data in a memory source

      If the block to deallocate is the last one allcoated, return the memory space to the
      memory source. Otherwise, do nothing.

      @param  b kwk::block containing the memory to deallocate
    **/
    void deallocate(block & b) noexcept
    {
      if( is_last(b) ) current_ = static_cast<std::byte*>(b.data);
    }

    /// Deallocates all memory owned by current kwk::shallow_allocator
    inline void           reset()           noexcept { current_ = data_; }

    /// Returns the current capacity of the kwk::shallow_allocator instance
    inline std::ptrdiff_t capacity() const  noexcept { return data_ + max_capacity - current_; }

    /// Checks if a kwk::block has been allocated by current instance of kwk::shallow_allocator
    inline bool owns(block const& b) const noexcept
    {
      auto begin = data_;
      return b.data >= begin && b.data < (begin+max_capacity);
    }

    /// Swap the contents of two instance of kwk::shallow_allocator
    void swap(shallow_allocator& other)
    {
      std::swap(data_   , other.data_   );
      std::swap(current_, other.current_);
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
