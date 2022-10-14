//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

TTS_CASE("Check dynamic slicing for kwk::between")
{
  using namespace kwk::literals;
  using kwk::between;
  using kwk::fixed;
  using kwk::of_size;

  int d0 = 13, d1 = 15, d2 = 47, d3 = 27;
  auto shp = kwk::of_size(d0, d1, d2, d3);

  constexpr int v0 = 1, v1 = 4, v2 = 8, v3 = 9, v4 = 11;

  auto b0 = between(v0, v1); auto c0 = between(fixed<v0>, fixed<v1>);
  auto b1 = between(v1, v2); auto c1 = between(fixed<v1>, fixed<v2>);
  auto b2 = between(v2, v3); auto c2 = between(fixed<v2>, fixed<v3>);
  auto b3 = between(v3, v4); auto c3 = between(fixed<v3>, fixed<v4>);

  TTS_EQUAL ( shp(b0)             , of_size(v1 - v0));
  TTS_EQUAL ( shp(b0, b1)         , of_size(v1 - v0, v2 - v1));
  TTS_EQUAL ( shp(b0, b1, b2)     , of_size(v1 - v0, v2 - v1, v3 - v2));
  TTS_EQUAL ( shp(b0, b1, b2, b3) , of_size(v1 - v0, v2 - v1, v3 - v2, v4 - v3));

  TTS_EQUAL ( shp(c0)             , of_size(v1 - v0));
  TTS_EQUAL ( shp(c0, c1)         , of_size(v1 - v0, v2 - v1));
  TTS_EQUAL ( shp(c0, c1, c2)     , of_size(v1 - v0, v2 - v1, v3 - v2));
  TTS_EQUAL ( shp(c0, c1, c2, c3) , of_size(v1 - v0, v2 - v1, v3 - v2, v4 - v3));

  // TTS_EXPR_IS(shp(b0).extent<0>(), kwk::joker); already tested in API.exe
  TTS_EQUAL(shp(c0).extent<0>()             , static_cast<std::uint32_t>(v1 - v0));
  TTS_EQUAL(shp(c0, c1).extent<0>()         , static_cast<std::uint32_t>(v1 - v0));
  TTS_EQUAL(shp(c0, c1).extent<1>()         , static_cast<std::uint32_t>(v2 - v1));

  TTS_EQUAL(shp(c0, c1, c2).extent<0>()     , static_cast<std::uint32_t>(v1 - v0));
  TTS_EQUAL(shp(c0, c1, c2).extent<1>()     , static_cast<std::uint32_t>(v2 - v1));
  TTS_EQUAL(shp(c0, c1, c2).extent<2>()     , static_cast<std::uint32_t>(v3 - v2));

  TTS_EQUAL(shp(c0, c1, c2, c3).extent<0>() , static_cast<std::uint32_t>(v1 - v0));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<1>() , static_cast<std::uint32_t>(v2 - v1));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<2>() , static_cast<std::uint32_t>(v3 - v2));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<3>() , static_cast<std::uint32_t>(v4 - v3));
};

TTS_CASE("Check static slicing for kwk::between")
{
  using namespace kwk::literals;
  using kwk::between;
  using kwk::fixed;
  using kwk::of_size;

  auto d0 = 13_c;
  auto d1 = 15_c;
  auto d2 = 47_c;
  auto d3 = 27_c;

  auto shp = kwk::of_size(d0, d1, d2, d3);

  constexpr int v0 = 1, v1 = 4, v2 = 8, v3 = 9, v4 = 11;

  auto b0 = between(v0, v1); auto c0 = between(fixed<v0>, fixed<v1>);
  auto b1 = between(v1, v2); auto c1 = between(fixed<v1>, fixed<v2>);
  auto b2 = between(v2, v3); auto c2 = between(fixed<v2>, fixed<v3>);
  auto b3 = between(v3, v4); auto c3 = between(fixed<v3>, fixed<v4>);

  TTS_EQUAL ( shp(b0)             , of_size(v1 - v0));
  TTS_EQUAL ( shp(b0, b1)         , of_size(v1 - v0, v2 - v1));
  TTS_EQUAL ( shp(b0, b1, b2)     , of_size(v1 - v0, v2 - v1, v3 - v2));
  TTS_EQUAL ( shp(b0, b1, b2, b3) , of_size(v1 - v0, v2 - v1, v3 - v2, v4 - v3));

  TTS_EXPECT ( shp(b0).is_fully_dynamic);
  TTS_EXPECT ( shp(b0, b1).is_fully_dynamic);
  TTS_EXPECT ( shp(b0, b1, b2).is_fully_dynamic);
  TTS_EXPECT ( shp(b0, b1, b2, b3).is_fully_dynamic);

  TTS_EQUAL ( shp(c0)             , of_size(v1 - v0));
  TTS_EQUAL ( shp(c0, c1)         , of_size(v1 - v0, v2 - v1));
  TTS_EQUAL ( shp(c0, c1, c2)     , of_size(v1 - v0, v2 - v1, v3 - v2));
  TTS_EQUAL ( shp(c0, c1, c2, c3) , of_size(v1 - v0, v2 - v1, v3 - v2, v4 - v3));

  // TTS_EXPR_IS(shp(b0).extent<0>(), kwk::joker); already tested in API.exe
  TTS_EQUAL(shp(c0).extent<0>()             , static_cast<std::uint32_t>(v1 - v0));
  TTS_EQUAL(shp(c0, c1).extent<0>()         , static_cast<std::uint32_t>(v1 - v0));
  TTS_EQUAL(shp(c0, c1).extent<1>()         , static_cast<std::uint32_t>(v2 - v1));

  TTS_EQUAL(shp(c0, c1, c2).extent<0>()     , static_cast<std::uint32_t>(v1 - v0));
  TTS_EQUAL(shp(c0, c1, c2).extent<1>()     , static_cast<std::uint32_t>(v2 - v1));
  TTS_EQUAL(shp(c0, c1, c2).extent<2>()     , static_cast<std::uint32_t>(v3 - v2));

  TTS_EQUAL(shp(c0, c1, c2, c3).extent<0>() , static_cast<std::uint32_t>(v1 - v0));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<1>() , static_cast<std::uint32_t>(v2 - v1));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<2>() , static_cast<std::uint32_t>(v3 - v2));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<3>() , static_cast<std::uint32_t>(v4 - v3));
};