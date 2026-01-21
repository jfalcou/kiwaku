//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/memory/heap_allocator.hpp>

TTS_CASE( "Nullpotent Allocation/Deallocation through heap_allocator" )
{
  kwk::heap_allocator alloc;

  auto memory = allocate( alloc, 0 );
  TTS_EQUAL ( memory   , nullptr );
  deallocate(alloc, memory);
};

TTS_CASE( "Allocation/Deallocation through heap_allocator" )
{
  kwk::heap_allocator alloc;
  auto memory = allocate( alloc, 64 );

  TTS_NOT_EQUAL ( memory, nullptr );
  deallocate(alloc, memory);
};
