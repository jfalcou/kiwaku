//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/kiwaku.hpp>

//==================================================================================================
// Part B : the accessor resolve path is bidirectional
//
//   solve layout -> accessor_down(indices, shape, stride) -> read(storage, index)
//                   -> accessor_up(raw value(s))          -> final value
//
// `none` and `nearest` are pure selections (one storage element read); `linear` reads all 2^N
// surrounding corners and blends them. Both must reproduce exact values at integer positions.
//==================================================================================================
TTS_CASE("Accessor pipeline : selection (none) is bidirectional identity")
{
  using namespace kwk::literals;

  float ref[ 5 ] = {10.f, 20.f, 30.f, 40.f, 50.f};

  auto  v        = kwk::view(kwk::source = ref, kwk::shape {5_c});

  // down(3, shape, stride) -> flat index 3 ; read(index 3) -> ref[3] ; up == identity.
  TTS_EQUAL(v[ 3 ], ref[ 3 ]);
  TTS_EQUAL(v[ 0 ], ref[ 0 ]);
  TTS_EQUAL(v[ 4 ], ref[ 4 ]);
};

TTS_CASE("Accessor pipeline : nearest interpolator samples the closest grid point")
{
  using namespace kwk::literals;

  float ref[ 5 ] = {10.f, 20.f, 30.f, 40.f, 50.f};

  auto  v        = kwk::view(kwk::source = ref, kwk::shape {5_c});
  auto  vn       = kwk::view(kwk::source = ref, kwk::shape {5_c}, kwk::nearest);

  TTS_TYPE_IS(typename decltype(vn)::source_type, typename decltype(v)::source_type);

  // Exact grid points must round-trip exactly.
  for(int i = 0; i < 5; ++i) TTS_EQUAL(vn[ i ], v[ i ]);
};

TTS_CASE("Accessor pipeline : linear interpolator blends neighbouring samples (1D)")
{
  using namespace kwk::literals;

  // A linear ramp y(x) = 10*x : exact linear interpolation at any position.
  float ramp[ 5 ] = {0.f, 10.f, 20.f, 30.f, 40.f};

  auto  vl        = kwk::view(kwk::source = ramp, kwk::shape {5_c}, kwk::linear);

  // Exact values at integer positions.
  TTS_EQUAL(vl[ 0 ], 0.0);
  TTS_EQUAL(vl[ 2 ], 20.0);
  TTS_EQUAL(vl[ 4 ], 40.0);

  // Midpoints : (ramp[i] + ramp[i+1]) / 2.
  auto approx_equal = [](double a, double b) { return std::abs(a - b) < 1e-5; };
  TTS_EXPECT(approx_equal(vl[ 0.5 ], 5.0));
  TTS_EXPECT(approx_equal(vl[ 1.5 ], 15.0));
  TTS_EXPECT(approx_equal(vl[ 3.5 ], 35.0));

  // 3/4 of the way : ramp[1]*(1-0.75) + ramp[2]*0.75.
  TTS_EXPECT(approx_equal(vl[ 1.75 ], 17.5));
};

TTS_CASE("Accessor pipeline : linear interpolator blends neighbouring samples (2D)")
{
  using namespace kwk::literals;

  // A bilinear ramp z(x,y) = 10*x + y on a 4x4 grid. The default stride is row-major (first dim
  // outermost), so element (i,j) lives at flat index 4*i + j.
  float grid[ 16 ];
  for(int i = 0; i < 4; ++i)
    for(int j = 0; j < 4; ++j)
      grid[ 4 * i + j ] = 10.f * static_cast<float>(i) + static_cast<float>(j);

  auto vl           = kwk::view(kwk::source = grid, kwk::shape {4_c, 4_c}, kwk::linear);

  auto approx_equal = [](double a, double b) { return std::abs(a - b) < 1e-5; };

  // Integer corners are exact.
  TTS_EQUAL((vl[ 2, 1 ]), 21.0);

  // Halfway : z(2.5, 1.5) = (10*2.5 + 1.5) = 26.5
  TTS_EXPECT(approx_equal(vl[ 2.5, 1.5 ], 26.5));

  // Quarter : z(0.25, 2.5) = 10*0.25 + 2.5 = 5.0  (both corners in-bounds)
  TTS_EXPECT(approx_equal(vl[ 0.25, 2.5 ], 5.0));
};

TTS_CASE("Accessor pipeline : boundaries feed the down/up pipeline")
{
  using namespace kwk::literals;

  float ref[ 4 ] = {0.f, 10.f, 20.f, 30.f};

  // clamp maps out-of-range indices into [0, 3] before the coordinate/read stage.
  auto  vc       = kwk::view(kwk::source = ref, kwk::shape {4_c}, kwk::clamp);
  TTS_EQUAL(vc[ 5 ], 30.f);
  TTS_EQUAL(vc[ -1 ], 0.f);

  // tile folds indices back into the domain.
  auto vt = kwk::view(kwk::source = ref, kwk::shape {4_c}, kwk::tile);
  TTS_EQUAL(vt[ 4 ], 0.f);
  TTS_EQUAL(vt[ 6 ], 20.f);

  // mirror reflects indices.
  auto vm = kwk::view(kwk::source = ref, kwk::shape {4_c}, kwk::mirror);
  TTS_EQUAL(vm[ 4 ], 20.f);
  TTS_EQUAL(vm[ 7 ], 10.f);
};

TTS_CASE("Accessor pipeline : floor/ceil/round are exact selection strategies")
{
  using namespace kwk::literals;

  // ref = {10*i}; each selection strategy samples one whole-index grid point.
  float ref[ 5 ] = {0.f, 10.f, 20.f, 30.f, 40.f};

  auto  vf       = kwk::view(kwk::source = ref, kwk::shape {5_c}, kwk::floor);
  auto  vc       = kwk::view(kwk::source = ref, kwk::shape {5_c}, kwk::ceil);
  auto  vr       = kwk::view(kwk::source = ref, kwk::shape {5_c}, kwk::round);

  // floor always picks the lattice point at or below the position.
  TTS_EQUAL(vf[ 2.7 ], 20.f); // floor(2.7) = 2
  TTS_EQUAL(vf[ 2.0 ], 20.f); // integer positions are exact
  TTS_EQUAL(vf[ 0.9 ], 0.f);  // floor(0.9) = 0

  // ceil always picks the lattice point at or above the position.
  TTS_EQUAL(vc[ 2.1 ], 30.f); // ceil(2.1) = 3
  TTS_EQUAL(vc[ 2.0 ], 20.f); // ceil(2.0) = 2 (exact)
  TTS_EQUAL(vc[ 0.1 ], 10.f); // ceil(0.1) = 1

  // round picks the nearest lattice point (half away from zero).
  TTS_EQUAL(vr[ 2.4 ], 20.f); // round(2.4) = 2
  TTS_EQUAL(vr[ 2.5 ], 30.f); // round(2.5) = 3
  TTS_EQUAL(vr[ 0.6 ], 10.f); // round(0.6) = 1
};
