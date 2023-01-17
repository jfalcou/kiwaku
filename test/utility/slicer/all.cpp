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
  TTS_EQUAL(shp(kwk::_, kwk::_)                , kwk::of_size(40, 10));
  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_)        , kwk::of_size(8, 5, 10));
  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_), shp);
};


TTS_CASE("Check static slicing for kwk::_")
{
  using namespace kwk::literals;

  auto shp = kwk::of_size(2_c, 4_c, 5_c, 10_c);

  TTS_EQUAL(shp(kwk::_)                   , kwk::of_size(400));
  TTS_TYPED_EQUAL(shp(kwk::_).extent<0>() , kwk::fixed<400>);

  TTS_EQUAL(shp(kwk::_, kwk::_)                   , kwk::of_size(40, 10));
  TTS_TYPED_EQUAL(shp(kwk::_, kwk::_).extent<0>() , kwk::fixed<40>);
  TTS_TYPED_EQUAL(shp(kwk::_, kwk::_).extent<1>() , 10_c);

  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_)                   , kwk::of_size(8, 5, 10));
  TTS_TYPED_EQUAL(shp(kwk::_, kwk::_, kwk::_).extent<0>() , kwk::fixed<8>);
  TTS_TYPED_EQUAL(shp(kwk::_, kwk::_, kwk::_).extent<1>() , 5_c );
  TTS_TYPED_EQUAL(shp(kwk::_, kwk::_, kwk::_).extent<2>() , 10_c);

  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_)                   , shp);
  TTS_TYPED_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_).extent<0>() , 2_c);
  TTS_TYPED_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_).extent<1>() , 4_c);
  TTS_TYPED_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_).extent<2>() , 5_c);
  TTS_TYPED_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_).extent<3>() , 10_c);
};
