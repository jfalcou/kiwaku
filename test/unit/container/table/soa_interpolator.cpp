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
#include <array>
#include <cmath>

//==================================================================================================
// H3 : interpolator <-> SoA integration.
//
// The per-field views produced by `get<I>` on a product-kind (SoA) table inherit the parent's
// interpolating accessor, so linear/bilinear blending works on each field exactly as it does on a
// plain scalar table. This ties the two finalized subsystems together: structure-of-arrays
// construction/population (H1/H1b) and the bidirectional accessor/interpolator pipeline.
//==================================================================================================
TTS_CASE("SoA linear interpolation : per-field 1D views blend neighbouring samples")
{
  using namespace kwk::literals;
  using K = kumi::tuple<float, double>;

  // Field 0 is a linear ramp y=10*x ; field 1 is the parabola y=x^2.
  std::array<float, 4>  xs = { 0.f, 10.f, 20.f, 30.f};
  std::array<double, 4> ys = { 0., 1., 4., 9.};

  // An owning product-kind (SoA) table whose interpolating accessor is shared by its field views.
  auto t = kwk::table(kwk::source = kumi::tuple{xs.data(), ys.data()}
                    , kwk::shape{4_c}
                    , kwk::as<K>()
                    , kwk::linear);

  auto fx = kwk::get<0>(t);
  auto fy = kwk::get<1>(t);

  // The field views carry the parent's linear interpolator.
  TTS_EXPECT(decltype(fx)::impl_type::accessor_type::interpolates);
  TTS_EXPECT(decltype(fy)::impl_type::accessor_type::interpolates);
  auto approx = [](double a, double b) { return std::abs(a - b) < 1e-5; };

  // Exact at integer positions (the transposed fibers reproduce the source exactly).
  TTS_EQUAL(fx[0], 0.0);
  TTS_EQUAL(fx[3], 30.0);
  TTS_EQUAL(fy[0], 0.0);
  TTS_EQUAL(fy[3], 9.0);

  // Midpoints : (ramp[i] + ramp[i+1]) / 2 and (i^2 + (i+1)^2) / 2.
  TTS_EXPECT(approx(fx[0.5], 5.0));
  TTS_EXPECT(approx(fx[1.5], 15.0));
  TTS_EXPECT(approx(fy[0.5], 0.5));
  TTS_EXPECT(approx(fy[2.5], 6.5));

  // 3/4 of the way : ramp[1]*0.25 + ramp[2]*0.75 = 17.5.
  TTS_EXPECT(approx(fx[1.75], 17.5));
};

TTS_CASE("SoA bilinear interpolation : per-field 2D views blend four corners")
{
  using namespace kwk::literals;
  using K = kumi::tuple<float, double>;

  // Bilinear ramp z(x,y) = 10*x + y on a 4x4 grid, plus a second field w = 2*z.
  std::array<float, 16>  xg;
  std::array<double, 16> yg;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
    {
      xg[4 * i + j] = 10.f * static_cast<float>(i) + static_cast<float>(j);
      yg[4 * i + j] = 2.0 * (10.0 * static_cast<double>(i) + static_cast<double>(j));
    }

  auto t = kwk::table(kwk::source = kumi::tuple{xg.data(), yg.data()}
                    , kwk::shape{4_c, 4_c}
                    , kwk::as<K>()
                    , kwk::linear);

  auto fx = kwk::get<0>(t);
  auto fy = kwk::get<1>(t);

  TTS_EQUAL(fx.ndim, 2);
  TTS_EQUAL(fy.ndim, 2);

  auto approx = [](double a, double b) { return std::abs(a - b) < 1e-5; };

  // Integer corners are exact on both fields.
  TTS_EQUAL((fx[2, 1]), 21.0);
  TTS_EQUAL((fy[2, 1]), 42.0);

  // Halfway : z(2.5, 1.5) = 26.5 and w = 53.
  TTS_EXPECT(approx(fx[2.5, 1.5], 26.5));
  TTS_EXPECT(approx(fy[2.5, 1.5], 53.0));

  // Quarter : z(0.25, 2.5) = 5.0 and w = 10.
  TTS_EXPECT(approx(fx[0.25, 2.5], 5.0));
  TTS_EXPECT(approx(fy[0.25, 2.5], 10.0));
};

TTS_CASE("SoA interpolation : the owning table reads its own captured fibers, not the source")
{
  using namespace kwk::literals;
  using K = kumi::tuple<float, double>;

  std::array<float, 4>  xs = {0.f, 10.f, 20.f, 30.f};
  std::array<double, 4> ys = {0., 1., 4., 9.};

  auto t = kwk::table(kwk::source = kumi::tuple{xs.data(), ys.data()}
                    , kwk::shape{4_c}
                    , kwk::as<K>()
                    , kwk::linear);

  auto fx = kwk::get<0>(t);

  // Mutating the source arrays after construction does not disturb the owning table: interpolation
  // reads the transposed fibers captured at build time, proving the SoA region is genuinely owned.
  xs[1] = 1000.f;

  auto approx = [](double a, double b) { return std::abs(a - b) < 1e-5; };
  TTS_EQUAL(fx[1], 10.0);            // the owned fiber still holds the build-time value
  TTS_EXPECT(approx(fx[1.5], 15.0)); // blend of the owned (10, 20), not the mutated source
};
