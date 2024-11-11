//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <cstddef>

namespace kwk::concepts
{
  //================================================================================================
  //! @brief **KIWAKU** allocator concept
  //!
  //! A **KIWAKU** Allocator is a type which instances can be passed as parameter to the following
  //! functions:
  //!   + `allocate(Allocator& alloc, std::size_t size)`
  //!   + `deallocate(Allocator& alloc, void* ptr)`
  //================================================================================================
  template<typename Allocator>
  concept allocator = requires(Allocator& alloc, void* ptr, std::size_t size)
  {
    { allocate  (alloc, size) };
    { deallocate(alloc, ptr ) };
  };
}
