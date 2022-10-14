//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

std::uint32_t sz(auto s, auto d) { return 1 + s/d; }

TTS_CASE("Check kwk::every special cases")
{
  using namespace kwk::literals;
  using kwk::every;
  using kwk::fixed;
  using kwk::of_size;

  auto shp = of_size(2,3_c,4,5_c);

  TTS_EQUAL( shp(every(150))                            , of_size(1));
  TTS_EQUAL( shp(every(4), every(75))                   , of_size(1,1));
  TTS_EQUAL( shp(every(4), every(5), every(33))         , of_size(1,1,1));
  TTS_EQUAL( shp(every(4), every(5), every(6), every(7)), of_size(1,1,1,1));

  auto shp0 = of_size(0,0_c,0,0_c);

  TTS_EQUAL( shp0(every(150))                            , of_size(0));
  TTS_EQUAL( shp0(every(4), every(75))                   , of_size(0,0));
  TTS_EQUAL( shp0(every(4), every(5), every(33))         , of_size(0,0,0));
  TTS_EQUAL( shp0(every(4), every(5), every(6), every(7)), of_size(0,0,0,0));
};

TTS_CASE("Check dynamic slicing for kwk::every")
{
  using namespace kwk::literals;
  using kwk::every;
  using kwk::fixed;
  using kwk::of_size;

  int d0 = 13, d1 = 15, d2 = 47, d3 = 27;
  auto shp = kwk::of_size(d0, d1, d2, d3);

  constexpr int v0 = 1, v1 = 2, v2 = 3, v3 = 5;

  auto e0 = every(v0); auto c0 = every(fixed<v0>);
  auto e1 = every(v1); auto c1 = every(fixed<v1>);
  auto e2 = every(v2); auto c2 = every(fixed<v2>);
  auto e3 = every(v3); auto c3 = every(fixed<v3>);

  TTS_EQUAL ( shp(e0)             , of_size(sz(d0*d1*d2*d3, v0)));
  TTS_EQUAL ( shp(c0)             , of_size(sz(d0*d1*d2*d3, v0)));
  TTS_EQUAL ( shp(e0, e1)         , of_size(sz(d0, v0), sz(d1*d2*d3, v1)));
  TTS_EQUAL ( shp(c0, c1)         , of_size(sz(d0, v0), sz(d1*d2*d3, v1)));
  TTS_EQUAL ( shp(e0, e1, e2)     , of_size(sz(d0, v0), sz(d1, v1), sz(d2*d3, v2)));
  TTS_EQUAL ( shp(c0, c1, c2)     , of_size(sz(d0, v0), sz(d1, v1), sz(d2*d3, v2)));
  TTS_EQUAL ( shp(e0, e1, e2, e3) , of_size(sz(d0, v0), sz(d1, v1), sz(d2, v2), sz(d3, v3)));
  TTS_EQUAL ( shp(c0, c1, c2, c3) , of_size(sz(d0, v0), sz(d1, v1), sz(d2, v2), sz(d3, v3)));

  TTS_EXPECT ( shp(e0)             .is_fully_dynamic);
  TTS_EXPECT ( shp(c0)             .is_fully_dynamic);
  TTS_EXPECT ( shp(e0, e1)         .is_fully_dynamic);
  TTS_EXPECT ( shp(c0, c1)         .is_fully_dynamic);
  TTS_EXPECT ( shp(e0, e1, e2)     .is_fully_dynamic);
  TTS_EXPECT ( shp(c0, c1, c2)     .is_fully_dynamic);
  TTS_EXPECT ( shp(e0, e1, e2, e3) .is_fully_dynamic);
  TTS_EXPECT ( shp(c0, c1, c2, c3) .is_fully_dynamic);
};


TTS_CASE("Check dynamic slicing for kwk::every")
{
  using namespace kwk::literals;
  using kwk::every;
  using kwk::fixed;
  using kwk::of_size;

  auto d0 = 13_c;
  auto d1 = 15_c;
  auto d2 = 47_c;
  auto d3 = 27_c;
  
  auto shp = kwk::of_size(d0, d1, d2, d3);

  constexpr int v0 = 1, v1 = 2, v2 = 3, v3 = 5;

  auto e0 = every(v0); auto c0 = every(fixed<v0>);
  auto e1 = every(v1); auto c1 = every(fixed<v1>);
  auto e2 = every(v2); auto c2 = every(fixed<v2>);
  auto e3 = every(v3); auto c3 = every(fixed<v3>);

  TTS_EQUAL ( shp(e0)             , of_size(sz(d0*d1*d2*d3, v0)));
  TTS_EQUAL ( shp(c0)             , of_size(sz(d0*d1*d2*d3, v0)));
  TTS_EQUAL ( shp(e0, e1)         , of_size(sz(d0, v0), sz(d1*d2*d3, v1)));
  TTS_EQUAL ( shp(c0, c1)         , of_size(sz(d0, v0), sz(d1*d2*d3, v1)));
  TTS_EQUAL ( shp(e0, e1, e2)     , of_size(sz(d0, v0), sz(d1, v1), sz(d2*d3, v2)));
  TTS_EQUAL ( shp(c0, c1, c2)     , of_size(sz(d0, v0), sz(d1, v1), sz(d2*d3, v2)));
  TTS_EQUAL ( shp(e0, e1, e2, e3) , of_size(sz(d0, v0), sz(d1, v1), sz(d2, v2), sz(d3, v3)));
  TTS_EQUAL ( shp(c0, c1, c2, c3) , of_size(sz(d0, v0), sz(d1, v1), sz(d2, v2), sz(d3, v3)));

  TTS_EQUAL ( shp(c0).extent<0>()         , sz(d0*d1*d2*d3, v0));
  TTS_EQUAL ( shp(c0, c1).extent<0>()     , sz(d0, v0));
  TTS_EQUAL ( shp(c0, c1).extent<1>()     , sz(d1*d2*d3, v1));

  TTS_EQUAL ( shp(c0, c1, c2).extent<0>() , sz(d0, v0));
  TTS_EQUAL ( shp(c0, c1, c2).extent<1>() , sz(d1, v1));
  TTS_EQUAL ( shp(c0, c1, c2).extent<2>() , sz(d2*d3, v2));

  TTS_EQUAL ( shp(c0, c1, c2, c3).extent<0>() , sz(d0, v0));
  TTS_EQUAL ( shp(c0, c1, c2, c3).extent<1>() , sz(d1, v1));
  TTS_EQUAL ( shp(c0, c1, c2, c3).extent<2>() , sz(d2, v2));
  TTS_EQUAL ( shp(c0, c1, c2, c3).extent<3>() , sz(d3, v3));

  // the is_fully_static tests are redundant
  TTS_EXPECT ( shp(e0)             .is_fully_dynamic);
  TTS_EXPECT ( shp(e0, e1)         .is_fully_dynamic);
  TTS_EXPECT ( shp(e0, e1, e2)     .is_fully_dynamic);
  TTS_EXPECT ( shp(e0, e1, e2, e3) .is_fully_dynamic);
};
