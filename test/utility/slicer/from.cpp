//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

TTS_CASE("Check basic slicing for kwk::from")
{
  using namespace kwk::literals;
  using kwk::range;
  using kwk::from;
  using kwk::fixed;

  constexpr std::size_t d0 = 1, d1 = 2, d2 = 3, d3 = 5;
  auto s2 = kwk::fixed<d2>;
  auto s3 = kwk::fixed<d3>;

  constexpr std::size_t f = 1;

  auto t0 = range(from = f);
  auto t1 = range(from = fixed<f>);
  auto t2 = range(from = f);
  auto t3 = range(from = fixed<f>);

  auto shp = kwk::of_size(d0, d1, s2, s3);

  TTS_EQUAL( shp(t0, t1, t2, t3) , kwk::of_size(d0-f, d1-f, d2-f, d3-f));

  TTS_TYPED_EQUAL(shp(t0, t1, t2, t3).extent<0>(), d0-f);
  TTS_TYPED_EQUAL(shp(t0, t1, t2, t3).extent<1>(), d1-f);
  TTS_TYPED_EQUAL(shp(t0, t1, t2, t3).extent<2>(), d2-f);
  TTS_TYPED_EQUAL(shp(t0, t1, t2, t3).extent<3>(), fixed<d3-f>);
};

TTS_CASE("Check slicing with end for kwk::from")
{
  using namespace kwk::literals;
  using kwk::range;
  using kwk::from;
  using kwk::fixed;
  using kwk::end;

  constexpr std::ptrdiff_t d0 = 1, d1 = 3, zero = 0;
  auto s1 = kwk::fixed<d1>;
  auto t0 = range(from = end);

  auto shp = kwk::of_size(d0, s1);

  TTS_EQUAL( shp(t0, t0), kwk::of_size(0, 0));
  TTS_TYPED_EQUAL(shp(t0, t0).extent<0>(), zero);
  TTS_TYPED_EQUAL(shp(t0, t0).extent<1>(), fixed<zero>);
};

TTS_CASE("Check slicing with end-n for kwk::from")
{
  using namespace kwk::literals;
  using kwk::range;
  using kwk::from;
  using kwk::fixed;
  using kwk::end;


  constexpr std::size_t d0 = 1, d1 = 3, d2 = 5, d3 = 7;
  auto s1 = kwk::fixed<d1>;
  auto s3 = kwk::fixed<d3>;

  constexpr std::size_t v0 = 1;
  auto c0 = fixed<v0>;
  auto t0 = range(from = end-v0);
  auto t1 = range(from = end-c0);

  auto shp = kwk::of_size(d0, s1, d2, s3);

  TTS_EQUAL( shp(t0, t0, t1, t1), kwk::of_size(v0, v0, c0, c0));

  // shape  = d s d s
  // slicer = d d s s
  // expect = d d d s
  TTS_TYPED_EQUAL(shp(t0, t0, t1, t1).extent<0>(), v0);
  TTS_TYPED_EQUAL(shp(t0, t0, t1, t1).extent<1>(), v0);
  TTS_TYPED_EQUAL(shp(t0, t0, t1, t1).extent<2>(), v0);
  TTS_TYPED_EQUAL(shp(t0, t0, t1, t1).extent<3>(), c0);
};

TTS_CASE("Check slicing with end/n for kwk::from")
{
  using namespace kwk::literals;
  using kwk::range;
  using kwk::from;
  using kwk::fixed;
  using kwk::end;

  constexpr std::size_t d0 = 3, d1 = 5, d2 = 7, d3 = 9;
  auto s1 = kwk::fixed<d1>;
  auto s3 = kwk::fixed<d3>;

  constexpr std::size_t v0 = 2;
  auto c0 = fixed<v0>;
  auto t0 = range(from = end/v0);
  auto t1 = range(from = end/c0);

  auto shp = kwk::of_size(d0, s1, d2, s3);

  TTS_EQUAL( shp(t0, t0, t1, t1), kwk::of_size(d0 - d0/v0, d1 - d1/v0, d2 - d2/c0, d3 - d3/c0));

  // shape  = d s d s
  // slicer = d d s s
  // expect = d d d s
  TTS_TYPED_EQUAL(shp(t0,t0, t1, t1).extent<0>(), d0 - d0/v0);
  TTS_TYPED_EQUAL(shp(t0, t0, t1, t1).extent<1>(), d1 - d1/v0);
  TTS_TYPED_EQUAL(shp(t0, t0, t1, t1).extent<2>(), d2 - d2/c0);
  TTS_TYPED_EQUAL(shp(t0, t0, t1, t1).extent<3>(), fixed<d3 - d3/c0>);
};

TTS_CASE("Check slicing with (k * end / q) for kwk::from")
{
  using namespace kwk::literals;
  using kwk::range;
  using kwk::from;
  using kwk::fixed;
  using kwk::end;

  constexpr std::size_t d0 = 3, d1 = 5, d2 = 7, d3 = 9, d4 = 11, d5 = 13, d6 = 15, d7 = 17;
  auto s4 = kwk::fixed<d4>;
  auto s5 = kwk::fixed<d5>;
  auto s6 = kwk::fixed<d6>;
  auto s7 = kwk::fixed<d7>;

  constexpr std::size_t f = 2;
  constexpr std::size_t d = 5;
  auto fc = fixed<f>;
  auto dc = fixed<d>;

  auto tdd = range(from = end*f/d);
  auto tdc = range(from = end*f/dc);
  auto tcd = range(from = end*fc/d);
  auto tcc = range(from = end*fc/dc);

  auto shp = kwk::of_size(d0, d1, d2, d3, s4, s5, s6, s7);

  auto sliced = shp(tdd, tdc, tcd, tcc, tdd, tdc, tcd, tcc);
  TTS_EQUAL ( sliced
            , kwk::of_size( d0-d0*f/d, d1-d1*f/d, d2-d2*f/d, d3-d3*f/d
                          , d4-d4*f/d, d5-d5*f/d, d6-d6*f/d, d7-d7*f/d)
            );

  // shape  = d d d d s s s s
  // slicer = d d d s d d d s
  // expect = d d d d d d d s
  TTS_TYPED_EQUAL(sliced.extent<0>(), d0-d0*f/d);
  TTS_TYPED_EQUAL(sliced.extent<1>(), d1-d1*f/d);
  TTS_TYPED_EQUAL(sliced.extent<2>(), d2-d2*f/d);
  TTS_TYPED_EQUAL(sliced.extent<3>(), d3-d3*f/d);
  TTS_TYPED_EQUAL(sliced.extent<4>(), d4-d4*f/d);
  TTS_TYPED_EQUAL(sliced.extent<5>(), d5-d5*f/d);
  TTS_TYPED_EQUAL(sliced.extent<6>(), d6-d6*f/d);
  TTS_TYPED_EQUAL(sliced.extent<7>(), fixed<d7-d7*f/d>);
};

TTS_CASE("Check slicing with (k * end / q + dynamic_offset) for kwk::from")
{
  using namespace kwk::literals;
  using kwk::range;
  using kwk::from;
  using kwk::fixed;
  using kwk::end;

  constexpr std::size_t od = 2;
  constexpr std::size_t d0 = 5, d1 = 7, d2 = 9, d3 = 11, d4 = 13, d5 = 15, d6 = 17, d7 = 19;
  auto s4 = kwk::fixed<d4>;
  auto s5 = kwk::fixed<d5>;
  auto s6 = kwk::fixed<d6>;
  auto s7 = kwk::fixed<d7>;

  constexpr std::size_t f = 2;
  constexpr std::size_t d = 5;
  auto fc = fixed<f>;
  auto dc = fixed<d>;

  auto tdd = range(from = end*f /d  + od);
  auto tdc = range(from = end*f /dc + od);
  auto tcd = range(from = end*fc/d  + od);
  auto tcc = range(from = end*fc/dc + od);

  auto shp = kwk::of_size(d0, d1, d2, d3, s4, s5, s6, s7);

  auto sliced = shp(tdd, tdc, tcd, tcc, tdd, tdc, tcd, tcc);
  TTS_EQUAL ( sliced
            , kwk::of_size(d0-d0*f/d-od, d1-d1*f/d-od, d2-d2*f/d-od
            , d3-d3*f/d-od, d4-d4*f/d-od, d5-d5*f/d-od, d6-d6*f/d-od, d7-d7*f/d-od)
            );

  TTS_TYPED_EQUAL(sliced.extent<0>(), d0-d0*f/d-od);
  TTS_TYPED_EQUAL(sliced.extent<1>(), d1-d1*f/d-od);
  TTS_TYPED_EQUAL(sliced.extent<2>(), d2-d2*f/d-od);
  TTS_TYPED_EQUAL(sliced.extent<3>(), d3-d3*f/d-od);
  TTS_TYPED_EQUAL(sliced.extent<4>(), d4-d4*f/d-od);
  TTS_TYPED_EQUAL(sliced.extent<5>(), d5-d5*f/d-od);
  TTS_TYPED_EQUAL(sliced.extent<6>(), d6-d6*f/d-od);
  TTS_TYPED_EQUAL(sliced.extent<7>(), d7-d7*f/d-od);
};

TTS_CASE("Check slicing with (k * end / q + static_offset) for kwk::from")
{
  using namespace kwk::literals;
  using kwk::range;
  using kwk::from;
  using kwk::fixed;
  using kwk::end;

  auto oc = fixed<2UL>;
  constexpr std::size_t d0 = 5, d1 = 7, d2 = 9, d3 = 11, d4 = 13, d5 = 15, d6 = 17, d7 = 19;
  auto s4 = kwk::fixed<d4>;
  auto s5 = kwk::fixed<d5>;
  auto s6 = kwk::fixed<d6>;
  auto s7 = kwk::fixed<d7>;

  constexpr std::size_t f = 2;
  constexpr std::size_t d = 5;
  auto fc = fixed<f>;
  auto dc = fixed<d>;

  auto tdd = range(from = end*f /d  + oc);
  auto tdc = range(from = end*f /dc + oc);
  auto tcd = range(from = end*fc/d  + oc);
  auto tcc = range(from = end*fc/dc + oc);

  auto shp = kwk::of_size(d0, d1, d2, d3, s4, s5, s6, s7);

  auto sliced = shp(tdd, tdc, tcd, tcc, tdd, tdc, tcd, tcc);
  TTS_EQUAL ( sliced
            , kwk::of_size(d0-d0*f/d-oc, d1-d1*f/d-oc, d2-d2*f/d-oc
            , d3-d3*f/d-oc, d4-d4*f/d-oc, d5-d5*f/d-oc, d6-d6*f/d-oc, d7-d7*f/d-oc)
            );

  TTS_TYPED_EQUAL(sliced.extent<0>(), d0-d0*f/d-oc);
  TTS_TYPED_EQUAL(sliced.extent<1>(), d1-d1*f/d-oc);
  TTS_TYPED_EQUAL(sliced.extent<2>(), d2-d2*f/d-oc);
  TTS_TYPED_EQUAL(sliced.extent<3>(), d3-d3*f/d-oc);
  TTS_TYPED_EQUAL(sliced.extent<4>(), d4-d4*f/d-oc);
  TTS_TYPED_EQUAL(sliced.extent<5>(), d5-d5*f/d-oc);
  TTS_TYPED_EQUAL(sliced.extent<6>(), d6-d6*f/d-oc);
  TTS_TYPED_EQUAL(sliced.extent<7>(), fixed<d7-d7*f/d-oc>);
};
