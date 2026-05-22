//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility.hpp>

TTS_CASE("ratio<T,U> behavior - Constexpr Unary Operators")
{
  using namespace kwk::literals;

  constexpr auto vd = -kwk::ratio{1, 2};
  TTS_CONSTEXPR_EQUAL(vd, (kwk::ratio{-1, 2}));

  constexpr auto vd2 = (-(-kwk::ratio{1, 2}));
  TTS_CONSTEXPR_EQUAL(vd2, (kwk::ratio{1, 2}));

  constexpr auto vd3 = (+kwk::ratio{1, 2});
  TTS_CONSTEXPR_EQUAL(vd3, (kwk::ratio{1, 2}));

  constexpr auto vs = -kwk::ratio{1_c, 2_c};
  TTS_CONSTEXPR_EQUAL(vs, (kwk::ratio{-1_c, 2_c}));

  constexpr auto vs2 = (-(-kwk::ratio{1_c, 2_c}));
  TTS_CONSTEXPR_EQUAL(vs2, (kwk::ratio{1_c, 2_c}));

  constexpr auto vs3 = (+kwk::ratio{1_c, 2_c});
  TTS_CONSTEXPR_EQUAL(vs3, (kwk::ratio{1_c, 2_c}));

  constexpr auto vm = -kwk::ratio{1_c, 2};
  TTS_CONSTEXPR_EQUAL(vm, (kwk::ratio{-1_c, 2}));
};

TTS_CASE("ratio<T,U> behavior - Comparisons (== and <=>)")
{
  using namespace kwk::literals;

  constexpr auto vd1 = kwk::ratio{1, 3};
  constexpr auto vd2 = kwk::ratio{2, 6};
  constexpr auto vd3 = kwk::ratio{5, 7};

  constexpr auto vs1 = kwk::ratio{1_c, 3_c};
  constexpr auto vs2 = kwk::ratio{2_c, 6_c};
  constexpr auto vs3 = kwk::ratio{5_c, 7_c};

  TTS_CONSTEXPR_EQUAL(vd1, vd2);
  TTS_CONSTEXPR_NOT_EQUAL(vd1, vd3);
  TTS_CONSTEXPR_NOT_EQUAL(vd2, vd3);

  TTS_CONSTEXPR_EQUAL(vd1, vs1);
  TTS_CONSTEXPR_EQUAL(vd2, vs2);
  TTS_CONSTEXPR_EQUAL(vd3, vs3);

  TTS_CONSTEXPR_EQUAL(vs1, vs2);
  TTS_CONSTEXPR_NOT_EQUAL(vs1, vs3);
  TTS_CONSTEXPR_NOT_EQUAL(vs2, vs3);

  TTS_EXPECT(vd1 < vd3);
  TTS_EXPECT(vd2 < vd3);
  TTS_EXPECT(vd1 < vs3);
  TTS_EXPECT(vd2 < vs3);

  TTS_EXPECT(vs3 > vs1);
  TTS_EXPECT(vs3 > vs2);
  TTS_EXPECT(vd3 > vs1);
  TTS_EXPECT(vd3 > vs2);
};

TTS_CASE("ratio<T,U> behavior - Arithmetic operations"){TTS_WHEN("Initializing ratios"){auto p1 = kwk::ratio{1, 2};
auto n1 = kwk::ratio{-1, 2};
auto p2 = kwk::ratio{1, -2};
auto n2 = kwk::ratio{-1, -2};

auto z = kwk::ratio{0, 5};
auto one = kwk::ratio{1, 1};

TTS_AND_THEN("Addition")
{
  TTS_EQUAL((p1 + n1).num, 0);
  TTS_EQUAL((n1 + n2).num, 0);
  TTS_EQUAL((p1 + n2), (kwk::ratio{4, 4}));
  TTS_EQUAL((p1 + z), p1);
}

TTS_AND_THEN("Subtraction")
{
  auto r1 = kwk::ratio{1, 2};
  auto r2 = kwk::ratio{1, 4};

  TTS_EQUAL((r1 - r2), (kwk::ratio{2, 8}));
  TTS_EQUAL((p1 - n1), (kwk::ratio{1, 1}));
  TTS_EQUAL((n1 - n1).num, 0);
  TTS_EQUAL((p1 - z), p1);
}

TTS_AND_THEN("Multiplication")
{
  TTS_EQUAL((p1 * p1).num, 1);
  TTS_EQUAL((p1 * n1).num, -1);
  TTS_EQUAL((n1 * p2), (kwk::ratio{1, 4}));
  TTS_EQUAL((p1 * z), z);
  TTS_EQUAL((n1 * one), n1);
}

TTS_AND_THEN("Division")
{
  auto r1 = kwk::ratio{1, 2};
  auto r2 = kwk::ratio{1, 3};

  TTS_EQUAL((r1 / r2), (kwk::ratio{3, 2}));
  TTS_EQUAL((p1 / p1), one);
  TTS_EQUAL((n1 / one), n1);
}
}
}
;
