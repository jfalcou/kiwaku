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
// Boundary-condition edge cases : degenerate (size-0 / size-1) dimensions must not hit
// modulo-by-zero and must fold every index onto the (single, if any) valid element.
//
//   tile  : tp % sp before the sp==0 guard was UB on a zero-extent dim.       -> guarded.
//   mirror: period = 2*(sp-1), tp % period before the sp<=1 guard was UB for   -> guarded.
//           sp==1 (period 0); a sp==0 dim also took the wrong fold path.
//==================================================================================================
TTS_CASE("Boundaries : mirror on a single-element (size-1) dimension folds to the one element")
{
  using namespace kwk::literals;

  float ref[ 1 ] = {42.f};

  // size-1 mirror : every signed index projects to index 0 (the only element).
  auto  vm       = kwk::view(kwk::source = ref, kwk::shape {1_c}, kwk::mirror);
  TTS_EQUAL(vm[ 0 ], 42.f);
  TTS_EQUAL(vm[ 3 ], 42.f);
  TTS_EQUAL(vm[ -1 ], 42.f);
};

TTS_CASE("Boundaries : tile on a single-element (size-1) dimension folds to the one element")
{
  using namespace kwk::literals;

  float ref[ 1 ] = {42.f};

  auto  vt       = kwk::view(kwk::source = ref, kwk::shape {1_c}, kwk::tile);
  TTS_EQUAL(vt[ 0 ], 42.f);
  TTS_EQUAL(vt[ 2 ], 42.f);
  TTS_EQUAL(vt[ -4 ], 42.f);
};

TTS_CASE("Boundaries : no modulo-by-zero on a zero-extent (size-0) dimension")
{
  using namespace kwk::literals;

  // A zero-extent dim has no storage to read, so assert the *boundary mapping* directly: the
  // tile/mirror functors must return 0 without evaluating a division by zero (compile- and run-time
  // safe). This locks the guard added before the modulo.
  kwk::shape sh {0_c};

  // tile(3, shape{0}) -> 0 (nothing to fold onto).
  float      guard = static_cast<float>(kumi::get<0>(kwk::__::tile_t {}(kumi::tuple {3.0}, sh)));
  TTS_EQUAL(guard, 0.f);

  // mirror(-2, shape{0}) -> 0 (empty dim maps to the sentinel 0, not a period fold).
  float g2 = static_cast<float>(kumi::get<0>(kwk::__::mirror_t {}(kumi::tuple {-2.0}, sh)));
  TTS_EQUAL(g2, 0.f);
};
