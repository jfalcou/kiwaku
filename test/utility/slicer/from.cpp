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

  constexpr unsigned int v0 = 1, v1 = 2, v2 = 3, v3 = 5;

  auto f0 = from(v0); auto c0 = from(fixed<v0>);
  auto f1 = from(v1); auto c1 = from(fixed<v1>);
  auto f2 = from(v2); auto c2 = from(fixed<v2>);
  auto f3 = from(v3); auto c3 = from(fixed<v3>);

  auto shp = kwk::of_size(d0, d1, d2, d3);

  // Shrink to 1D
  TTS_EQUAL(shp(f0)             , kwk::of_size(d0*d1*d2*d3 - v0));
  TTS_EQUAL(shp(c0)             , kwk::of_size(d0*d1*d2*d3 - v0));
  TTS_EQUAL(shp(c0).extent<0>() , (d0*d1*d2*d3 - v0));
  TTS_EXPECT(shp(f0).is_fully_dynamic);
  TTS_EXPECT(shp(c0).is_fully_static);

  // Shrink to 2D
  TTS_EQUAL(shp(f0, f1)             , kwk::of_size(d0 - v0, d1*d2*d3 - v1));
  TTS_EQUAL(shp(c0, c1)             , kwk::of_size(d0 - v0, d1*d2*d3 - v1));
  TTS_EQUAL(shp(c0, c1).extent<0>() , (d0 - v0));
  TTS_EQUAL(shp(c0, c1).extent<1>() , (d1*d2*d3 - v1));
  TTS_EXPECT    (shp(c0, c1).is_fully_static);
  TTS_EXPECT_NOT(shp(c0, f1).is_fully_dynamic);
  TTS_EXPECT    (shp(f0, f1).is_fully_dynamic);

  // Shrink to 3D
  TTS_EQUAL(shp(f0, f1, f2)             , kwk::of_size(d0 - v0, d1 - v1, d2*d3 - v2));
  TTS_EQUAL(shp(c0, c1, c2)             , kwk::of_size(d0 - v0, d1 - v1, d2*d3 - v2));
  TTS_EQUAL(shp(c0, c1, c2).extent<0>() , (d0 - v0));
  TTS_EQUAL(shp(c0, c1, c2).extent<1>() , (d1 - v1));
  TTS_EQUAL(shp(c0, c1, c2).extent<2>() , (d2*d3 - v2));
  TTS_EXPECT    (shp(c0, c1, c2).is_fully_static);
  TTS_EXPECT_NOT(shp(f0, f1, c2).is_fully_dynamic);
  TTS_EXPECT    (shp(f0, f1, f2).is_fully_dynamic);

  // Same for 4D (most likely redundant)
  TTS_EQUAL(shp(f0, f1, f2, f3)             , kwk::of_size(d0 - v0, d1 - v1, d2 - v2, d3 - v3));
  TTS_EQUAL(shp(c0, c1, c2, c3)             , kwk::of_size(d0 - v0, d1 - v1, d2 - v2, d3 - v3));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<0>() , (d0 - v0));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<1>() , (d1 - v1));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<2>() , (d2 - v2));
  TTS_EQUAL(shp(c0, c1, c2, c3).extent<3>() , (d3 - v3));
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
  auto de = from(end - 3);
  auto se = from(end - 3_c);

  TTS_EQUAL( of_size(9)(e)            , of_size(0)      );
  TTS_EQUAL( of_size(9,7)(e,e)        , of_size(0,0)    );
  TTS_EQUAL( of_size(9,7,5)(e,e,e)    , of_size(0,0,0)  );
  TTS_EQUAL( of_size(9,7,5,3)(e,e,e,e), of_size(0,0,0,0));

  TTS_EXPECT( of_size(9)(e).is_fully_static );
  TTS_EXPECT( of_size(9,7)(e,e).is_fully_static );
  TTS_EXPECT( of_size(9,7,5)(e,e,e).is_fully_static );
  TTS_EXPECT( of_size(9,7,5,3)(e,e,e,e).is_fully_static );

  TTS_EQUAL( of_size(9)(de)               , of_size(3)      );
  TTS_EQUAL( of_size(9,7)(de,de)          , of_size(3,3)    );
  TTS_EQUAL( of_size(9,7,5)(de,de,de)     , of_size(3,3,3)  );
  TTS_EQUAL( of_size(9,7,5,3)(de,de,de,de), of_size(3,3,3,3));

  TTS_EXPECT( of_size(9)(de).is_fully_dynamic);
  TTS_EXPECT( of_size(9,7)(de,de).is_fully_dynamic);
  TTS_EXPECT( of_size(9,7,5)(de,de,de).is_fully_dynamic);
  TTS_EXPECT( of_size(9,7,5,3)(de,de,de,de).is_fully_dynamic);

  TTS_EQUAL( of_size(9_c)(de)                     , of_size(3)      );
  TTS_EQUAL( of_size(9_c,7_c)(de,de)              , of_size(3,3)    );
  TTS_EQUAL( of_size(9_c,7_c,5_c)(de,de,de)       , of_size(3,3,3)  );
  TTS_EQUAL( of_size(9_c,7_c,5_c,3_c)(de,de,de,de), of_size(3,3,3,3));

  TTS_EXPECT( of_size(9_c)(de).is_fully_dynamic);
  TTS_EXPECT( of_size(9_c,7_c)(de,de).is_fully_dynamic);
  TTS_EXPECT( of_size(9_c,7_c,5_c)(de,de,de).is_fully_dynamic);
  TTS_EXPECT( of_size(9_c,7_c,5_c,3_c)(de,de,de,de).is_fully_dynamic);

  TTS_EQUAL( of_size(9)(se)               , of_size(3)      );
  TTS_EQUAL( of_size(9,7)(se,se)          , of_size(3,3)    );
  TTS_EQUAL( of_size(9,7,5)(se,se,se)     , of_size(3,3,3)  );
  TTS_EQUAL( of_size(9,7,5,3)(se,se,se,se), of_size(3,3,3,3));

  TTS_EXPECT( of_size(9)(se).is_fully_static);
  TTS_EXPECT( of_size(9,7)(se,se).is_fully_static);
  TTS_EXPECT( of_size(9,7,5)(se,se,se).is_fully_static);
  TTS_EXPECT( of_size(9,7,5,3)(se,se,se,se).is_fully_static);

  TTS_EQUAL( of_size(9_c)(se)                     , of_size(3)      );
  TTS_EQUAL( of_size(9_c,7_c)(se,se)              , of_size(3,3)    );
  TTS_EQUAL( of_size(9_c,7_c,5_c)(se,se,se)       , of_size(3,3,3)  );
  TTS_EQUAL( of_size(9_c,7_c,5_c,3_c)(se,se,se,se), of_size(3,3,3,3));

  TTS_EXPECT( of_size(9_c)(se).is_fully_static);
  TTS_EXPECT( of_size(9_c,7_c)(se,se).is_fully_static);
  TTS_EXPECT( of_size(9_c,7_c,5_c)(se,se,se).is_fully_static);
  TTS_EXPECT( of_size(9_c,7_c,5_c,3_c)(se,se,se,se).is_fully_static);
};
