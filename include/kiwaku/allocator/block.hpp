//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <utility>

namespace kwk
{
  //================================================================================================
  //  block is the basic element returned by allocator.
  //  It contains the pointer and the allocated length of said block
  //================================================================================================
  struct block
  {
    void swap(block& other)
    {
      std::swap(data  , other.data  );
      std::swap(length, other.length);
    }

    void reset() noexcept
    {
      data    = nullptr;
      length  = 0;
    }

    explicit operator bool() const { return length != 0; }
    bool operator==(const block &rhs) const { return data == rhs.data && length == rhs.length; }
    bool operator!=(const block &rhs) const { return data != rhs.data || length != rhs.length; }

    void*           data    = nullptr;
    std::ptrdiff_t  length  = 0;
  };
}
