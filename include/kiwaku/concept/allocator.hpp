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
  template<typename A>
  concept allocator = std::semiregular<A> && requires(A a, block& b, std::ptrdiff_t n)
  {
    { a.allocate(n)   } -> std::same_as<block>;
    { a.deallocate(b) };
  };
}
