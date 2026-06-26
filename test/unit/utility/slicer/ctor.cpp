//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>

using kwk::_;

TTS_CASE("Slice behavior - Ensure dynamic informations are kept")
{
  auto shp1 = kwk::shape{7};
  auto shp2 = kwk::shape{7, 3};
  auto shp3 = kwk::shape{7, 3, 5};
  auto shp4 = kwk::shape{7, 3, 5, 2};
  auto shp7 = kwk::shape{7, 3, 5, 2, 6, 8, 4};

  auto sl1 = kwk::slicer{0, 17, 1};
  auto sl2 = kwk::slicer{2, 3, 0};
  auto sl3 = kwk::to_slicer(44);
  auto sl4 = kwk::to_slicer(_);
  auto sl5 = kwk::slicer{1, 5, _};
  auto sl6 = kwk::slicer{_, _, 2};
  auto sl7 = kwk::slicer{_, 3, _};

  TTS_EQUAL(sizeof(sl1), (3 * sizeof(int)));
  TTS_EQUAL(sizeof(sl2), (3 * sizeof(int)));
  TTS_EQUAL(sizeof(sl3), (2 * sizeof(int)));
  TTS_EQUAL(sizeof(sl4), sizeof(kwk::_));
  TTS_EQUAL(sizeof(sl5), (2 * sizeof(int)));
  TTS_EQUAL(sizeof(sl6), (sizeof(int)));
  TTS_EQUAL(sizeof(sl7), (sizeof(int)));

  auto res1 = kwk::reshape(shp1, sl1);
  auto res2 = kwk::reshape(shp2, sl1, sl2);
  auto res3 = kwk::reshape(shp3, sl1, sl2, sl3);
  auto res4 = kwk::reshape(shp4, sl1, sl2, sl3, sl4);
  auto res7 = kwk::reshape(shp7, sl1, sl2, sl3, sl4, sl5, sl6, sl7);

  TTS_EQUAL(res1, (kwk::shape{17}));
  TTS_EQUAL(res2, (kwk::shape{17, 0}));
  TTS_EQUAL(res3, (kwk::shape{17, 0, 1}));
  TTS_EQUAL(res4, (kwk::shape{17, 0, 1, 2}));
  TTS_EQUAL(res7, (kwk::shape{17, 0, 1, 2, 4, 4, 3}));
};

TTS_CASE("Slice behavior"){TTS_WHEN("- Initialize static shape and slicers"){using namespace kwk::literals;
auto shp1 = kwk::shape{7_c};
auto shp2 = kwk::shape{7_c, 3_c};
auto shp3 = kwk::shape{7_c, 3_c, 5_c};
auto shp4 = kwk::shape{7_c, 3_c, 5_c, 2_c};
auto shp7 = kwk::shape{7_c, 3_c, 5_c, 2_c, 6_c, 8_c, 4_c};

auto str1 = kwk::stride{1_c};
auto str2 = kwk::stride{3_c, 1_c};
auto str3 = kwk::stride{15_c, 5_c, 1_c};
auto str4 = kwk::stride{30_c, 10_c, 2_c, 1_c};
auto str7 = kwk::stride{5760_c, 1920_c, 384_c, 192_c, 32_c, 4_c, 1_c};

auto sl1 = kwk::slicer{0_c, 17_c, 1_c};
auto sl2 = kwk::slicer{2_c, 3_c, 0_c};
auto sl3 = kwk::to_slicer(44_c);
auto sl4 = kwk::to_slicer(_);
auto sl5 = kwk::slicer{1_c, 5_c, _};
auto sl6 = kwk::slicer{_, _, 2_c};
auto sl7 = kwk::slicer{_, 3_c, _};

// Each function taking a variadic number of slicers shall be able to take in a tuple of slicers
// auto tp1 = kumi::tuple{sl1};
// auto tp2 = kumi::tuple{sl1, sl2};
// auto tp3 = kumi::tuple{sl1, sl2, sl3};
// auto tp4 = kumi::tuple{sl1, sl2, sl3, sl4};
// auto tp7 = kumi::tuple{sl1, sl2, sl3, sl4, sl5, sl6, sl7};

TTS_AND_THEN("- Check EBO")
{
  TTS_EQUAL(sizeof(sl1), 1UL);
  TTS_EQUAL(sizeof(sl2), 1UL);
  TTS_EQUAL(sizeof(sl3), 1UL);
  TTS_EQUAL(sizeof(sl4), sizeof(kwk::_));
  TTS_EQUAL(sizeof(sl5), sizeof(kwk::_));
  TTS_EQUAL(sizeof(sl6), sizeof(kwk::_));
  TTS_EQUAL(sizeof(sl7), sizeof(kwk::_));
}

TTS_AND_THEN("- Check reshape ")
{
  auto res1 = kwk::reshape(shp1, sl1);
  auto res2 = kwk::reshape(shp2, sl1, sl2);
  auto res3 = kwk::reshape(shp3, sl1, sl2, sl3);
  auto res4 = kwk::reshape(shp4, sl1, sl2, sl3, sl4);
  auto res7 = kwk::reshape(shp7, sl1, sl2, sl3, sl4, sl5, sl6, sl7);

  TTS_EQUAL(res1, (kwk::shape{17_c}));
  TTS_EQUAL(res2, (kwk::shape{17_c, 0_c}));
  TTS_EQUAL(res3, (kwk::shape{17_c, 0_c, 1_c}));
  TTS_EQUAL(res4, (kwk::shape{17_c, 0_c, 1_c, 2_c}));
  TTS_EQUAL(res7, (kwk::shape{17_c, 0_c, 1_c, 2_c, 4_c, 4_c, 3_c}));
}

TTS_AND_THEN("- Check restride ")
{
  auto res1 = kwk::restride(str1, sl1);
  auto res2 = kwk::restride(str2, sl1, sl2);
  auto res3 = kwk::restride(str3, sl1, sl2, sl3);
  auto res4 = kwk::restride(str4, sl1, sl2, sl3, sl4);
  auto res7 = kwk::restride(str7, sl1, sl2, sl3, sl4, sl5, sl6, sl7);

  TTS_EQUAL(res1, (kwk::stride{1_c}));
  TTS_EQUAL(res2, (kwk::stride{3_c, 0_c}));
  TTS_EQUAL(res3, (kwk::stride{15_c, 0_c, 1_c}));
  TTS_EQUAL(res4, (kwk::stride{30_c, 0_c, 2_c, 1_c}));
  TTS_EQUAL(res7, (kwk::stride{5760_c, 0_c, 384_c, 192_c, 32_c, 8_c, 1_c}));
}

TTS_AND_THEN("- Check origin")
{
  auto res1 = kwk::origin(shp1, sl1);
  auto res2 = kwk::origin(shp2, sl1, sl2);
  auto res3 = kwk::origin(shp3, sl1, sl2, sl3);
  auto res4 = kwk::origin(shp4, sl1, sl2, sl3, sl4);
  auto res7 = kwk::origin(shp7, sl1, sl2, sl3, sl4, sl5, sl6, sl7);

  TTS_EQUAL(res1, (kumi::tuple{0_c}));
  TTS_EQUAL(res2, (kumi::tuple{0_c, 2_c}));
  TTS_EQUAL(res3, (kumi::tuple{0_c, 2_c, 44_c}));
  TTS_EQUAL(res4, (kumi::tuple{0_c, 2_c, 44_c, 0}));
  TTS_EQUAL(res7, (kumi::tuple{0_c, 2_c, 44_c, 0, 1_c, 0, 0}));
}
}
}
;
#include <iostream>

TTS_CASE("Slice behavior - Ensure specific slice behave as intended")
{
  auto shp1 = kwk::shape{7};

  auto sl1 = kwk::at(4);
  auto sl2 = kwk::every(3);
  auto sl3 = kwk::first(5);
  auto sl4 = kwk::last(2);
  auto sl5 = kwk::drop_last(3);
  auto sl6 = kwk::drop_first(4);
  auto sl7 = kwk::between(2, 5);

  TTS_EQUAL(sizeof(sl1), (2 * sizeof(int)));
  TTS_EQUAL(sizeof(sl2), (sizeof(int)));
  TTS_EQUAL(sizeof(sl3), (sizeof(int)));
  TTS_EQUAL(sizeof(sl4), (2 * sizeof(int)));
  TTS_EQUAL(sizeof(sl5), (2 * sizeof(int)));
  TTS_EQUAL(sizeof(sl6), (sizeof(int)));
  TTS_EQUAL(sizeof(sl7), (2 * sizeof(int)));

  auto res1 = kwk::reshape(shp1, sl1);
  auto res2 = kwk::reshape(shp1, sl2);
  auto res3 = kwk::reshape(shp1, sl3);
  auto res4 = kwk::reshape(shp1, sl4);
  auto res5 = kwk::reshape(shp1, sl5);
  auto res6 = kwk::reshape(shp1, sl6);
  auto res7 = kwk::reshape(shp1, sl7);

  TTS_EQUAL(res1, (kwk::shape{1}));
  TTS_EQUAL(res2, (kwk::shape{3}));
  TTS_EQUAL(res3, (kwk::shape{5}));
  TTS_EQUAL(res4, (kwk::shape{2}));
  TTS_EQUAL(res5, (kwk::shape{4}));
  TTS_EQUAL(res6, (kwk::shape{3}));
  TTS_EQUAL(res7, (kwk::shape{3}));
};
