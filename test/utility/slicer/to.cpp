//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

TTS_CASE("Check dynamic slicing for kwk::to")
{
  using namespace kwk::literals;
  using kwk::to;

  auto shp = kwk::of_size(2, 4, 5, 10);

  TTS_EQUAL(shp(to(1))                      , kwk::of_size(1));
  TTS_EQUAL(shp(to(1), to(2))               , kwk::of_size(1, 2));
  TTS_EQUAL(shp(to(1), to(2), to(3))        , kwk::of_size(1, 2, 3));
  TTS_EQUAL(shp(to(1), to(2), to(3), to(5)) , kwk::of_size(1, 2, 3, 5));

  TTS_EQUAL(shp(to(1_c))                            , kwk::of_size(1));
  // It is possible to get the compile-time extent<0> value, since the value
  // returned by to is static <=> its agrument is static
  // (the shape does not have to be static since its value is not part
  // of the computation)
  TTS_EQUAL(shp(to(1_c)).extent<0>(), 1U);

  TTS_EQUAL(shp(to(1_c), to(2_c))                   , kwk::of_size(1, 2));
  TTS_EQUAL(shp(to(1_c), to(2_c)).extent<0>(), 1U);
  TTS_EQUAL(shp(to(1_c), to(2_c)).extent<1>(), 2U);

  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c))          , kwk::of_size(1, 2, 3));
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c)).extent<0>(), 1U);
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c)).extent<1>(), 2U);
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c)).extent<2>(), 3U);

  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c), to(5_c)) , kwk::of_size(1, 2, 3, 5));
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c), to(5_c)).extent<0>(), 1U);
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c), to(5_c)).extent<1>(), 2U);
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c), to(5_c)).extent<2>(), 3U);
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c), to(5_c)).extent<3>(), 5U);

};

TTS_CASE("Check static slicing for kwk::to")
{
  using namespace kwk::literals;
  using kwk::to;

  auto shp = kwk::of_size(2_c, 4_c, 5_c, 10_c);

  TTS_EQUAL(shp(to(1))                      , kwk::of_size(1));
  TTS_EXPECT(shp(to(1))  .is_fully_dynamic);
  TTS_EXPECT(shp(to(1_c)).is_fully_static);

  TTS_EQUAL (shp(to(1), to(2))               , kwk::of_size(1, 2));
  TTS_EXPECT(shp(to(1), to(2)).is_fully_dynamic);
  TTS_EXPECT(shp(to(1_c), to(2_c)).is_fully_static);

  TTS_EQUAL (shp(to(1), to(2), to(3))        , kwk::of_size(1, 2, 3));
  TTS_EXPECT(shp(to(1), to(2), to(3)).is_fully_dynamic);
  TTS_EXPECT(shp(to(1_c), to(2_c), to(3_c)).is_fully_static);
  
  TTS_EQUAL (shp(to(1), to(2), to(3), to(5)) , kwk::of_size(1, 2, 3, 5));
  TTS_EXPECT(shp(to(1), to(2), to(3), to(5)).is_fully_dynamic);
  TTS_EXPECT(shp(to(1_c), to(2_c), to(3_c), to(5_c)).is_fully_static);

  TTS_EQUAL(shp(to(1_c))                            , kwk::of_size(1));
  TTS_EQUAL(shp(to(1_c)).extent<0>(), 1U);

  TTS_EQUAL(shp(to(1_c), to(2_c))                   , kwk::of_size(1, 2));
  TTS_EQUAL(shp(to(1_c), to(2_c)).extent<0>(), 1U);
  TTS_EQUAL(shp(to(1_c), to(2_c)).extent<1>(), 2U);

  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c))          , kwk::of_size(1, 2, 3));
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c)).extent<0>(), 1U);
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c)).extent<1>(), 2U);
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c)).extent<2>(), 3U);

  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c), to(5_c)) , kwk::of_size(1, 2, 3, 5));
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c), to(5_c)).extent<0>(), 1U);
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c), to(5_c)).extent<1>(), 2U);
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c), to(5_c)).extent<2>(), 3U);
  TTS_EQUAL(shp(to(1_c), to(2_c), to(3_c), to(5_c)).extent<3>(), 5U);
};
