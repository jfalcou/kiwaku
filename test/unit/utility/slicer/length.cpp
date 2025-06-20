//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include "tts.hpp"
#include <kwk/utility/container/shape.hpp>

TTS_CASE("Check slicing for kwk::length")
{
  using namespace kwk::literals;
  using kwk::slice;
  using kwk::length;
  using kwk::fixed;
  using kwk::of_size;

  int d0 = 13, d1 = 15, d2 = 47, d3 = 27;
  auto shp = kwk::of_size(d0, d1, d2, d3);

  constexpr unsigned int v0 = 1, v1 = 4, v2 = 8, v3 = 9;

  auto b0 = slice(length = v0); auto c0 = slice(length = fixed<v0>);
  auto b1 = slice(length = v1); auto c1 = slice(length = fixed<v1>);
  auto b2 = slice(length = v2); auto c2 = slice(length = fixed<v2>);
  auto b3 = slice(length = v3); auto c3 = slice(length = fixed<v3>);

  TTS_EQUAL ( shp(b0)             , of_size(v0));
  TTS_EQUAL ( shp(b0, b1)         , of_size(v0, v1));
  TTS_EQUAL ( shp(b0, b1, b2)     , of_size(v0, v1, v2));
  TTS_EQUAL ( shp(b0, b1, b2, b3) , of_size(v0, v1, v2, v3));

  TTS_EQUAL ( shp(c0)             , of_size(v0));
  TTS_EQUAL ( shp(c0, c1)         , of_size(v0, v1));
  TTS_EQUAL ( shp(c0, c1, c2)     , of_size(v0, v1, v2));
  TTS_EQUAL ( shp(c0, c1, c2, c3) , of_size(v0, v1, v2, v3));

  TTS_EQUAL(get<0>(shp(c0))             , v0);

  TTS_EQUAL(get<0>(shp(c0, c1))         , v0);
  TTS_EQUAL(get<1>(shp(c0, c1))         , v1);

  TTS_EQUAL(get<0>(shp(c0, c1, c2))     , v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2))     , v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2))     , v2);

  TTS_EQUAL(get<0>(shp(c0, c1, c2, c3)) , v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2, c3)) , v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2, c3)) , v2);
  TTS_EQUAL(get<3>(shp(c0, c1, c2, c3)) , v3);
};

TTS_CASE("Check slicing for kwk::length + kwk::from")
{
  using namespace kwk::literals;
  using kwk::slice;
  using kwk::length;
  using kwk::from;
  using kwk::fixed;
  using kwk::of_size;

  int d0 = 13, d1 = 15, d2 = 47, d3 = 27;
  auto shp = kwk::of_size(d0, d1, d2, d3);

  constexpr unsigned int v0 = 1, v1 = 4, v2 = 8, v3 = 9;

  auto b0 = slice(length = v0, from = 4); auto c0 = slice(length = fixed<v0>, from = fixed<2>);
  auto b1 = slice(length = v1, from = 4); auto c1 = slice(length = fixed<v1>, from = fixed<2>);
  auto b2 = slice(length = v2, from = 4); auto c2 = slice(length = fixed<v2>, from = fixed<2>);
  auto b3 = slice(length = v3, from = 4); auto c3 = slice(length = fixed<v3>, from = fixed<2>);

  TTS_EQUAL ( shp(b0)             , of_size(v0));
  TTS_EQUAL ( shp(b0, b1)         , of_size(v0, v1));
  TTS_EQUAL ( shp(b0, b1, b2)     , of_size(v0, v1, v2));
  TTS_EQUAL ( shp(b0, b1, b2, b3) , of_size(v0, v1, v2, v3));

  TTS_EQUAL ( shp(c0)             , of_size(v0));
  TTS_EQUAL ( shp(c0, c1)         , of_size(v0, v1));
  TTS_EQUAL ( shp(c0, c1, c2)     , of_size(v0, v1, v2));
  TTS_EQUAL ( shp(c0, c1, c2, c3) , of_size(v0, v1, v2, v3));

  TTS_EQUAL(get<0>(shp(c0))             , v0);

  TTS_EQUAL(get<0>(shp(c0, c1))         , v0);
  TTS_EQUAL(get<1>(shp(c0, c1))         , v1);

  TTS_EQUAL(get<0>(shp(c0, c1, c2))     , v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2))     , v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2))     , v2);

  TTS_EQUAL(get<0>(shp(c0, c1, c2, c3)) , v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2, c3)) , v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2, c3)) , v2);
  TTS_EQUAL(get<3>(shp(c0, c1, c2, c3)) , v3);
};

TTS_CASE("Check slicing for kwk::length + kwk::to")
{
  using namespace kwk::literals;
  using kwk::slice;
  using kwk::length;
  using kwk::to;
  using kwk::fixed;
  using kwk::of_size;

  int d0 = 13, d1 = 15, d2 = 47, d3 = 27;
  auto shp = kwk::of_size(d0, d1, d2, d3);

  constexpr unsigned int v0 = 1, v1 = 4, v2 = 8, v3 = 9;

  auto b0 = slice(length = v0, to = 100); auto c0 = slice(length = fixed<v0>, to = fixed<100>);
  auto b1 = slice(length = v1, to = 100); auto c1 = slice(length = fixed<v1>, to = fixed<100>);
  auto b2 = slice(length = v2, to = 100); auto c2 = slice(length = fixed<v2>, to = fixed<100>);
  auto b3 = slice(length = v3, to = 100); auto c3 = slice(length = fixed<v3>, to = fixed<100>);

  TTS_EQUAL ( shp(b0)             , of_size(v0));
  TTS_EQUAL ( shp(b0, b1)         , of_size(v0, v1));
  TTS_EQUAL ( shp(b0, b1, b2)     , of_size(v0, v1, v2));
  TTS_EQUAL ( shp(b0, b1, b2, b3) , of_size(v0, v1, v2, v3));

  TTS_EQUAL ( shp(c0)             , of_size(v0));
  TTS_EQUAL ( shp(c0, c1)         , of_size(v0, v1));
  TTS_EQUAL ( shp(c0, c1, c2)     , of_size(v0, v1, v2));
  TTS_EQUAL ( shp(c0, c1, c2, c3) , of_size(v0, v1, v2, v3));

  TTS_EQUAL(get<0>(shp(c0))             , v0);

  TTS_EQUAL(get<0>(shp(c0, c1))         , v0);
  TTS_EQUAL(get<1>(shp(c0, c1))         , v1);

  TTS_EQUAL(get<0>(shp(c0, c1, c2))     , v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2))     , v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2))     , v2);

  TTS_EQUAL(get<0>(shp(c0, c1, c2, c3)) , v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2, c3)) , v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2, c3)) , v2);
  TTS_EQUAL(get<3>(shp(c0, c1, c2, c3)) , v3);
};

TTS_CASE("Check slicing for kwk::length + kwk::by")
{
  using namespace kwk::literals;
  using kwk::slice;
  using kwk::length;
  using kwk::by;
  using kwk::fixed;
  using kwk::of_size;

  int d0 = 13, d1 = 15, d2 = 47, d3 = 27;
  auto shp = kwk::of_size(d0, d1, d2, d3);

  constexpr unsigned int v0 = 1, v1 = 4, v2 = 8, v3 = 9;

  auto b0 = slice(length = v0, by = 2); auto c0 = slice(length = fixed<v0>, by = fixed<4>);
  auto b1 = slice(length = v1, by = 2); auto c1 = slice(length = fixed<v1>, by = fixed<4>);
  auto b2 = slice(length = v2, by = 2); auto c2 = slice(length = fixed<v2>, by = fixed<4>);
  auto b3 = slice(length = v3, by = 2); auto c3 = slice(length = fixed<v3>, by = fixed<4>);

  TTS_EQUAL ( shp(b0)             , of_size(v0));
  TTS_EQUAL ( shp(b0, b1)         , of_size(v0, v1));
  TTS_EQUAL ( shp(b0, b1, b2)     , of_size(v0, v1, v2));
  TTS_EQUAL ( shp(b0, b1, b2, b3) , of_size(v0, v1, v2, v3));

  TTS_EQUAL ( shp(c0)             , of_size(v0));
  TTS_EQUAL ( shp(c0, c1)         , of_size(v0, v1));
  TTS_EQUAL ( shp(c0, c1, c2)     , of_size(v0, v1, v2));
  TTS_EQUAL ( shp(c0, c1, c2, c3) , of_size(v0, v1, v2, v3));

  TTS_EQUAL(get<0>(shp(c0))             , v0);

  TTS_EQUAL(get<0>(shp(c0, c1))         , v0);
  TTS_EQUAL(get<1>(shp(c0, c1))         , v1);

  TTS_EQUAL(get<0>(shp(c0, c1, c2))     , v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2))     , v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2))     , v2);

  TTS_EQUAL(get<0>(shp(c0, c1, c2, c3)) , v0);
  TTS_EQUAL(get<1>(shp(c0, c1, c2, c3)) , v1);
  TTS_EQUAL(get<2>(shp(c0, c1, c2, c3)) , v2);
  TTS_EQUAL(get<3>(shp(c0, c1, c2, c3)) , v3);
};
