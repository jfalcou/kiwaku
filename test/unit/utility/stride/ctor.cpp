//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>

TTS_CASE("Stride construction from scalar/shapes")
{
  using namespace kwk::literals;

  using kwk::shape;
  using kwk::stride;
  auto s1 = stride(1);
  auto s2 = stride(3, 1_c);
  auto s3 = stride(21, 3, 1_c);

  auto fs1 = stride(1);
  auto fs2 = stride(1_c, 7);
  auto fs3 = stride(1_c, 9, 63);

  auto sp1 = shape(3);
  auto sp2 = shape(7, 3);
  auto sp3 = shape(9, 7, 3);

  TTS_EQUAL(s1, (kwk::stride(sp1, kwk::row_major_order)));
  TTS_EQUAL(s2, (kwk::stride(sp2, kwk::row_major_order)));
  TTS_EQUAL(s3, (kwk::stride(sp3, kwk::row_major_order)));

  TTS_EQUAL(fs1, (kwk::stride(sp1, kwk::column_major_order)));
  TTS_EQUAL(fs2, (kwk::stride(sp2, kwk::column_major_order)));
  TTS_EQUAL(fs3, (kwk::stride(sp3, kwk::column_major_order)));
};
