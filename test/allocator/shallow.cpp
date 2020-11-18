//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/allocator/shallow.hpp>

TTS_CASE( "Allocation/Deallocation through shallow_allocator" )
{
  using namespace kwk::literals;

  std::byte data[16];
  auto a = kwk::make_shallow<16,8>(data);

  TTS_WHEN("Setup a 16 bytes allocator")
  {
    TTS_AND_THEN( "allocates 0 byte" )
    {
      auto memory = a.allocate(0_B);

      TTS_EQUAL     ( memory.data   , nullptr );
      TTS_EQUAL     ( memory.length , 0_B     );
      TTS_EXPECT_NOT( a.owns(memory)          );
      TTS_EQUAL     ( a.capacity()  , 16_B    );
    }

    TTS_AND_THEN( "allocates 1 byte" )
    {
      auto memory = a.allocate(1_B);

      TTS_NOT_EQUAL     ( memory.data   , nullptr );
      TTS_GREATER_EQUAL ( memory.length , 1_B     );
      TTS_EXPECT        ( a.owns(memory)          );
      TTS_EQUAL         ( a.capacity()  , 8_B     );
    }

    TTS_AND_THEN( "allocates 15 bytes" )
    {
      auto memory = a.allocate(15_B);

      TTS_EQUAL     ( memory.data   , nullptr );
      TTS_EQUAL     ( memory.length , 0_B     );
      TTS_EXPECT_NOT( a.owns(memory)          );
      TTS_EQUAL     (a.capacity()   , 8_B     );
    }

    TTS_AND_THEN( "deallocates then allocates 15 bytes" )
    {
      a.reset();
      auto memory = a.allocate(15_B);

      TTS_NOT_EQUAL     ( memory.data   , nullptr );
      TTS_GREATER_EQUAL ( memory.length , 15_B    );
      TTS_EXPECT        ( a.owns(memory)          );
      TTS_EQUAL         ( a.capacity()   , 0_B    );
    }

    TTS_AND_THEN( "deallocates then allocates 1000 bytes" )
    {
      a.reset();
      auto memory = a.allocate(1000_B);

      TTS_EQUAL     ( memory.data   , nullptr );
      TTS_EQUAL     ( memory.length , 0_B     );
      TTS_EXPECT_NOT( a.owns(memory)          );
      TTS_EQUAL     (a.capacity()   , 16_B    );
    }
  }

  a.reset();
}

TTS_CASE( "Allocation/Reallocation through shallow_allocator" )
{
  using namespace kwk::literals;

  int data[25];
  auto a = kwk::make_shallow<100,16>(&data[0]);

  TTS_WHEN("Allocate a block of memory")
  {
    auto memory     = a.allocate( 5_B );
    auto old_memory = memory.data;

    TTS_AND_THEN( "it's resized to a smaller size" )
    {
      TTS_EXPECT        ( a.reallocate(memory, 3_B) );
      TTS_EQUAL         ( memory.data   , old_memory);
      TTS_GREATER_EQUAL ( memory.length , 3_B       );
      TTS_EXPECT        ( a.owns(memory)            );
      TTS_EQUAL         ( a.capacity()  , 84_B      );

      a.deallocate(memory);
    }

    TTS_AND_THEN( "it's resized to a smaller size after another allocation" )
    {
      auto other_memory = a.allocate( 5_B );

      TTS_EXPECT        ( a.reallocate(memory, 3_B) );
      TTS_EQUAL         ( memory.data   , old_memory);
      TTS_GREATER_EQUAL ( memory.length , 3_B       );
      TTS_EXPECT        ( a.owns(memory)            );
      TTS_EQUAL         ( a.capacity()  , 68_B      );

      a.deallocate(other_memory);
      a.deallocate(memory);
    }

    TTS_AND_THEN( "it's resized to a larger size fitting the extra-space" )
    {
      TTS_EXPECT        ( a.reallocate(memory, 15_B));
      TTS_EQUAL         ( memory.data   , old_memory);
      TTS_GREATER_EQUAL ( memory.length , 15_B      );
      TTS_EXPECT        ( a.owns(memory)            );
      TTS_EQUAL         ( a.capacity()  , 84_B      );

      a.deallocate(memory);
    }

    TTS_AND_THEN( "it's resized to a larger size fitting the extra-space after another allocation" )
    {
      auto other_memory = a.allocate( 5_B );

      TTS_EXPECT        ( a.reallocate(memory, 15_B));
      TTS_EQUAL         ( memory.data   , old_memory);
      TTS_GREATER_EQUAL ( memory.length , 15_B      );
      TTS_EXPECT        ( a.owns(memory)            );
      TTS_EQUAL         ( a.capacity()  , 68_B      );

      a.deallocate(other_memory);
      a.deallocate(memory);
    }

    TTS_AND_THEN( "it's resized to a larger size not fitting the extra-space" )
    {
      TTS_EXPECT        ( a.reallocate(memory, 25_B));
      TTS_EQUAL         ( memory.data   , old_memory);
      TTS_GREATER_EQUAL ( memory.length , 25_B      );
      TTS_EXPECT        ( a.owns(memory)            );
      TTS_EQUAL         ( a.capacity()  , 68_B      );

      a.deallocate(memory);
    }

    TTS_AND_THEN( "it's resized to a larger size not fitting the extra-space after another allocation" )
    {
      auto other_memory = a.allocate( 5_B );

      TTS_EXPECT        ( a.reallocate(memory, 25_B));
      TTS_NOT_EQUAL     ( memory.data   , old_memory);
      TTS_GREATER_EQUAL ( memory.length , 25_B      );
      TTS_EXPECT        ( a.owns(memory)            );
      TTS_EQUAL         ( a.capacity()  , 36_B      );

      a.deallocate(other_memory);
      a.deallocate(memory);
    }

    TTS_AND_THEN( "it's resized to a nothing" )
    {
      TTS_EXPECT    ( a.reallocate(memory, 0_B) );
      TTS_EQUAL     ( memory.data   , nullptr   );
      TTS_EQUAL     ( memory.length , 0_B       );
      TTS_EXPECT_NOT( a.owns(memory)            );
      TTS_EQUAL     ( a.capacity()  , 68_B     );

      a.deallocate(memory);
    }
  }
}

TTS_CASE( "Checks memory retrieval by shallow_allocator" )
{
  using namespace kwk::literals;

  double data[8];
  auto a = kwk::make_shallow<64,16>(&data[0]);

  TTS_WHEN("A 64B allocator is setup")
  {
    TTS_AND_THEN( "a 1_B allocation is performed once" )
    {
      auto b = a.allocate(1_B);
      TTS_EQUAL     ( a.capacity(), 48_B    );
      TTS_NOT_EQUAL ( b.data      , nullptr );
      TTS_EQUAL     ( b.length    , 16_B    );
    }

    TTS_AND_THEN( "a 1_B allocation is performed twice" )
    {
      auto b = a.allocate(1_B);
      TTS_EQUAL     ( a.capacity(), 32_B);
      TTS_NOT_EQUAL ( b.data      , nullptr );
      TTS_EQUAL     ( b.length    , 16_B    );
    }

    TTS_AND_THEN( "a 1_B allocation is performed thrice" )
    {
      auto b = a.allocate(1_B);
      TTS_EQUAL     ( a.capacity(), 16_B);
      TTS_NOT_EQUAL ( b.data      , nullptr );
      TTS_EQUAL     ( b.length    , 16_B    );
    }

    TTS_AND_THEN( "a 1_B allocation is performed four times" )
    {
      auto b = a.allocate(1_B);
      TTS_EQUAL     ( a.capacity(), 0_B     );
      TTS_NOT_EQUAL ( b.data      , nullptr );
      TTS_EQUAL     ( b.length    , 16_B    );
    }

    TTS_AND_THEN( "a 1_B allocation is performed five times" )
    {
      auto b = a.allocate(1_B);
      TTS_EQUAL ( a.capacity(), 0_B     );
      TTS_EQUAL ( b.data      , nullptr );
      TTS_EQUAL ( b.length    , 0_B     );
    }

    TTS_AND_THEN( "everything is released" )
    {
      a.reset();
      TTS_EQUAL ( a.capacity(), 64_B);
    }
  }
}