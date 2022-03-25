//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <kwk/detail/assert.hpp>
#include <cstdlib>

namespace kwk
{
  //================================================================================================
  /**
    @ingroup memory
    @brief Malloc based  allocator

    Model of the kwk::concepts::allocator concept. kwk::heap_allocator allocates and deallocates
    memory block on the heap using `malloc` and `free`.
  **/
  //================================================================================================
  struct heap_allocator
  {
    /**
      @brief  Allocates data on the heap
      @param  n Number of bytes to allocate
      @return A pointer to the newly allocated memory. If zero byte was requested, the returned
              pointer is equal to `nullptr`.
    **/
    [[nodiscard]] void* allocate(std::ptrdiff_t n) noexcept
    {
      return (n!=0) ? malloc(n) : nullptr;
    }

    /**
      @brief  Deallocates data on the heap

      Free the heap memory from a pointer. If the deallocated memory was not allocated by an
      instance of kwk::heap_allocator, the behavior is undefined.

      @param  b Pointer to the memory to deallocate
    **/
    void deallocate(void* ptr) noexcept { if(ptr) free(ptr); }

    /// Swap the contents of two instance of kwk::heap_allocator
    void swap(heap_allocator&) {}
  };
}
