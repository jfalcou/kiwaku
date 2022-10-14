//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

TTS_CASE("Check dynamic slicing for kwk::_")
{
  using namespace kwk::literals;

  auto shp = kwk::of_size(2, 4, 5, 10);

  TTS_EQUAL(shp(kwk::_)                        , kwk::of_size(400));
  TTS_EQUAL(shp(kwk::_, kwk::_)                , kwk::of_size(2, 200));
  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_)        , kwk::of_size(2, 4, 50));
  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_), shp);
};


TTS_CASE("Check static slicing for kwk::_")
{
  using namespace kwk::literals;

  auto shp = kwk::of_size(2_c, 4_c, 5_c, 10_c);

  TTS_EQUAL(shp(kwk::_)            , kwk::of_size(400));
  TTS_EQUAL(shp(kwk::_).extent<0>(), 400U);

  TTS_EQUAL(shp(kwk::_, kwk::_)            , kwk::of_size(2, 200));
  TTS_EQUAL(shp(kwk::_, kwk::_).extent<0>(), 2U);
  TTS_EQUAL(shp(kwk::_, kwk::_).extent<1>(), 200U);

  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_)            , kwk::of_size(2, 4, 50));
  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_).extent<0>(), 2U);
  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_).extent<1>(), 4U);
  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_).extent<2>(), 50U);

  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_)            , shp);
  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_).extent<0>(), 2U);
  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_).extent<1>(), 4U);
  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_).extent<2>(), 5U);
  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_).extent<3>(), 10U);
};
