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
// A `kwk::table(kwk::generator = f, ...)` never owns memory : `f` IS the sink of the accessor
// pipeline, taking precedence over the scheme and consuming whatever the previous stage returns.
// With the canonical resolver that is the flat element index (there is no coordinate round-trip) ;
// interpolators drive `f` with each corner's index and blend. The builder derives the shape from
// the generator's own option (its single dynamic dimension) when no explicit `kwk::shape` is given.
//==================================================================================================

// A generator that reports the flat index it is handed (the canonical resolver's output).
constexpr auto flat_index = [](auto n) { return static_cast<int>(n); };

// A constant generator : ignores its input and yields 1.0.
constexpr auto one    = [](auto...) { return 1.0; };

TTS_CASE("J6 - generator table derives its shape from the generator option")
{
  // No explicit shape : the documented single dynamic dimension from `shape_of(generator)` is used.
  auto t = kwk::table(kwk::generator = flat_index);
  TTS_EQUAL(t->ndim, 1);
  TTS_EQUAL(t->size(), 0); // wildcard dimension : extent unknown until given explicitly
};

TTS_CASE("J6 - 1D generator table reads through scalar access")
{
  auto t = kwk::table(kwk::generator = flat_index, kwk::shape {4_c});
  TTS_EQUAL(t->ndim, 1);
  TTS_EQUAL(t->size(), 4);

  // The generator is the sink, receiving the flat index from the canonical resolver.
  TTS_EQUAL(t[ 0 ], 0);
  TTS_EQUAL(t[ 1 ], 1);
  TTS_EQUAL(t[ 2 ], 2);
  TTS_EQUAL(t[ 3 ], 3);
};

TTS_CASE("J6 - 2D generator table reads the flat index handed by canonical")
{
  auto t = kwk::table(kwk::generator = flat_index, kwk::shape {3_c, 4_c});
  TTS_EQUAL(t->ndim, 2);
  TTS_EQUAL(t->size(), 12);

  // canonical flattens (i, j) over stride{4,1} -> N = i*4 + j; the generator sees N directly
  // (no reconstruction of the coordinate pair).
  TTS_EQUAL(static_cast<int>(t[ 0, 0 ]), 0);
  TTS_EQUAL(static_cast<int>(t[ 0, 3 ]), 3);
  TTS_EQUAL(static_cast<int>(t[ 1, 0 ]), 4);
  TTS_EQUAL(static_cast<int>(t[ 2, 2 ]), 10);
  TTS_EQUAL(static_cast<int>(t[ 2, 3 ]), 11);
};

TTS_CASE("J6 - generator table interpolates (linear) through the function source")
{
  auto t = kwk::table(kwk::generator = flat_index, kwk::shape {5_c}, kwk::linear);
  TTS_EXPECT(decltype(t)::impl_type::accessor_type::interpolates);

  // Linear corner blending over the flat ramp f(n) = n : reading 2.5 merges f(2) and f(3).
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
