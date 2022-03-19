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
#include <cstdlib>

namespace kwk
{
  //================================================================================================
  /**
    @ingroup memory
    @brief Malloc based block allocator

    Model of the kwk::concepts::allocator concept. kwk::heap_allocator allocates and deallocates
    memory block on the heap using `malloc` and `free`.
  **/
  //================================================================================================
  struct heap_allocator
  {
    /**
      @brief  Allocates data on the heap
      @param  n Number of bytes to allocate
      @return A kwk::block wrapping the newly allocated memory and its size. If zero byte was
              requested, the returned kwk::block is empty.
    **/
    [[nodiscard]] block allocate(std::ptrdiff_t n) noexcept
    {
      return (n!=0) ? block{ malloc(n), n } : block{ nullptr, n };
    }

    /**
      @brief  Deallocates data on the heap

      Free the heap memory from a kwk::block. If the deallocated memory was not allocated by an
      instance of kwk::heap_allocator, the behavior is undefined.

      @param  b kwk::block containing the memory to deallocate
    **/
    void deallocate(block & b) noexcept { if(b.data) free(b.data); }

    /// Swap the contents of two instance of kwk::heap_allocator
    void swap(heap_allocator&) {}
  };
}
