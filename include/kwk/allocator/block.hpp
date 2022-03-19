//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#pragma once

#include <utility>

namespace kwk
{
  //================================================================================================
  /**
    @ingroup memory
    @brief Memory block handler

    kwk::block is a thin wrapper around memory returned by an kwk::concepts::allocator.
    It's stores both a pointer to the owned memory block and the actual allocated size of said
    memory block.
  **/
  //================================================================================================
  struct block
  {
    /**
      @brief Swap the contents of a kwk::block with another
      @param other kwk::block to swap
    **/
    void swap(block& other)
    {
      std::swap(data  , other.data  );
      std::swap(length, other.length);
    }

    /**
      @brief Reset a kwk::block
      Sets the length and pointer to the memory to 0.

      @warning This does not free the ressources reference by kwk::block::data
    **/
    void reset() noexcept
    {
      data    = nullptr;
      length  = 0;
    }

    /**
      @brief Boolean conversion operator
      @return `true` if curren kwk::block length is not equal to zero.
    **/
    explicit operator bool() const { return length != 0; }

    /**
      @brief Equality comparison operator

      @param  lhs, rhs  kwk::block to compare
      @return `true` if lhs and rhs have the same length and reference the same memory block,
              `false` otherwise.
    **/
    friend bool operator==(block const& lhs,block const& rhs) noexcept
    {
      return lhs.data == rhs.data && lhs.length == rhs.length;
    }

    /**
      @brief Inequality comparison operator

      @param  lhs, rhs  kwk::block to compare
      @return `false` if lhs and rhs have the same length and reference the same memory block,
              `true` otherwise.
    **/
    friend bool operator!=(block const& lhs,block const& rhs) noexcept
    {
      return lhs.data != rhs.data || lhs.length != rhs.length;
    }

    /// Pointer to the allocated block of memory
    void* data  = nullptr;

    /// Actual size in bytes of the allocated block of memory
    std::ptrdiff_t  length  = 0;
  };
}
