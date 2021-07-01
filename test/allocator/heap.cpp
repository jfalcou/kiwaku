//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/allocator/heap_allocator.hpp>

TTS_CASE( "Nullpotent Allocation/Deallocation through heap_allocator" )
{
  kwk::heap_allocator a;

  auto memory = a.allocate( 0 );

  TTS_EQUAL ( memory.data   , nullptr );
  TTS_EQUAL ( memory.length , 0     );

  a.deallocate(memory);
};

TTS_CASE( "Allocation/Deallocation through heap_allocator" )
{
  kwk::heap_allocator a;

  auto memory = a.allocate( 64 );

  TTS_NOT_EQUAL ( memory.data   , nullptr );
  TTS_EQUAL     ( memory.length , 64      );

  a.deallocate(memory);
};
