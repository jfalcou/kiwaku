//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

namespace kwk_test
{
  template<typename T>
  struct slicer
  {
    T dim;
    constexpr slicer(T dim_): dim(dim_) { }
  };

  template<auto... S, std::size_t N, typename T>
  constexpr auto reshape( kwk::shape<S...> const& sh
                        , slicer<T> s
                        , kumi::index_t<N> const&
                        ) noexcept
  {
    if constexpr( kwk::concepts::static_constant<T> ) return kwk::fixed<N + 10 * T::value>;
    else                                              return get<N>(sh) + 10 * s.dim;
  }
}

TTS_CASE("Check that slicing keeps dynamic values")
{
  auto shp1 = kwk::of_size(7);
  auto shp2 = kwk::of_size(7, 3);
  auto shp3 = kwk::of_size(7, 3, 5);
  auto shp4 = kwk::of_size(7, 3, 5, 2);
  auto shp7 = kwk::of_size(7, 3, 5, 2, 6, 8, 4);

  auto sl1 = kwk_test::slicer(1);
  auto sl2 = kwk_test::slicer(2);
  auto sl3 = kwk_test::slicer(3);
  auto sl4 = kwk_test::slicer(4);
  auto sl5 = kwk_test::slicer(5);
  auto sl6 = kwk_test::slicer(6);
  auto sl7 = kwk_test::slicer(7);

  auto res1 = shp1(sl1);
  auto res2 = shp2(sl1, sl2);
  auto res3 = shp3(sl1, sl2, sl3);
  auto res4 = shp4(sl1, sl2, sl3, sl4);
  auto res7 = shp7(sl1, sl2, sl3, sl4, sl5, sl6, sl7);

  TTS_EQUAL(get<0>(res1), 17);
  TTS_EQUAL(get<0>(res2), 17);
  TTS_EQUAL(get<0>(res3), 17);
  TTS_EQUAL(get<0>(res4), 17);
  TTS_EQUAL(get<0>(res7), 17);

  TTS_EQUAL(get<1>(res2), 23);
  TTS_EQUAL(get<1>(res3), 23);
  TTS_EQUAL(get<1>(res4), 23);
  TTS_EQUAL(get<1>(res7), 23);

  TTS_EQUAL(get<2>(res3), 35);
  TTS_EQUAL(get<2>(res4), 35);
  TTS_EQUAL(get<2>(res7), 35);

  TTS_EQUAL(get<3>(res4), 42);
  TTS_EQUAL(get<3>(res7), 42);

  TTS_EQUAL(get<4>(res7), 56);
  TTS_EQUAL(get<5>(res7), 68);
  TTS_EQUAL(get<6>(res7), 74);

};

TTS_CASE("Check that slicing keeps static values")
{
  using namespace kwk::literals;

  auto shp1 = kwk::of_size(7);
  auto shp2 = kwk::of_size(7, 3);
  auto shp3 = kwk::of_size(7, 3, 5);
  auto shp4 = kwk::of_size(7, 3, 5, 2);
  auto shp7 = kwk::of_size(7, 3, 5, 2, 6, 8, 4);

  auto sl1 = kwk_test::slicer(1_c);
  auto sl2 = kwk_test::slicer(2_c);
  auto sl3 = kwk_test::slicer(3_c);
  auto sl4 = kwk_test::slicer(4_c);
  auto sl5 = kwk_test::slicer(5_c);
  auto sl6 = kwk_test::slicer(6_c);
  auto sl7 = kwk_test::slicer(7_c);

  auto res1 = shp1(sl1);
  auto res2 = shp2(sl1, sl2);
  auto res3 = shp3(sl1, sl2, sl3);
  auto res4 = shp4(sl1, sl2, sl3, sl4);
  auto res7 = shp7(sl1, sl2, sl3, sl4, sl5, sl6, sl7);

  // 0 + 1 * 10
  TTS_EQUAL(get<0>(res1), 10U);
  TTS_EQUAL(get<0>(res2), 10U);
  TTS_EQUAL(get<0>(res3), 10U);
  TTS_EQUAL(get<0>(res4), 10U);
  TTS_EQUAL(get<0>(res7), 10U);
  // 1 + 2 * 10
  TTS_EQUAL(get<1>(res2), 21U);
  TTS_EQUAL(get<1>(res3), 21U);
  TTS_EQUAL(get<1>(res4), 21U);
  TTS_EQUAL(get<1>(res7), 21U);
  // 2 + 3 * 10
  TTS_EQUAL(get<2>(res3), 32U);
  TTS_EQUAL(get<2>(res4), 32U);
  TTS_EQUAL(get<2>(res7), 32U);

  TTS_EQUAL(get<3>(res4), 43U);
  TTS_EQUAL(get<3>(res7), 43U);

  TTS_EQUAL(get<4>(res7)    , 54U);
  TTS_EQUAL(get<5>(res7)    , 65U);
  TTS_EQUAL(get<6>(res7)    , 76U);
};
