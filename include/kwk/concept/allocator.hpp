//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <concepts>

namespace kwk::concepts
{
  //================================================================================================
  /**
    @brief Allocator concept

    A **KIWAKU** Allocator is a @semiregular, @swappable type which provides:
      + a `allocate` member function that takes an integral value and allocates bytes
      + a `deallocate` member function that takes a pointer to bytes as parameter
  **/
  //================================================================================================
  template<typename A>
  concept allocator =   std::semiregular<A>
                    &&  std::swappable<A>
                    &&  requires(A a, void* b, std::ptrdiff_t n)
  {
    { a.allocate(n)   };
    { a.deallocate(b) };
  };

  //================================================================================================
  /**
    @brief Aligned Allocator concept

    A **KIWAKU** Aligned Allocator is a kwk::concepts::allocator that also provides:
      + a `allocate_aligned` member function and allocates aligned memory on a given alignment
      + a `deallocate_aligned` member function that deallocates aligned memory
  **/
  //================================================================================================
  template<typename A>
  concept aligned_allocator =   allocator<A>
                            &&  requires(A a, void* b, std::ptrdiff_t n, std::ptrdiff_t al)
  {
    { a.allocate_aligned(n,al) };
    { a.deallocate_aligned(b) };
  };
}
