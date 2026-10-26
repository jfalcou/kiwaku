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
// C9 : the accessor is a variadic pipeline (`accessor<Coordinate, Boundaries, Interpolators...>`)
// of stages, composed with `accessor | accessor` and with `fractional` / `interpolates` derived
// across the interpolator pack. Absent stages (no boundary / no interpolator) are trivial.
//==================================================================================================
TTS_CASE("Accessor pipeline : absent stages are trivial identities (2-stage)")
{
  using namespace kwk::literals;

  double ref[ 4 ] = {0., 10., 20., 30.};
  auto   read     = [ & ](std::size_t i) -> double { return ref[ i ]; };

  // `accessor{ canonical }` alone : a coordinate with no boundary and no interpolator.
  auto   acc      = kwk::accessor {kwk::canonical, kumi::unit {}};

  TTS_EXPECT(!decltype(acc)::interpolates);
  TTS_EXPECT(!decltype(acc)::fractional);

  // Flat index of position {2} over shape{4}, stride{1} -> ref[2].
  TTS_EQUAL(acc(kumi::tuple {2.0}, kwk::shape {4_c}, kwk::stride {1_c}, read), 20.0);
  TTS_EQUAL(acc(kumi::tuple {0.0}, kwk::shape {4_c}, kwk::stride {1_c}, read), 0.0);
};

TTS_CASE("Accessor pipeline : a selection stage engages only fractional, not interpolates")
{
  using namespace kwk::literals;

  double ref[ 5 ] = {0., 10., 20., 30., 40.};
  auto   read     = [ & ](std::size_t i) -> double { return ref[ i ]; };

  auto   acc      = kwk::accessor {kwk::canonical, kumi::unit {}, kwk::__::nearest {}};

  TTS_EXPECT(!decltype(acc)::interpolates);
  TTS_EXPECT(decltype(acc)::fractional);

  // nearest rounds the fractional position onto the closest lattice point, then flattens.
  TTS_EQUAL(acc(kumi::tuple {2.4}, kwk::shape {5_c}, kwk::stride {1_c}, read), 20.0);
  TTS_EQUAL(acc(kumi::tuple {2.7}, kwk::shape {5_c}, kwk::stride {1_c}, read), 30.0);
};

TTS_CASE("Accessor pipeline : an interpolating stage engages both fractional and interpolates")
{
  using namespace kwk::literals;

  double ramp[ 5 ] = {0., 10., 20., 30., 40.};
  auto   read      = [ & ](std::size_t i) -> double { return ramp[ i ]; };

  auto   acc       = kwk::accessor {kwk::canonical, kumi::unit {}, kwk::__::linear {}};

  TTS_EXPECT(decltype(acc)::interpolates);
  TTS_EXPECT(decltype(acc)::fractional);

  auto approx_equal = [](double a, double b) { return std::abs(a - b) < 1e-9; };

  // ramp is linear : interpolation reproduces the ramp exactly at any position.
  TTS_EXPECT(approx_equal(acc(kumi::tuple {0.5}, kwk::shape {5_c}, kwk::stride {1_c}, read), 5.0));
  TTS_EXPECT(approx_equal(acc(kumi::tuple {2.0}, kwk::shape {5_c}, kwk::stride {1_c}, read), 20.0));
  TTS_EXPECT(approx_equal(acc(kumi::tuple {3.5}, kwk::shape {5_c}, kwk::stride {1_c}, read), 35.0));
};

TTS_CASE("Accessor pipeline : variadic pack engages the FIRST interpolating stage")
{
  using namespace kwk::literals;

  double ref[ 4 ] = {0., 10., 20., 30.};
  auto   read     = [ & ](std::size_t i) -> double { return ref[ i ]; };

  // An inert `none` leads a variadic pack whose first interpolating stage is `linear`.
  auto   acc = kwk::accessor {kwk::canonical, kumi::unit {}, kwk::__::none {}, kwk::__::linear {}};

  TTS_EXPECT(decltype(acc)::interpolates);
  TTS_EXPECT(decltype(acc)::fractional);
  TTS_EQUAL(acc(kumi::tuple {1.0}, kwk::shape {4_c}, kwk::stride {1_c}, read), 10.0);
};

TTS_CASE("Accessor pipeline : `accessor | accessor` concatenates the stages")
{
  using namespace kwk::literals;

  double ramp[ 4 ] = {0., 10., 20., 30.};
  auto   read      = [ & ](std::size_t i) -> double { return ramp[ i ]; };

  // `base` is a plain selection pipeline; `interp` brings the linear interpolation stage.
  auto   base      = kwk::accessor {kwk::canonical, kumi::unit {}, kwk::__::none {}};
  auto   interp    = kwk::accessor {kwk::canonical, kumi::unit {}, kwk::__::linear {}};

  auto   comp      = base | interp;

  // The composite's first interpolating stage is `linear` : both flags come true.
  TTS_EXPECT(decltype(comp)::interpolates);
  TTS_EXPECT(decltype(comp)::fractional);

  auto approx_equal = [](double a, double b) { return std::abs(a - b) < 1e-9; };

  // Down/up across the concatenated pipeline still reproduce the linear ramp.
  TTS_EXPECT(
      approx_equal(comp(kumi::tuple {1.5}, kwk::shape {4_c}, kwk::stride {1_c}, read), 15.0));
  TTS_EXPECT(approx_equal(comp(kumi::tuple {0.0}, kwk::shape {4_c}, kwk::stride {1_c}, read), 0.0));
  TTS_EXPECT(
      approx_equal(comp(kumi::tuple {3.0}, kwk::shape {4_c}, kwk::stride {1_c}, read), 30.0));
};
