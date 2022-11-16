//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/assert.hpp>
#include <cstdlib>

namespace kwk
{
  //================================================================================================
  //! @ingroup memory
  //! @brief Malloc based allocator
  //!
  //! Model of the kwk::concepts::allocator concept. kwk::heap allocates and deallocates
  //! memory block on the heap using `malloc` and `free`.
  //================================================================================================
  inline constexpr struct heap_allocator {} heap = {};

  //================================================================================================
  //! @ingroup memory
  //! @brief  Allocates data on the heap
  //! @param  n Number of bytes to allocate
  //! @return A pointer to the newly allocated memory. If zero byte was requested, the returned
  //!         pointer is equal to `nullptr`.
  //================================================================================================
  [[nodiscard]] inline void* allocate(heap_allocator const&, std::size_t n) noexcept
  {
    return (n!=0) ? malloc(n) : nullptr;
  }

  //================================================================================================
  //! @ingroup memory
  //! @brief  Deallocates data on the heap
  //!
  //! Free the heap memory from a pointer. If the deallocated memory was not allocated by an
  //! instance of kwk::heap_allocator, the behavior is undefined.
  //!
  //! @param ptr Pointer to the memory to deallocate
  //================================================================================================
  inline void deallocate(heap_allocator const&,void* ptr) noexcept { if(ptr) free(ptr); }
}
