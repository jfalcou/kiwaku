//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/kiwaku.hpp>
#include <kumi/tuple.hpp>
#include <cmath>

using namespace kwk::literals;

//==================================================================================================
// J6 : generator-backed tables (function-defined spaces, no storage).
//
// A `kwk::table(kwk::generator = f, ...)` never owns memory : every element is evaluated on demand
// by calling `f` with the reconstructed grid coordinates. The builder derives the shape from the
// generator's own option (its single dynamic dimension) when no explicit `kwk::shape` is given, and
// the access/interpolate path must be able to read through the function source exactly as it does
// through memory.
//==================================================================================================

// A position-dependent generator : value at a flat position is the sum of its coordinates.
constexpr auto sum_of = [](auto... xs) { return (xs + ...); };

// A constant generator : ignores coordinates and yields 1.0.
constexpr auto one    = [](auto...) { return 1.0; };

TTS_CASE("J6 - generator table derives its shape from the generator option")
{
  // No explicit shape : the documented single dynamic dimension from `shape_of(generator)` is used.
  auto t = kwk::table(kwk::generator = sum_of);
  TTS_EQUAL(t->ndim, 1);
  TTS_EQUAL(t->size(), 0); // wildcard dimension : extent unknown until given explicitly
};

TTS_CASE("J6 - 1D generator table reads through scalar access")
{
  auto t = kwk::table(kwk::generator = sum_of, kwk::shape {4_c});
  TTS_EQUAL(t->ndim, 1);
  TTS_EQUAL(t->size(), 4);

  // row-major flat index == the single coordinate.
  TTS_EQUAL(static_cast<int>(t[ 0 ]), 0);
  TTS_EQUAL(static_cast<int>(t[ 1 ]), 1);
  TTS_EQUAL(static_cast<int>(t[ 2 ]), 2);
  TTS_EQUAL(static_cast<int>(t[ 3 ]), 3);
};

TTS_CASE("J6 - 2D generator table unflattens into grid coordinates")
{
  auto t = kwk::table(kwk::generator = sum_of, kwk::shape {3_c, 4_c});
  TTS_EQUAL(t->ndim, 2);
  TTS_EQUAL(t->size(), 12);

  // row-major over 3x4 : flat index = i*4 + j  =>  coordinate = (i, j)  =>  value = i + j.
  TTS_EQUAL(static_cast<int>(t[ 0, 0 ]), 0);
  TTS_EQUAL(static_cast<int>(t[ 0, 3 ]), 3);
  TTS_EQUAL(static_cast<int>(t[ 1, 0 ]), 1);
  TTS_EQUAL(static_cast<int>(t[ 2, 2 ]), 4);
  TTS_EQUAL(static_cast<int>(t[ 2, 3 ]), 5);
};

TTS_CASE("J6 - generator table interpolates (linear) through the function source")
{
  auto t = kwk::table(kwk::generator = sum_of, kwk::shape {5_c}, kwk::linear);
  TTS_EXPECT(decltype(t)::impl_type::accessor_type::interpolates);

  // Linear corner blending over the coordinate ramp : f(x) = x, so reading 2.5 merges f(2) and
  // f(3).
  TTS_EQUAL(t[ 2.5 ], 2.5);
};

TTS_CASE("J6 - generator tables never allocate storage")
{
  // A constant generator ignoring coordinates : any position yields 1.0.
  auto t = kwk::table(kwk::generator = one, kwk::shape {4_c});
  TTS_EQUAL(sizeof(t), 1u); // the constant_blob carries no data (a non-type template parameter)
  TTS_EQUAL(t[ 0 ], 1.0);
  TTS_EQUAL(t[ 3 ], 1.0);
};
