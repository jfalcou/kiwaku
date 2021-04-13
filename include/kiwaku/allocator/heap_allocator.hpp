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
#include <cstdlib>

namespace kwk
{
  struct heap_allocator
  {
    [[nodiscard]] block allocate(std::ptrdiff_t n) noexcept
    {
      return (n!=0) ? block{ malloc(n), n } : block{ nullptr, n };
    }

    void deallocate(block & b) noexcept { if(b.data) free(b.data); }

    void swap(heap_allocator&) {}
  };
}
