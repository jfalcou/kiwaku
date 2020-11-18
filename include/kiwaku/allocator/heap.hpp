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
#include <cstdlib>

namespace kwk
{
  struct heap_allocator
  {
    [[nodiscard]] block allocate(bytes n) noexcept
    {
      return (n!=0) ? block{ malloc(to_integer(n)), n } : block{ nullptr, n };
    }

    bool reallocate(block & b, bytes n) noexcept
    {
      if(n > b.length)
      {
        block new_block{ realloc(b.data, to_integer(n)), n };

        if(new_block)
        {
          b = new_block;
          return true;
        }

        return false;
      }
      else
      {
        return true;
      }
    }

    bool expand(block& b, std::ptrdiff_t delta) noexcept
    {
      if(!delta) return true;
      return reallocate(b,bytes{b.length+delta});
    }

    void deallocate(block & b) noexcept { free(b.data); }
  };
}
