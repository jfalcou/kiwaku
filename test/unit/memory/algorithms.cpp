//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/kiwaku.hpp>
#include <algorithm>

// A non-implicit-lifetime type with a user-provided ctor/dtor so initialize/destroy are observable.
struct tracks
{
  static inline int alive = 0;
  int payload;
  tracks() : payload{0} { ++alive; }
  tracks(tracks const& o) : payload{o.payload} { ++alive; }
  tracks(tracks&& o) : payload{o.payload} { ++alive; }
  tracks& operator=(tracks const& o) { payload = o.payload; return *this; }
  ~tracks() { --alive; }
};

static void run_lifetime_test()
{
  tracks arr[4];
  TTS_EQUAL(tracks::alive, 4);   // the array constructed 4 live objects

  auto v = kwk::view(kwk::source = arr, kwk::shape{4});
  static_assert( std::same_as<typename decltype(v)::value_type, tracks> );

  kwk::destroy(v);               // destroys the 4 live objects
  TTS_EQUAL(tracks::alive, 0);

  kwk::initialize(v);            // re-constructs them into the (now dead) storage
  TTS_EQUAL(tracks::alive, 4);
}

TTS_CASE("memory.copy - element-wise copy across backed views")
{
  float src[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  float dst[8] = {};

  auto vs = kwk::view(kwk::source = src, kwk::shape{8});
  auto vd = kwk::view(kwk::source = dst, kwk::shape{8});

  kwk::copy(vd, vs);

  for (std::ptrdiff_t i = 0; i < 8; ++i) TTS_EQUAL(dst[i], src[i]);
};

TTS_CASE("memory.copy - multidimensional shape")
{
  float src[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  float dst[12] = {};

  auto vs = kwk::view(kwk::source = src, kwk::shape{3, 4});
  auto vd = kwk::view(kwk::source = dst, kwk::shape{3, 4});

  kwk::copy(vd, vs);

  for (std::ptrdiff_t i = 0; i < 12; ++i) TTS_EQUAL(dst[i], src[i]);
};

TTS_CASE("memory.fill - every cell holds the value")
{
  float buf[12] = {};
  auto v = kwk::view(kwk::source = buf, kwk::shape{3, 4});

  kwk::fill(v, 2.5f);

  for (std::ptrdiff_t i = 0; i < 12; ++i) TTS_EQUAL(buf[i], 2.5f);
};

TTS_CASE("memory.destroy / initialize - lifetime is balanced over raw storage")
{
  // Deduction from a non-implicit-lifetime element type (tracks) resolves through a consteval
  // path in the builder; keep that logic in a helper so the TTS lambda stays a plain callable.
  run_lifetime_test();
};
