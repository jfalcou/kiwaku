//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/allocator/heap.hpp>

TTS_CASE( "Nullpotent Allocation/Deallocation through heap_allocator" )
{
  using namespace kwk::literals;
  kwk::heap_allocator a;

  auto memory = a.allocate( 0_B );

  TTS_EQUAL ( memory.data   , nullptr );
  TTS_EQUAL ( memory.length , 0_B     );

  a.deallocate(memory);
}

TTS_CASE( "Allocation/Deallocation through heap_allocator" )
{
  using namespace kwk::literals;
  kwk::heap_allocator a;

  auto memory = a.allocate( 64_B );

  TTS_NOT_EQUAL ( memory.data   , nullptr );
  TTS_EQUAL     ( memory.length , 64_B    );

  a.deallocate(memory);
}

