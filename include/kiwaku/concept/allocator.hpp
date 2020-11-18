//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/allocator/block.hpp>
#include <kiwaku/allocator/bytes.hpp>
#include <concepts>

namespace kwk::concepts
{
  template<typename A>
  concept allocator = std::semiregular<A> && requires(A a, block& b)
  {
    { a.allocate(bytes{})      } -> std::same_as<block>;
    { a.reallocate(b, bytes{}) } -> std::same_as<bool>;
    { a.deallocate(b)          };
    { a.expand(b, 0 )          } -> std::same_as<bool>;
  };
}
