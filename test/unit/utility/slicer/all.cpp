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
  TTS_TYPED_EQUAL(get<0>(shp(kwk::_)) , kwk::fixed<400>);

  TTS_EQUAL(shp(kwk::_, kwk::_)                   , kwk::of_size(40, 10));
  TTS_TYPED_EQUAL(get<0>(shp(kwk::_, kwk::_)) , kwk::fixed<40>);
  TTS_TYPED_EQUAL(get<1>(shp(kwk::_, kwk::_)) , 10_c);

  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_)                   , kwk::of_size(8, 5, 10));
  TTS_TYPED_EQUAL(get<0>(shp(kwk::_, kwk::_, kwk::_)) , kwk::fixed<8>);
  TTS_TYPED_EQUAL(get<1>(shp(kwk::_, kwk::_, kwk::_)) , 5_c );
  TTS_TYPED_EQUAL(get<2>(shp(kwk::_, kwk::_, kwk::_)) , 10_c);

  TTS_EQUAL(shp(kwk::_, kwk::_, kwk::_, kwk::_)                   , shp);
  TTS_TYPED_EQUAL(get<0>(shp(kwk::_, kwk::_, kwk::_, kwk::_)) , 2_c);
  TTS_TYPED_EQUAL(get<1>(shp(kwk::_, kwk::_, kwk::_, kwk::_)) , 4_c);
  TTS_TYPED_EQUAL(get<2>(shp(kwk::_, kwk::_, kwk::_, kwk::_)) , 5_c);
  TTS_TYPED_EQUAL(get<3>(shp(kwk::_, kwk::_, kwk::_, kwk::_)) , 10_c);
};
