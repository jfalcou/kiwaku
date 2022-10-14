//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

TTS_CASE("Check dynamic slicing for kwk::from")
{
  using namespace kwk::literals;
  using kwk::from;
  using kwk::fixed;

  int d0 = 2, d1 = 4, d2 = 5, d3 = 10;
  constexpr int v0 = 1, v1 = 2, v2 = 3, v3 = 5;

  auto f0 = from(v0); auto c0 = from(fixed<v0>);
  auto f1 = from(v1); auto c1 = from(fixed<v1>);
  auto f2 = from(v2); auto c2 = from(fixed<v2>);
  auto f3 = from(v3); auto c3 = from(fixed<v3>);

  auto shp = kwk::of_size(d0, d1, d2, d3);

  TTS_EQUAL(shp(f0)             , kwk::of_size(            d0*d1*d2*d3 - v0));
  TTS_EQUAL(shp(f0, f1)         , kwk::of_size(d0-v0,         d1*d2*d3 - v1));
  TTS_EQUAL(shp(f0, f1, f2)     , kwk::of_size(d0-v0, d1-v1,     d2*d3 - v2));
  TTS_EQUAL(shp(f0, f1, f2, f3) , kwk::of_size(d0-v0, d1-v1, d2-v2, d3 - v3));

  TTS_EQUAL(shp(c0)             , kwk::of_size(            d0*d1*d2*d3 - v0));
  TTS_EQUAL(shp(c0, c1)         , kwk::of_size(d0-v0,         d1*d2*d3 - v1));
  TTS_EQUAL(shp(c0, c1, c2)     , kwk::of_size(d0-v0, d1-v1,     d2*d3 - v2));
  TTS_EQUAL(shp(c0, c1, c2, c3) , kwk::of_size(d0-v0, d1-v1, d2-v2, d3 - v3));
};

TTS_CASE("Check static slicing for kwk::from")
{
  using namespace kwk::literals;
  using kwk::from;
  using kwk::fixed;

  auto d0 = 2_c;
  auto d1 = 4_c;
  auto d2 = 5_c;
  auto d3 = 10_c;

  constexpr int v0 = 1, v1 = 2, v2 = 3, v3 = 5;

  auto f0 = from(v0); auto c0 = from(fixed<v0>);
  auto f1 = from(v1); auto c1 = from(fixed<v1>);
  auto f2 = from(v2); auto c2 = from(fixed<v2>);
  auto f3 = from(v3); auto c3 = from(fixed<v3>);

  auto shp = kwk::of_size(d0, d1, d2, d3);

  // Shrink to 1D
  TTS_EQUAL(shp(f0)             , kwk::of_size(d0*d1*d2*d3 - v0));
  TTS_EQUAL(shp(c0)             , kwk::of_size(d0*d1*d2*d3 - v0));
  TTS_EQUAL(shp(c0).extent<0>() , static_cast<std::uint32_t>(d0*d1*d2*d3 - v0));
  TTS_EXPECT(shp(f0).is_fully_dynamic);
  TTS_EXPECT(shp(c0).is_fully_static);

  // Shrink to 2D
  TTS_EQUAL(shp(f0, f1)             , kwk::of_size(d0 - v0, d1*d2*d3 - v1));
  TTS_EQUAL(shp(c0, c1)             , kwk::of_size(d0 - v0, d1*d2*d3 - v1));
  TTS_EQUAL(shp(c0, c1).extent<0>() , static_cast<std::uint32_t>(d0 - v0));
  TTS_EQUAL(shp(c0, c1).extent<1>() , static_cast<std::uint32_t>(d1*d2*d3 - v1));
  TTS_EXPECT    (shp(c0, c1).is_fully_static);
  TTS_EXPECT_NOT(shp(c0, f1).is_fully_dynamic);
  TTS_EXPECT    (shp(f0, f1).is_fully_dynamic);

  // Shrink to 3D
  TTS_EQUAL(shp(f0, f1, f2)             , kwk::of_size(d0 - v0, d1 - v1, d2*d3 - v2));
  TTS_EQUAL(shp(c0, c1, c2)             , kwk::of_size(d0 - v0, d1 - v1, d2*d3 - v2));
  TTS_EQUAL(shp(c0, c1, c2).extent<0>() , static_cast<std::uint32_t>(d0 - v0));
  TTS_EQUAL(shp(c0, c1, c2).extent<1>() , static_cast<std::uint32_t>(d1 - v1));
  TTS_EQUAL(shp(c0, c1, c2).extent<2>() , static_cast<std::uint32_t>(d2*d3 - v2));
  TTS_EXPECT    (shp(c0, c1, c2).is_fully_static);
  TTS_EXPECT_NOT(shp(f0, f1, c2).is_fully_dynamic);
  TTS_EXPECT    (shp(f0, f1, f2).is_fully_dynamic);

  // Same for 4D (most likely redundant)
  TTS_EQUAL(shp(f0, f1, f2, f3)             , kwk::of_size(d0 - v0, d1 - v1, d2 - v2, d3 - v3));
  TTS_EQUAL(shp(c0, c1, c2, c3)             , kwk::of_size(d0 - v0, d1 - v1, d2 - v2, d3 - v3));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<0>() , static_cast<std::uint32_t>(d0 - v0));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<1>() , static_cast<std::uint32_t>(d1 - v1));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<2>() , static_cast<std::uint32_t>(d2 - v2));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<3>() , static_cast<std::uint32_t>(d3 - v3));
  TTS_EXPECT    (shp(c0, c1, c2, c3).is_fully_static);
  TTS_EXPECT_NOT(shp(f0, f1, c2, c3).is_fully_dynamic);
  TTS_EXPECT    (shp(f0, f1, f2, f3).is_fully_dynamic);
};

TTS_CASE("Check kwk::from with kwk::end")
{
  using namespace kwk::literals;
  using kwk::from;
  using kwk::fixed;
  using kwk::end;
  using kwk::of_size;

  auto e  = from(end);
  // auto de = from(end - 3);
  // auto se = from(end - 5_c);

  TTS_EQUAL( of_size(9)(e), of_size(0) );
  TTS_EXPR_IS( of_size(9)(e).extent<0>(), kwk::constant<0> );
};
