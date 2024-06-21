//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

TTS_CASE("Check dynamic slicing for kwk::from/kwk::to combo")
{
  using namespace kwk::literals;
  using kwk::slice;
  using kwk::from;
  using kwk::to;
  using kwk::fixed;
  using kwk::of_size;

  int d0 = 13, d1 = 15, d2 = 47, d3 = 27;
  auto shp = kwk::of_size(d0, d1, d2, d3);

  constexpr unsigned int v0 = 1, v1 = 4, v2 = 8, v3 = 9, v4 = 11;

  auto b0 = slice(from = v0, to = v1); auto c0 = slice(from = fixed<v0>, to = fixed<v1>);
  auto b1 = slice(from = v1, to = v2); auto c1 = slice(from = fixed<v1>, to = fixed<v2>);
  auto b2 = slice(from = v2, to = v3); auto c2 = slice(from = fixed<v2>, to = fixed<v3>);
  auto b3 = slice(from = v3, to = v4); auto c3 = slice(from = fixed<v3>, to = fixed<v4>);

  TTS_EQUAL ( shp(b0)             , of_size(v1 - v0));
  TTS_EQUAL ( shp(b0, b1)         , of_size(v1 - v0, v2 - v1));
  TTS_EQUAL ( shp(b0, b1, b2)     , of_size(v1 - v0, v2 - v1, v3 - v2));
  TTS_EQUAL ( shp(b0, b1, b2, b3) , of_size(v1 - v0, v2 - v1, v3 - v2, v4 - v3));

  TTS_EQUAL ( shp(c0)             , of_size(v1 - v0));
  TTS_EQUAL ( shp(c0, c1)         , of_size(v1 - v0, v2 - v1));
  TTS_EQUAL ( shp(c0, c1, c2)     , of_size(v1 - v0, v2 - v1, v3 - v2));
  TTS_EQUAL ( shp(c0, c1, c2, c3) , of_size(v1 - v0, v2 - v1, v3 - v2, v4 - v3));

  TTS_EQUAL(get<0>(shp(c0))             , v1 - v0);
  TTS_EQUAL(get<0>(shp(c0, c1))         , v1 - v0);
  TTS_EQUAL(get<1>(shp(c0, c1))         , v2 - v1);

  TTS_EQUAL(get<0>(shp(c0, c1, c2))     , v1 - v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2))     , v2 - v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2))     , v3 - v2);

  TTS_EQUAL(get<0>(shp(c0, c1, c2, c3)) , v1 - v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2, c3)) , v2 - v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2, c3)) , v3 - v2);
  TTS_EQUAL(get<3>(shp(c0, c1, c2, c3)) , v4 - v3);
};

TTS_CASE("Check static slicing for kwk::from/kwk::to combo")
{
  using namespace kwk::literals;
  using kwk::slice;
  using kwk::from;
  using kwk::to;
  using kwk::fixed;
  using kwk::of_size;

  auto d0 = 13_c;
  auto d1 = 15_c;
  auto d2 = 47_c;
  auto d3 = 27_c;

  auto shp = kwk::of_size(d0, d1, d2, d3);

  constexpr unsigned int v0 = 1, v1 = 4, v2 = 8, v3 = 9, v4 = 11;

  auto b0 = slice(from = v0, to = v1); auto c0 = slice(from = fixed<v0>, to = fixed<v1>);
  auto b1 = slice(from = v1, to = v2); auto c1 = slice(from = fixed<v1>, to = fixed<v2>);
  auto b2 = slice(from = v2, to = v3); auto c2 = slice(from = fixed<v2>, to = fixed<v3>);
  auto b3 = slice(from = v3, to = v4); auto c3 = slice(from = fixed<v3>, to = fixed<v4>);

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

  TTS_EQUAL(get<0>(shp(c0))             , v1 - v0);
  TTS_EQUAL(get<0>(shp(c0, c1))         , v1 - v0);
  TTS_EQUAL(get<1>(shp(c0, c1))         , v2 - v1);

  TTS_EQUAL(get<0>(shp(c0, c1, c2))     , v1 - v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2))     , v2 - v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2))     , v3 - v2);

  TTS_EQUAL(get<0>(shp(c0, c1, c2, c3)) , v1 - v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2, c3)) , v2 - v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2, c3)) , v3 - v2);
  TTS_EQUAL(get<3>(shp(c0, c1, c2, c3)) , v4 - v3);
};
