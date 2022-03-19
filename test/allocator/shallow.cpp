//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/allocator/shallow_allocator.hpp>

TTS_CASE( "Allocation/Deallocation through shallow_allocator" )
{
  std::byte data[16];
  auto a = kwk::make_shallow<16,8>(data);

  {
    auto memory = a.allocate(0);

    TTS_EQUAL     ( memory.data   , nullptr );
    TTS_EQUAL     ( memory.length , 0       );
    TTS_EXPECT_NOT( a.owns(memory)          );
    TTS_EQUAL     ( a.capacity()  , 16      );
  }

  {
    auto memory = a.allocate(1);

    TTS_NOT_EQUAL     ( memory.data   , nullptr );
    TTS_GREATER_EQUAL ( memory.length , 1       );
    TTS_EXPECT        ( a.owns(memory)          );
    TTS_EQUAL         ( a.capacity()  , 8       );
  }

  {
    auto memory = a.allocate(15);

    TTS_EQUAL     ( memory.data   , nullptr );
    TTS_EQUAL     ( memory.length , 0       );
    TTS_EXPECT_NOT( a.owns(memory)          );
    TTS_EQUAL     ( a.capacity()   , 8      );
  }

  {
    a.reset();
    auto memory = a.allocate(15);

    TTS_NOT_EQUAL     ( memory.data   , nullptr );
    TTS_GREATER_EQUAL ( memory.length , 15      );
    TTS_EXPECT        ( a.owns(memory)          );
    TTS_EQUAL         ( a.capacity()   , 0      );
  }

  {
    a.reset();
    auto memory = a.allocate(1000);

    TTS_EQUAL     ( memory.data   , nullptr );
    TTS_EQUAL     ( memory.length , 0       );
    TTS_EXPECT_NOT( a.owns(memory)          );
    TTS_EQUAL     ( a.capacity()   , 16     );
  }

  a.reset();
};

TTS_CASE( "Checks memory retrieval by shallow_allocator" )
{
  double data[8];
  auto a = kwk::make_shallow<64,16>(&data[0]);

  {
    auto b = a.allocate(1);
    TTS_EQUAL     ( a.capacity(), 48      );
    TTS_NOT_EQUAL ( b.data      , nullptr );
    TTS_EQUAL     ( b.length    , 16      );
  }

  {
    auto b = a.allocate(1);
    TTS_EQUAL     ( a.capacity(), 32      );
    TTS_NOT_EQUAL ( b.data      , nullptr );
    TTS_EQUAL     ( b.length    , 16      );
  }

  {
    auto b = a.allocate(1);
    TTS_EQUAL     ( a.capacity(), 16      );
    TTS_NOT_EQUAL ( b.data      , nullptr );
    TTS_EQUAL     ( b.length    , 16      );
  }

  {
    auto b = a.allocate(1);
    TTS_EQUAL     ( a.capacity(), 0       );
    TTS_NOT_EQUAL ( b.data      , nullptr );
    TTS_EQUAL     ( b.length    , 16      );
  }

  {
    auto b = a.allocate(1);
    TTS_EQUAL ( a.capacity(), 0       );
    TTS_EQUAL ( b.data      , nullptr );
    TTS_EQUAL ( b.length    , 0       );
  }

  {
    a.reset();
    TTS_EQUAL( a.capacity(), 64 );
  }
};
