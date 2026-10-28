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
// The accessor is a fibre : a chain of plain function objects (`accessor_t{ canonical }` as the
// innermost resolver) extended by piping a boundary (clamp/tile/mirror) and/or an interpolator
// (none/nearest/floor/ceil/round/linear). Reading is `acc(position, shape, stride, read)`.
//
// Piping runs the chain backwards (continuation-passing) : the outermost piped stage runs first on
// the position — a boundary transforms it and passes it down, an interpolator calls the rest back
// once per sample and blends — and the resolver leaf maps the position to a storage offset which
// the `read` callback turns into a value. An absent stage is simply *not piped* (no identity).
//==================================================================================================
TTS_CASE("Accessor : a plain resolver with no additional stage")
{
  using namespace kwk::literals;

  double ref[ 4 ] = {0., 10., 20., 30.};
  auto   read     = [ & ](std::size_t i) -> double { return ref[ i ]; };

  auto   acc      = kwk::accessor {kwk::canonical};

  TTS_EXPECT(!decltype(acc)::interpolates);
  TTS_EXPECT(!decltype(acc)::fractional);

  // Flat index of position {2} over shape{4}, stride{1} -> ref[2].
  TTS_EQUAL(acc(kumi::tuple {2_c}, kwk::shape {4_c}, kwk::stride {1_c}, read), 20.0);
  TTS_EQUAL(acc(kumi::tuple {0_c}, kwk::shape {4_c}, kwk::stride {1_c}, read), 0.0);
  TTS_EQUAL(acc(kumi::tuple {3_c}, kwk::shape {4_c}, kwk::stride {1_c}, read), 30.0);
};

TTS_CASE("Accessor : a boundary is piped on and passes its transformed position down")
{
  using namespace kwk::literals;

  double ref[ 4 ] = {0., 10., 20., 30.};
  auto   read     = [ & ](std::size_t i) -> double { return ref[ i ]; };

  auto   acc      = kwk::accessor {kwk::canonical} | kwk::clamp;

  TTS_EXPECT(!decltype(acc)::interpolates);
  TTS_EXPECT(!decltype(acc)::fractional);

  // clamp projects out-of-range positions into [0, 3] before the resolver leaf.
  TTS_EQUAL(acc(kumi::tuple {-2_c}, kwk::shape {4_c}, kwk::stride {1_c}, read), 0.0);
  TTS_EQUAL(acc(kumi::tuple {1_c}, kwk::shape {4_c}, kwk::stride {1_c}, read), 10.0);
  TTS_EQUAL(acc(kumi::tuple {9_c}, kwk::shape {4_c}, kwk::stride {1_c}, read), 30.0);
};

TTS_CASE("Accessor : a selection stage engages only fractional, not interpolates")
{
  using namespace kwk::literals;

  double ref[ 5 ] = {0., 10., 20., 30., 40.};
  auto   read     = [ & ](std::size_t i) -> double { return ref[ i ]; };

  auto   acc      = kwk::accessor {kwk::canonical} | kwk::nearest;

  TTS_EXPECT(!decltype(acc)::interpolates);
  TTS_EXPECT(decltype(acc)::fractional);

  // nearest maps the fractional position onto the closest lattice point, then resolves it.
  TTS_EQUAL(acc(kumi::tuple {2.4}, kwk::shape {5_c}, kwk::stride {1_c}, read), 20.0);
  TTS_EQUAL(acc(kumi::tuple {2.7}, kwk::shape {5_c}, kwk::stride {1_c}, read), 30.0);
};

TTS_CASE("Accessor : an interpolating stage engages both fractional and interpolates")
{
  using namespace kwk::literals;

  double ramp[ 5 ] = {0., 10., 20., 30., 40.};
  auto   read      = [ & ](std::size_t i) -> double { return ramp[ i ]; };

  auto   acc       = kwk::accessor {kwk::canonical} | kwk::linear;

  TTS_EXPECT(decltype(acc)::interpolates);
  TTS_EXPECT(decltype(acc)::fractional);

  auto approx_equal = [](double a, double b) { return std::abs(a - b) < 1e-9; };

  // ramp is linear : interpolation reproduces the ramp exactly at any position.
  TTS_EXPECT(approx_equal(acc(kumi::tuple {0.5}, kwk::shape {5_c}, kwk::stride {1_c}, read), 5.0));
  TTS_EXPECT(approx_equal(acc(kumi::tuple {2.0}, kwk::shape {5_c}, kwk::stride {1_c}, read), 20.0));
  TTS_EXPECT(approx_equal(acc(kumi::tuple {3.5}, kwk::shape {5_c}, kwk::stride {1_c}, read), 35.0));
};

TTS_CASE("Accessor : boundary and interpolator compose by piping in order")
{
  using namespace kwk::literals;

  double ref[ 4 ] = {0., 10., 20., 30.};
  auto   read     = [ & ](std::size_t i) -> double { return ref[ i ]; };

  // The interpolator is the outermost stage : it calls the boundary back with each corner, and the
  // boundary passes the clamped corner down to the resolver leaf.
  auto   acc      = kwk::accessor {kwk::canonical} | kwk::clamp | kwk::linear;

  TTS_EXPECT(decltype(acc)::interpolates);
  TTS_EXPECT(decltype(acc)::fractional);

  auto approx_equal = [](double a, double b) { return std::abs(a - b) < 1e-9; };

  // In-bounds : the boundary is a no-op and linear blends the two neighbours.
  TTS_EXPECT(approx_equal(acc(kumi::tuple {1.5}, kwk::shape {4_c}, kwk::stride {1_c}, read), 15.0));
  // Out-of-bounds : the linear corners get clamped to [0, 3], so the read stays in-bounds.
  TTS_EXPECT(approx_equal(acc(kumi::tuple {-1.5}, kwk::shape {4_c}, kwk::stride {1_c}, read), 0.0));
  TTS_EXPECT(approx_equal(acc(kumi::tuple {9.5}, kwk::shape {4_c}, kwk::stride {1_c}, read), 30.0));
};

TTS_CASE("Accessor : a runtime label is a property of the fibre")
{
  using namespace kwk::literals;

  // The label is a (runtime) kumi::str held by the fibre; an empty name means "no name".
  auto acc = kwk::accessor {kwk::canonical};
  TTS_EQUAL(acc.name().size(), 0u);

  // A fibre can carry a runtime name via `named`; reading preserves the runtime name.
  auto named = kwk::accessor {kwk::canonical}.named("fibre_v");
  TTS_EQUAL(named.name(), kumi::str {"fibre_v"});
  TTS_EQUAL(named.name().size(), 7u);
};
