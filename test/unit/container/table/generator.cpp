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
constexpr auto one        = [](auto...) { return 1.0; };

// A generator yielding `float` : used to pin the collection's value_type to the generator's return.
constexpr auto ramp_float = [](auto n) { return static_cast<float>(n); };

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

// Metafunction to fetch the terminal (last piped) stage of an accessor's chain. S4 injects the
// generator as the accessor's *final* stage, so the last element of the pack must be the generator
// terminal — that is what lets `resolve` be a single uniform read with no `constant_blob` special
// case (the accessor itself terminates at the generator).
template<typename... S> struct last_of;
template<typename S> struct last_of<S>
{
  using type = S;
};
template<typename S0, typename S1, typename... Rest> struct last_of<S0, S1, Rest...>
{
  using type = typename last_of<S1, Rest...>::type;
};

template<typename A> struct accessor_last;
template<typename... S> struct accessor_last<kwk::accessor_t<S...>>
{
  using type = typename last_of<S...>::type;
};

TTS_CASE("J6 - the generator is injected as the accessor's terminal (final) stage")
{
  auto t      = kwk::table(kwk::generator = flat_index, kwk::shape {4_c});

  // The fibre ends at the generator : the last piped stage is a generator terminal, not
  // `canonical`.
  using acc_t = decltype(t)::impl_type::accessor_type;
  using gen_t = typename accessor_last<acc_t>::type;
  static_assert(kwk::__::is_generator_v<gen_t>);
  TTS_EXPECT(kwk::__::is_generator_v<gen_t>);
};

TTS_CASE("J6 - the collection's value_type is the generator's own return type")
{
  // The generator's return type drives the kind, so a table's element type matches what `f`
  // yields — not a `real64` default. A generator returning `float` gives a `float` table.
  auto t = kwk::table(kwk::generator = ramp_float, kwk::shape {4_c});
  TTS_TYPE_IS(typename decltype(t)::value_type, float);
  TTS_TYPE_IS(decltype(t)::impl_type::kind_t, float);

  // An integral generator gives an integral element type (this is the case that previously fell
  // back to `double`).
  auto i = kwk::table(kwk::generator = flat_index, kwk::shape {4_c});
  TTS_TYPE_IS(typename decltype(i)::value_type, int);
};

TTS_CASE("J6 - a boundary composes with the generator through the accessor chain")
{
  // `tile` + generator : out-of-range logical indices are wrapped to physical ones first, and only
  // then does the generator terminal read the flattened (physical) index — proving the generator
  // lives at the read end of the same staged pipeline as the boundary.
  auto t = kwk::table(kwk::generator = flat_index, kwk::shape {4_c}, kwk::tile);

  TTS_EQUAL(t[ 0 ], 0);
  TTS_EQUAL(t[ 3 ], 3);
  TTS_EQUAL(t[ 4 ], 0);  // tile : 4 -> 0, generator sees the physical 0
  TTS_EQUAL(t[ -1 ], 3); // tile : -1 -> 3, generator sees the physical 3
};
