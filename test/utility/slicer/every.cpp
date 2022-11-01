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

  auto shp  = of_size(2, 2, 3_c, 3_c);
  auto shp0 = of_size(0, 0, 0_c, 0_c);

  TTS_EQUAL( shp(every(4), every(5_c), every(6), every(7_c)), of_size(1,1,1,1));
  TTS_EQUAL( shp0(every(4), every(5_c), every(6), every(7_c)), of_size(0,0,0,0));

  TTS_TYPED_EQUAL(shp(every(4), every(5_c), every(6), every(7_c)).extent<0>(), 1LL);
  TTS_TYPED_EQUAL(shp(every(4), every(5_c), every(6), every(7_c)).extent<1>(), 1LL);
  TTS_TYPED_EQUAL(shp(every(4), every(5_c), every(6), every(7_c)).extent<2>(), 1LL);
  TTS_TYPED_EQUAL(shp(every(4), every(5_c), every(6), every(7_c)).extent<3>(), fixed<1LL>);

  TTS_TYPED_EQUAL(shp0(every(4), every(5_c), every(6), every(7_c)).extent<0>(), 0LL);
  TTS_TYPED_EQUAL(shp0(every(4), every(5_c), every(6), every(7_c)).extent<1>(), 0LL);
  TTS_TYPED_EQUAL(shp0(every(4), every(5_c), every(6), every(7_c)).extent<2>(), 0LL);
  TTS_TYPED_EQUAL(shp0(every(4), every(5_c), every(6), every(7_c)).extent<3>(), fixed<0LL>);
};

TTS_CASE("Check slicing for kwk::every")
{
  using namespace kwk::literals;
  using kwk::every;
  using kwk::fixed;
  using kwk::of_size;

  constexpr std::size_t d0 = 17, d1 = 29;
  auto d2 = fixed<23>;
  auto d3 = fixed<97>;

  auto shp = of_size(d0, d1, d2, d3);

  constexpr std::size_t v0 = 4, v2 = 6;
  auto v1 = fixed<5>;
  auto v3 = fixed<7>;

  auto e0 = every(v0);
  auto e1 = every(v1);
  auto e2 = every(v2);
  auto e3 = every(v3);

  TTS_EQUAL( shp(e0, e1, e2, e3), of_size(d0/v0+1, d1/v1+1, d2/v2+1, d3/v3+1));

  TTS_TYPED_EQUAL(shp(e0, e1, e2, e3).extent<0>(), d0/v0+1);
  TTS_TYPED_EQUAL(shp(e0, e1, e2, e3).extent<1>(), d1/v1+1);
  TTS_TYPED_EQUAL(shp(e0, e1, e2, e3).extent<2>(), d2/v2+1);
  TTS_TYPED_EQUAL(shp(e0, e1, e2, e3).extent<3>(), fixed<d3/v3+1ULL>);
};

TTS_CASE("Check exact slicing for kwk::every")
{
  using namespace kwk::literals;
  using kwk::every;
  using kwk::fixed;
  using kwk::of_size;

  constexpr size_t d0 = 33, d1 = 99;
  auto d2 = fixed<100>;
  auto d3 = fixed<1000>;

  auto shp = of_size(d0, d1, d2, d3);

  constexpr size_t v0 = 3, v2 = 10;
  auto v1 = fixed<9>;
  auto v3 = fixed<5>;

  auto e0 = every(v0);
  auto e1 = every(v1);
  auto e2 = every(v2);
  auto e3 = every(v3);

  TTS_EQUAL( shp(e0, e1, e2, e3), of_size(d0/v0, d1/v1, d2/v2, d3/v3));

  TTS_TYPED_EQUAL(shp(e0, e1, e2, e3).extent<0>(), d0/v0);
  TTS_TYPED_EQUAL(shp(e0, e1, e2, e3).extent<1>(), d1/v1);
  TTS_TYPED_EQUAL(shp(e0, e1, e2, e3).extent<2>(), d2/v2);
  TTS_TYPED_EQUAL(shp(e0, e1, e2, e3).extent<3>(), fixed<d3/5ULL>);
};
