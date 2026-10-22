//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/kiwaku.hpp>
#include <kumi/tuple.hpp>
#include <cstddef>

using namespace kwk::literals;

//==================================================================================================
// J5 : keep-and-test the maintained shape utilities `equivalent`, `squeeze` and `operator""_D`.
//
// These were previously flagged as potentially dead API. They are kept (legitimate public surface)
// and pinned here with explicit assertions so they cannot silently drift dead or change semantics:
//   - `equivalent(a, b)`  : true iff a and b describe the same data space once trivial (1-sized)
//                           dimensions are removed — a shape-torsion invariant.
//   - `squeeze(s)`        : the shape with every trivial dimension removed.
//   - `N_D`               : the N-dimensional wildcard (fully dynamic) shape literal.
//==================================================================================================

TTS_CASE("J5 - squeeze removes trivial dimensions")
{
  using kwk::shape;

  // Interior, leading and trailing ones all collapse.
  TTS_EQUAL(shape{3},          kwk::squeeze(shape{1_c, 3}));
  TTS_EQUAL(shape{3},          kwk::squeeze(shape{3, 1_c}));
  TTS_EQUAL((shape{3, 4}),     kwk::squeeze(shape{1_c, 3, 1_c, 4}));
  TTS_EQUAL((shape{3, 4}),     kwk::squeeze(shape{3, 1_c, 4}));
  TTS_EQUAL(shape{},           kwk::squeeze(shape{1_c}));          // all-trivial collapses to empty
  TTS_EQUAL((shape{3, 5, 7}),  kwk::squeeze(shape{3, 5, 7}));      // no trivial dims : unchanged
};

TTS_CASE("J5 - equivalent detects the same data space across trivial dimensions")
{
  using kwk::shape;

  // Same space expressed with different numbers of trivial dimensions.
  TTS_EXPECT(kwk::equivalent(shape{3},       shape{1_c, 3}));
  TTS_EXPECT(kwk::equivalent(shape{3},       shape{1_c, 1_c, 3}));
  TTS_EXPECT(kwk::equivalent(shape{3, 4},    shape{1_c, 3, 4}));
  TTS_EXPECT(kwk::equivalent(shape{3, 4},    shape{3, 1_c, 4}));
  TTS_EXPECT(kwk::equivalent(shape{3, 1_c,4}, shape{3, 4}));

  // Order is a torsion : a different ordering is a different space.
  TTS_EXPECT_NOT(kwk::equivalent(shape{3, 5}, shape{5, 3}));

  // A genuinely different extent is not equivalent, even with the same rank and product.
  TTS_EXPECT_NOT(kwk::equivalent(shape{2, 6}, shape{3, 4}));
  TTS_EXPECT_NOT(kwk::equivalent(shape{15},   shape{3, 5}));

  // Trivial-only and empty are equivalent (the null space is one shape).
  TTS_EXPECT(kwk::equivalent(shape{1_c}, shape{}));
};

TTS_CASE("J5 - N_D builds the n-dimensional wildcard shape")
{

  TTS_EQUAL(decltype(0_D)::ndim, 0);
  TTS_EQUAL(decltype(1_D)::ndim, 1);
  TTS_EQUAL(decltype(2_D)::ndim, 2);
  TTS_EQUAL(decltype(4_D)::ndim, 4);
  TTS_EQUAL(decltype(12_D)::ndim, 12);

  // The literal selects the rank at compile time : a 2-D wildcard space matches the shape{_,_}.
  TTS_EQUAL(decltype(2_D)::ndim, decltype(kwk::shape{kwk::_, kwk::_})::ndim);

  // Wildcard (dynamic) dimensions carry no fixed extent.
  TTS_EQUAL((4_D).size(), 0);

  // A dynamic wildcard space is equivalent to itself after squeezing (no trivial dims to lose).
  TTS_EXPECT(kwk::equivalent(4_D, 4_D));
  TTS_EXPECT(kwk::equivalent(1_D, 1_D));
};
