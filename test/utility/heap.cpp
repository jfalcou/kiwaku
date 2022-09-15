//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier:   KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/allocator/heap_allocator.hpp>

TTS_CASE( "Nullpotent Allocation/Deallocation through heap_allocator" )
{
  kwk::heap_allocator a;

  auto memory = a.allocate( 0 );
  TTS_EQUAL ( memory   , nullptr );

  a.deallocate(memory);
};

TTS_CASE( "Allocation/Deallocation through heap_allocator" )
{
  kwk::heap_allocator a;

  auto memory = a.allocate( 64 );

  TTS_NOT_EQUAL ( memory, nullptr );

  a.deallocate(memory);
};
