//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <concepts>
#include <kiwaku/allocator/block.hpp>

namespace kwk::concepts
{
  //================================================================================================
  /**
    @ingroup concept

    @brief Allocator concept

    A **KIWAKU** Allocator is a @semiregular, @swappable type which provides:
      + a `allocate` member function that takes an integral value and return a block
      + a `deallocate` member function that takes a lvalue reference to a block as parameter
  **/
  //================================================================================================
  template<typename A>
  concept allocator =   std::semiregular<A>
                    &&  std::swappable<A>
                    &&  requires(A a, block& b, std::ptrdiff_t n)
  {
    { a.allocate(n)   } -> std::same_as<block>;
    { a.deallocate(b) };
  };
}
