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

TTS_CASE("Check kwk::by special cases")
{
  using namespace kwk::literals;
  using kwk::range;
  using kwk::by;
  using kwk::fixed;
  using kwk::of_size;

  auto shp  = of_size(2, 2, 3_c, 3_c);
  auto shp0 = of_size(0, 0, 0_c, 0_c);

  constexpr auto unit = 1, zero = 0;

  TTS_EQUAL( shp(range(by=4), range(by=5_c), range(by=6), range(by=7_c)), of_size(1,1,1,1));
  TTS_EQUAL( shp0(range(by=4), range(by=5_c), range(by=6), range(by=7_c)), of_size(0,0,0,0));

  TTS_TYPED_EQUAL(get<0>(shp(range(by=4), range(by=5_c), range(by=6), range(by=7_c))), unit);
  TTS_TYPED_EQUAL(get<1>(shp(range(by=4), range(by=5_c), range(by=6), range(by=7_c))), unit);
  TTS_TYPED_EQUAL(get<2>(shp(range(by=4), range(by=5_c), range(by=6), range(by=7_c))), unit);
  TTS_TYPED_EQUAL(get<3>(shp(range(by=4), range(by=5_c), range(by=6), range(by=7_c))), fixed<unit>);

  TTS_TYPED_EQUAL(get<0>(shp0(range(by=4), range(by=5_c), range(by=6), range(by=7_c))), zero);
  TTS_TYPED_EQUAL(get<1>(shp0(range(by=4), range(by=5_c), range(by=6), range(by=7_c))), zero);
  TTS_TYPED_EQUAL(get<2>(shp0(range(by=4), range(by=5_c), range(by=6), range(by=7_c))), zero);
  TTS_TYPED_EQUAL(get<3>(shp0(range(by=4), range(by=5_c), range(by=6), range(by=7_c))), fixed<zero>);
};

TTS_CASE("Check slicing for kwk::by")
{
  using namespace kwk::literals;
  using kwk::range;
  using kwk::by;
  using kwk::fixed;
  using kwk::of_size;

  constexpr int d0 = 17, d1 = 29;
  auto d2 = fixed<23>;
  auto d3 = fixed<97>;

  auto shp = of_size(d0, d1, d2, d3);

  constexpr int v0 = 4, v2 = 6, one = 1;
  auto v1 = fixed<5>;
  auto v3 = fixed<7>;

  auto e0 = range(by=v0);
  auto e1 = range(by=v1);
  auto e2 = range(by=v2);
  auto e3 = range(by=v3);

  TTS_EQUAL( shp(e0, e1, e2, e3), of_size(d0/v0+1, d1/v1+1,d2/v2+1, d3/v3+1));

  TTS_TYPED_EQUAL(get<0>(shp(e0, e1, e2, e3)), d0/v0+one);
  TTS_TYPED_EQUAL(get<1>(shp(e0, e1, e2, e3)), d1/v1+one);
  TTS_TYPED_EQUAL(get<2>(shp(e0, e1, e2, e3)), d2/v2+one);
  TTS_TYPED_EQUAL(get<3>(shp(e0, e1, e2, e3)), d3/v3+fixed<one>);
};

TTS_CASE("Check exact slicing for kwk::by")
{
  using namespace kwk::literals;
  using kwk::range;
  using kwk::by;
  using kwk::fixed;
  using kwk::of_size;

  constexpr std::ptrdiff_t d0 = 33, d1 = 99;
  auto d2 = fixed<100>;
  auto d3 = fixed<1000>;

  auto shp = of_size(d0, d1, d2, d3);

  constexpr std::ptrdiff_t v0 = 3, v2 = 10;
  auto v1 = fixed<std::ptrdiff_t{9}>;
  auto v3 = fixed<std::ptrdiff_t{5}>;

  auto e0 = range(by=v0);
  auto e1 = range(by=v1);
  auto e2 = range(by=v2);
  auto e3 = range(by=v3);

  TTS_EQUAL( shp(e0, e1, e2, e3), of_size(d0/v0, d1/v1, d2/v2, d3/v3));

  TTS_TYPED_EQUAL(get<0>(shp(e0, e1, e2, e3)), d0/v0);
  TTS_TYPED_EQUAL(get<1>(shp(e0, e1, e2, e3)), d1/v1);
  TTS_TYPED_EQUAL(get<2>(shp(e0, e1, e2, e3)), d2/v2);
  TTS_TYPED_EQUAL(get<3>(shp(e0, e1, e2, e3)), d3/v3);
};
