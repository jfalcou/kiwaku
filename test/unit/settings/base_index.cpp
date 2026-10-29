//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/kiwaku.hpp>

TTS_CASE("base_index : c_index is the canonical 0-based identity")
{
  using namespace kwk::literals;

  float ref[ 5 ] = {10.f, 20.f, 30.f, 40.f, 50.f};

  auto  v        = kwk::view(kwk::source = ref, kwk::shape {5_c});
  auto  vc       = kwk::view(kwk::source = ref, kwk::shape {5_c}, kwk::c_index);

  // A 0-based base index collapses to the canonical origin: the two views address identical bytes
  // with identical types (the coordinate wrapper is elided when the base is 0).
  TTS_TYPE_IS(decltype(v), decltype(vc));
  for(int i = 0; i < 5; ++i) TTS_EQUAL(vc[ i ], v[ i ]);
  TTS_EQUAL(vc[ 0 ], 10.f);
  TTS_EQUAL(vc[ 4 ], 50.f);
};

TTS_CASE("base_index : fortran_index shifts the origin by one (1D)")
{
  using namespace kwk::literals;

  float ref[ 5 ] = {10.f, 20.f, 30.f, 40.f, 50.f};

  // Index 1 ... 5 address elements 0 ... 4 under a 1-based origin.
  auto  vf       = kwk::view(kwk::source = ref, kwk::shape {5_c}, kwk::fortran_index);
  auto  ref0     = kwk::view(kwk::source = ref, kwk::shape {5_c});

  TTS_EQUAL(vf[ 1 ], 10.f);
  TTS_EQUAL(vf[ 2 ], 20.f);
  TTS_EQUAL(vf[ 5 ], 50.f);

  // The stored byte offsets are unchanged: the base lives purely in the addressing coordinate
  // stage, so a 1-based view is the same size as its 0-based sibling.
  TTS_EQUAL(sizeof(vf), sizeof(ref0));
};

TTS_CASE("base_index : fortran_index shifts the origin along every dimension (2D)")
{
  using namespace kwk::literals;

  float ref[ 6 ] = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f};

  // 2x3 row-major : (i,j) flat = i*3 + j.
  auto  v0       = kwk::view(kwk::source = ref, kwk::shape {2_c, 3_c});
  auto  vf       = kwk::view(kwk::source = ref, kwk::shape {2_c, 3_c}, kwk::fortran_index);

  // Under a 1-based origin, logical (i,j) maps to 0-based (i-1, j-1) -> flat (i-1)*3 + (j-1).
  auto  f11      = vf[ 1, 1 ];
  auto  f12      = vf[ 1, 2 ];
  auto  f23      = vf[ 2, 3 ];
  TTS_EQUAL(f11, 1.f);
  TTS_EQUAL(f12, 2.f);
  TTS_EQUAL(f23, 6.f);

  auto a00 = v0[ 0, 0 ];
  auto a12 = v0[ 1, 2 ];
  TTS_EQUAL(a00, 1.f);
  TTS_EQUAL(a12, 6.f);
};

TTS_CASE("base_index : a boundary clamps a below-base index before the resolver sees it")
{
  using namespace kwk::literals;

  float ref[ 5 ] = {10.f, 20.f, 30.f, 40.f, 50.f};

  // Under a 1-based origin the physical offset of logical index `i` is `i - 1`, so a below-base
  // request like `[0]` maps to a *negative* physical offset. That must be caught by the boundary
  // (boundaries clamp *before* the resolver), so the resolver never receives a negative offset —
  // the D4 invariant that keeps the resolver's signed-middle non-negativity assert from firing.
  auto  vfb      = kwk::view(kwk::source = ref, kwk::shape {5_c}, kwk::fortran_index, kwk::clamp);

  TTS_EQUAL(vfb[ 0 ], 10.f); // below-base clamps to physical 0
  TTS_EQUAL(vfb[ 1 ], 10.f);
  TTS_EQUAL(vfb[ 5 ], 50.f); // top clamps to physical 4
  TTS_EQUAL(vfb[ 6 ], 50.f);
};
