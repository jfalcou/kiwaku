//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/allocator/shallow_allocator.hpp>

TTS_CASE( "Allocation/Deallocation through shallow_allocator" )
{
  std::byte data[16];
  auto a = kwk::make_shallow<16,8>(data);

  TTS_WHEN("Setup a 16 bytes allocator")
  {
    TTS_AND_THEN( "allocates 0 byte" )
    {
      auto memory = a.allocate(0);

      TTS_EQUAL     ( memory.data   , nullptr );
      TTS_EQUAL     ( memory.length , 0       );
      TTS_EXPECT_NOT( a.owns(memory)          );
      TTS_EQUAL     ( a.capacity()  , 16      );
    }

    TTS_AND_THEN( "allocates 1 byte" )
    {
      auto memory = a.allocate(1);

      TTS_NOT_EQUAL     ( memory.data   , nullptr );
      TTS_GREATER_EQUAL ( memory.length , 1       );
      TTS_EXPECT        ( a.owns(memory)          );
      TTS_EQUAL         ( a.capacity()  , 8       );
    }

    TTS_AND_THEN( "allocates 15 bytes" )
    {
      auto memory = a.allocate(15);

      TTS_EQUAL     ( memory.data   , nullptr );
      TTS_EQUAL     ( memory.length , 0       );
      TTS_EXPECT_NOT( a.owns(memory)          );
      TTS_EQUAL     ( a.capacity()   , 8      );
    }

    TTS_AND_THEN( "deallocates then allocates 15 bytes" )
    {
      a.reset();
      auto memory = a.allocate(15);

      TTS_NOT_EQUAL     ( memory.data   , nullptr );
      TTS_GREATER_EQUAL ( memory.length , 15      );
      TTS_EXPECT        ( a.owns(memory)          );
      TTS_EQUAL         ( a.capacity()   , 0      );
    }

    TTS_AND_THEN( "deallocates then allocates 1000 bytes" )
    {
      a.reset();
      auto memory = a.allocate(1000);

      TTS_EQUAL     ( memory.data   , nullptr );
      TTS_EQUAL     ( memory.length , 0       );
      TTS_EXPECT_NOT( a.owns(memory)          );
      TTS_EQUAL     ( a.capacity()   , 16     );
    }
  }

  a.reset();
}

TTS_CASE( "Checks memory retrieval by shallow_allocator" )
{
  double data[8];
  auto a = kwk::make_shallow<64,16>(&data[0]);

  TTS_WHEN("A 64B allocator is setup")
  {
    TTS_AND_THEN( "a 1 allocation is performed once" )
    {
      auto b = a.allocate(1);
      TTS_EQUAL     ( a.capacity(), 48      );
      TTS_NOT_EQUAL ( b.data      , nullptr );
      TTS_EQUAL     ( b.length    , 16      );
    }

    TTS_AND_THEN( "a 1 allocation is performed twice" )
    {
      auto b = a.allocate(1);
      TTS_EQUAL     ( a.capacity(), 32      );
      TTS_NOT_EQUAL ( b.data      , nullptr );
      TTS_EQUAL     ( b.length    , 16      );
    }

    TTS_AND_THEN( "a 1 allocation is performed thrice" )
    {
      auto b = a.allocate(1);
      TTS_EQUAL     ( a.capacity(), 16      );
      TTS_NOT_EQUAL ( b.data      , nullptr );
      TTS_EQUAL     ( b.length    , 16      );
    }

    TTS_AND_THEN( "a 1 allocation is performed four times" )
    {
      auto b = a.allocate(1);
      TTS_EQUAL     ( a.capacity(), 0       );
      TTS_NOT_EQUAL ( b.data      , nullptr );
      TTS_EQUAL     ( b.length    , 16      );
    }

    TTS_AND_THEN( "a 1 allocation is performed five times" )
    {
      auto b = a.allocate(1);
      TTS_EQUAL ( a.capacity(), 0       );
      TTS_EQUAL ( b.data      , nullptr );
      TTS_EQUAL ( b.length    , 0       );
    }

    TTS_AND_THEN( "everything is released" )
    {
      a.reset();
      TTS_EQUAL( a.capacity(), 64 );
    }
  }
}
