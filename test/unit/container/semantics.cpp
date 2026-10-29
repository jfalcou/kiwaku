//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
// Semantics : one end-to-end pass over every construction/access semantics in one file. Each case
// is deliberately shallow — it proves the full object composes and reads back correctly, as a
// tripwire for the accessor refactor ("fibre = chain of plain function composition"):
//
//   plain, boundary, interpolated, generator, generator+interpolator, base index, SoA/product,
//   const access.
//
//==================================================================================================
#include "test.hpp"
#include <kiwaku/kiwaku.hpp>
#include <kumi/tuple.hpp>
#include <array>

using namespace kwk::literals;

// A ramp generator reporting the flat index it is handed.
constexpr auto ramp = [](auto n) { return static_cast<float>(n); };

TTS_CASE("SEMANTICS - plain owning table")
{
  std::array<float, 4> ref = {1.f, 2.f, 3.f, 4.f};
  auto                 t   = kwk::table(kwk::source = ref, kwk::shape {4_c});

  TTS_EQUAL(t->size(), 4);
  TTS_EQUAL(t[ 0 ], 1.f);
  TTS_EQUAL(t[ 1 ], 2.f);
  TTS_EQUAL(t[ 3 ], 4.f);
};

TTS_CASE("SEMANTICS - boundary (tile) clamps reads into range")
{
  std::array<float, 4> ref = {1.f, 2.f, 3.f, 4.f};
  auto                 t   = kwk::table(kwk::source = ref, kwk::shape {4_c}, kwk::tile);

  TTS_EQUAL(t[ 0 ], 1.f);
  TTS_EQUAL(t[ 4 ], 1.f);  // 4 -> 0
  TTS_EQUAL(t[ -1 ], 4.f); // -1 -> 3
};

TTS_CASE("SEMANTICS - interpolated (linear) fused read")
{
  std::array<float, 5> ref = {0.f, 1.f, 2.f, 3.f, 4.f};
  auto                 t   = kwk::table(kwk::source = ref, kwk::shape {5_c}, kwk::linear);

  TTS_EQUAL(t[ 2.5f ], 2.5f);
};

TTS_CASE("SEMANTICS - generator-backed table (no storage)")
{
  auto t = kwk::table(kwk::generator = ramp, kwk::shape {4_c});

  TTS_EQUAL(sizeof(t), 1u); // the function source is the accessor's terminal stage, no memory
  TTS_EQUAL(t[ 0 ], 0.f);
  TTS_EQUAL(t[ 2 ], 2.f);
  TTS_EQUAL(t[ 3 ], 3.f);
};

TTS_CASE("SEMANTICS - generator + interpolator compose in the accessor")
{
  auto t = kwk::table(kwk::generator = ramp, kwk::shape {5_c}, kwk::linear);

  // f(n) = n over the flat ramp : linear at 2.5 merges f(2) and f(3).
  TTS_EQUAL(t[ 2.5f ], 2.5f);
};

TTS_CASE("SEMANTICS - base index (fortran) folds into the offset")
{
  std::array<float, 6> ref = {0.f, 1.f, 2.f, 3.f, 4.f, 5.f};
  auto t = kwk::table(kwk::source = ref, kwk::of_shape = kwk::shape {2_c, 3_c}, kwk::fortran_index);

  // 1-based : logical (1,1) -> physical (0,0) -> ref[0]. The extra parens keep the comma inside a
  // single macro argument (multi-index access).
  TTS_EQUAL((t[ 1, 1 ]), 0.f);
  TTS_EQUAL((t[ 2, 3 ]), 5.f);
};

TTS_CASE("SEMANTICS - SoA / product kind, per-field access")
{
  using K                  = kumi::tuple<float, double>;
  std::array<float, 4>  xs = {1.f, 2.f, 3.f, 4.f};
  std::array<double, 4> ys = {10., 20., 30., 40.};

  auto                  v =
      kwk::table(kwk::source = kumi::tuple {xs.data(), ys.data()}, kwk::shape {4_c}, kwk::as<K>());

  auto fx = kwk::get<0>(v);
  auto fy = kwk::get<1>(v);
  TTS_EQUAL(fx->kind(), (kwk::as<float>()));
  TTS_EQUAL(fy->kind(), (kwk::as<double>()));
  TTS_EQUAL(fx[ 2 ], 3.f);
  TTS_EQUAL(fy[ 3 ], 40.);
};

TTS_CASE("SEMANTICS - const access")
{
  std::array<float, 4> const ref = {1.f, 2.f, 3.f, 4.f};
  auto                       t   = kwk::table(kwk::source = ref, kwk::shape {4_c});

  TTS_EQUAL(t[ 0 ], 1.f);
  TTS_EQUAL(t[ 3 ], 4.f);
};
